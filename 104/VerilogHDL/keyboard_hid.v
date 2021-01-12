module keyboard_hid #(DELAY = 20) (
	input clock, reset, on_event,
	input[7:0] key_event,
	output TXD
);

localparam IDEL = 4'b0000,
		   SPECIAL = 4'b0001,
		   RESERVED = 4'b0011,
		   KEY1 = 4'b0010,
		   KEY2 = 4'b0110,
		   KEY3 = 4'b0111,
		   KEY4 = 4'b0101,
		   KEY5 = 4'b0100,
		   KEY6 = 4'b1100,
		   FINISH = 4'b1000;

reg[3:0] curr_state, next_state;
reg[7:0] key_sync, hid_data;
reg[31:0] delay;

(* ram_init_file = "source/hid.mif" *)
reg[7:0] hid_key_map[0 : 127];
wire uart_en = curr_state != IDEL && curr_state != FINISH;

always @(posedge clock, posedge reset)
	if (reset)
		key_sync <= 0;
	else if (on_event)
		key_sync <= key_event;
	else
		key_sync <= key_sync;

always @(posedge clock, posedge reset)
	if (reset)
		delay <= 0;
	else if (curr_state == FINISH)
		delay <= delay + 1; 
	else
		delay <= 0;

always @(posedge clock, posedge reset)
	if (reset)
		curr_state <= IDEL;
	else
		curr_state <= next_state;

always @(curr_state, on_event, tx_ready) 
	begin: state_switch
	  	next_state = curr_state;
		if (tx_ready)
			case (curr_state)
				IDEL:
					if (on_event)
						next_state = SPECIAL;
				SPECIAL: next_state = RESERVED;
				RESERVED: next_state = KEY1;
				KEY1: next_state = KEY2;
				KEY2: next_state = KEY3;
				KEY3: next_state = KEY4;
				KEY4: next_state = KEY5;
				KEY5: next_state = KEY6;
				KEY6: next_state = FINISH;
				FINISH: 
					if (delay[DELAY]) 
						next_state = IDEL;
				default: next_state = IDEL;
			endcase
	end

always @(posedge clock, posedge reset)
	if (reset)
		hid_data <= 0;
	else case (curr_state)
		IDEL: hid_data <= 0;
		SPECIAL: hid_data <= 0;
		RESERVED: hid_data <= 0;
		KEY1: hid_data <= key_sync[7] ? hid_key_map[key_sync[6:0]] : 0;
		KEY2: hid_data <= 0;
		KEY3: hid_data <= 0;
		KEY4: hid_data <= 0;
		KEY5: hid_data <= 0;
		KEY6: hid_data <= 0;
		FINISH: hid_data <= 0;
		default: hid_data <= 0;
	endcase

uart_tx #(.BAUD_RATE(9600)) ch9328(
	.clk(clock), 
	.rst_n(!reset), 
	.tx_data(hid_data), 
	.tx_data_valid(uart_en), 
	.tx_data_ready(tx_ready), 
	.tx_pin(TXD)
);

endmodule
