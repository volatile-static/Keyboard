module keyboard_ps2(
	input clock_50, reset,
	output reg num_lock, scroll_lock, caps_lock,
	inout tri PS2_CLK, PS2_DAT
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/
 
localparam 
	INIT    = 5'b00001,
	LISTEN  = 5'b00010,
	ACK     = 5'b10000;

/*****************************************************************************
 *                           REG/WIRE Declarations                           *
 *****************************************************************************/

wire clock_quarter;

reg[7:0] tx_buffer, rx_buffer;
wire[7:0] rx_data;
wire rx_complete, tx_ready, tx_faild;
reg write_en;
reg[4:0] curr_state, next_state;


/*****************************************************************************
 *                            Combinational logic                            *
 *****************************************************************************/

divider #(499) clk_gen(clock_50, clock_quarter);  // 50MHZ / 2(499 + 1) = 20us

/*****************************************************************************
 *                             Sequential logic                              *
 *****************************************************************************/
	
		
/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/

always @(posedge clock_quarter, posedge reset)
	if (reset)
		curr_state <= INIT;
	else
		curr_state <= next_state;

ps2_bus ps2_bus(
	.clock_quarter(clock_quarter),
	.reset(reset),
	.enable(write_en),
	.tx_data(tx_buffer),
	.rx_data(rx_data),
	.tx_ready(tx_ready),
	.tx_faild(tx_faild),
	.rx_complete(rx_complete),
	.PS2_CLK(PS2_CLK),
	.PS2_DAT(PS2_DAT)
);

// always @ *
// 	case (curr_state)
// 		INIT: 
// 			next_state <= SEND;
// 		LISTEN:
// 			if (!clk_sync)
// 				next_state <= RECEIVE;
// 			else
// 				next_state <= LISTEN;
// 		RECEIVE: 
// 			if (rx_finish)
// 				next_state <= ACK;
// 			else
// 				next_state <= RECEIVE;
// 		SEND: 
// 			if (tx_finish && !tx_faild)
// 				case (rx_buffer)
// 					8'hff: next_state <= INIT;
// 					8'hed: next_state <= RECEIVE;
// 					8'hf3: next_state <= RECEIVE;
// 					default: next_state <= LISTEN;
// 				endcase
// 			else
// 				next_state <= SEND;
// 		ACK:
// 			next_state <= SEND;
// 		default: next_state <= INIT;
// 	endcase

// always @(posedge clock_quarter, posedge reset)
// 	if (reset)
// 		begin
// 			tx_buffer <= 1'b0;
// 			start_tx <= 1'b0;
// 			start_rx <= 0;
// 			rx_buffer <= 0;
// 		end
// 	else
// 		case (curr_state)
// 			INIT: begin
// 				start_tx <= 0;
// 				start_rx <= 0;
// 				tx_buffer <= 8'haa;
// 				rx_buffer <= 0;
// 			end
// 			RECEIVE: 
// 				if (rx_idel)
// 					start_rx <= 1;
// 				else
// 					start_rx <= 0;
// 			SEND: 
// 				if (delay_cnt > 16'd50000 && tx_idel)
// 					start_tx <= 1;
// 				else
// 					start_tx <= 0;
// 			ACK: begin
// 				rx_buffer <= rx_data;
// 				tx_buffer <= 8'hfa;
// 			end
// 			default: ;
// 		endcase

endmodule
