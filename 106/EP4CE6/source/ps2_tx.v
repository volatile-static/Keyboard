module ps2_tx(
	input start, clock_quarter, reset,
	input[7:0] buffer,  // data to send
	output ready, finish,
	output reg abort,  // transmit is canceled by host
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
	DELAY  = 6'b100000;


//=======================================================
//  REG/WIRE declarations
//=======================================================

reg tx_en;  // transmit edabled
reg[5:0] curr_state, next_state;
reg[2:0] byte_cnt;  // 8 bits/byte
reg[1:0] bit_cnt;  // 4 states/bit

wire bit_finish = bit_cnt == 2'b11;
wire tx_delay = curr_state == DELAY;


/*****************************************************************************
 *                            Combinational logic                            *
 *****************************************************************************/
function dat_out(input en);
	if (!en)
		dat_out = 1'bz;
	else if (curr_state == START)
		dat_out = 1'b0;
	else if (curr_state == DATA)
		dat_out = buffer[byte_cnt] ? 1'bz : 1'b0;
	else if (curr_state == PARITY)
		dat_out = (^buffer) ? 1'b0 : 1'bz;
	else
		dat_out = 1'bz;
endfunction

assign PS2_DAT = dat_out(tx_en);  // asserting PS2_DAT before negedge PS2_CLK
assign PS2_CLK = (^bit_cnt) ? 1'b0 : 1'bz;
assign ready = curr_state == IDEL;			
assign finish = curr_state == STOP && bit_finish;

/*****************************************************************************
 *                             Sequential logic                              *
 *****************************************************************************/

always @ (posedge start, posedge tx_delay)
	if (tx_delay)
		tx_en <= 0;
	else
		tx_en <= 1;
		
// the host pull down PS2_CLK to stop transmit
always @ (posedge clock_quarter, posedge tx_delay)
	if (tx_delay)
		abort <= 0;
	else
		abort <= bit_finish && !PS2_CLK;
		
// count 4 clock_quarter in a bit when transmitting
always @ (posedge clock_quarter)
	if (tx_en && curr_state != IDEL)
		bit_cnt <= bit_cnt + 1;
	else
		bit_cnt <= 0;

// count 8 bit when sending data
always @ (posedge clock_quarter)
	if (curr_state != DATA)
		byte_cnt <= 0;
	else if (bit_finish)
		byte_cnt <= byte_cnt + 1;
	else
		byte_cnt <= byte_cnt;

/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/
always @ (posedge clock_quarter, posedge reset)
	if (reset)
		curr_state = DELAY; 
	else
		curr_state = next_state;
		
always @ *
	if (abort)
		next_state <= DELAY;
	else 
	case (curr_state)
		IDEL: 
			if (tx_en)
				next_state <= START;
			else
				next_state <= IDEL;
		START:
			if (bit_finish)
				next_state <= DATA;
			else
				next_state <= START;
		DATA:
			if (byte_cnt == 3'd7 && bit_finish)
				next_state <= PARITY;
			else
				next_state <= DATA;
		PARITY:
			if (bit_finish)
			 	next_state <= STOP;
			else
				next_state <= PARITY;
		STOP:
			if (bit_finish)
				next_state <= DELAY;
			else
				next_state <= STOP;
		DELAY:
			next_state <= IDEL;  // why need delay?
		default: next_state <= DELAY;
	endcase
	
endmodule
