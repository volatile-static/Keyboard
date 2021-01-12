module ps2_rx #(DATA_TIMEOUT = 20) (
	input clock_quarter, reset, start,
	output ready, faild, finish, // parity check faild, etc...
	output reg[7:0] buffer,  // store received data
	inout tri PS2_CLK, PS2_DAT
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/
localparam 
	IDEL   = 6'b000001,
    START  = 6'b000010,
	DATA   = 6'b000100,
	PARITY = 6'b001000,
	STOP   = 6'b010000,
	ACK    = 6'b100000;

/*****************************************************************************
 *                           REG/WIRE Declarations                           *
 *****************************************************************************/

reg[7:0] wait_cnt;
reg[5:0] curr_state, next_state;
reg[2:0] byte_cnt;  // 8 bits/byte
reg[1:0] bit_cnt;  // 4 states/bit
reg clk_sync, dat_sync;
reg parity_read;

wire bit_finish = bit_cnt == 2'b11;
wire rx_timeout = wait_cnt > DATA_TIMEOUT;

/*****************************************************************************
 *                            Combinational logic                            *
 *****************************************************************************/

assign PS2_CLK = (^bit_cnt) ? 1'b0 : 1'bz;
assign PS2_DAT = (curr_state == ACK && bit_cnt) ? 1'b0 : 1'bz;
 
assign ready = curr_state == IDEL;
assign finish = rx_timeout || (bit_finish && curr_state == ACK);
assign faild  = rx_timeout || (bit_finish && (
	(curr_state == PARITY  && (^buffer) != dat_sync) |
	(curr_state == STOP    && !dat_sync)));

/*****************************************************************************
 *                             Sequential logic                              *
 *****************************************************************************/

always @(posedge clock_quarter)
	begin
	  	clk_sync <= PS2_CLK;
		dat_sync <= PS2_DAT;
	end

always @(posedge clock_quarter)
	if (curr_state != IDEL && curr_state != START)
		bit_cnt <= bit_cnt + 1;
	else
		bit_cnt <= 0;

// count when receiving 8bit data
always @(posedge clock_quarter)
	if (curr_state != DATA)
		byte_cnt <= 0;
	else if (bit_finish)
		byte_cnt <= byte_cnt + 1;
	else
		byte_cnt <= byte_cnt;

always @(posedge clock_quarter, posedge reset)
	if (reset)
		buffer <= 8'h0;
	else if (curr_state == START)
		buffer <= 8'h0;
	else if (curr_state == DATA && bit_finish) // latch PS2_DAT at posedge PS2_CLK
		buffer <= {dat_sync, buffer[7:1]};  // LSB first, right shift
	else
		buffer <= buffer;

always @(posedge clock_quarter)
	if (curr_state != START)
		wait_cnt <= 0;
	else
		wait_cnt <= wait_cnt + 1;

/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/
always @ (posedge clock_quarter, posedge reset)
	if (reset)
		curr_state = IDEL; 
	else
		curr_state = next_state;
		
always @ *
	case (curr_state)
		IDEL: 
			if (start)
				next_state = START;
			else
				next_state = IDEL;
		START:
			if (clk_sync && !dat_sync)
				next_state = DATA;
			else if (rx_timeout)
				next_state = IDEL;
			else
				next_state = START;
		DATA:
			if (byte_cnt == 3'd7 && bit_finish)
				next_state = PARITY;
			else
				next_state = DATA;
		PARITY:
			if (bit_finish)
			 	next_state = STOP;
			else
				next_state = PARITY;
		STOP:
			if (bit_finish)
				next_state = ACK;
			else
				next_state = STOP;
		ACK:
			if (bit_finish)
				next_state = IDEL;
			else
				next_state = ACK;
		default: next_state = IDEL;
	endcase

endmodule
