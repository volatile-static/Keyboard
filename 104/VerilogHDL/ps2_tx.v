module ps2_tx(
	input start, clock_quarter, reset,
	input[7:0] buffer,  // data to send
	output ready, finish,
	output abort,  // transmit is canceled by host
	inout tri PS2_CLK, 
	inout reg PS2_DAT
);

/*****************************************************************************\
|*                           Parameter Declarations                          *|
\*****************************************************************************/
localparam 
	IDLE   = 6'b000001,
	START  = 6'b000010,
	DATA   = 6'b000100,
	PARITY = 6'b001000,
	STOP   = 6'b010000,
	DELAY  = 6'b100000;

/*****************************************************************************\
|*                 Internal wires and registers Declarations                 *|
\*****************************************************************************/

reg tx_en;  // transmit edabled
reg[5:0] curr_state, next_state;
reg[2:0] byte_cnt;  // 8 bits/byte
reg[1:0] bit_cnt;  // 4 states/bit

/*****************************************************************************\
|*                            Combinational logic                            *|
\*****************************************************************************/

wire bit_finish = bit_cnt == 2'b11;
wire tx_delay = curr_state == DELAY;

assign ready = curr_state == IDLE;			
assign finish = curr_state == STOP && bit_finish;
assign abort = !tx_delay && bit_finish && !PS2_CLK;  // Inhibit(the host pull down PS2_CLK to stop transmition)
assign PS2_CLK = (^bit_cnt) ? 1'b0 : 1'bz;

always @ *  // asserting PS2_DAT before negedge PS2_CLK
	if (!tx_en)
		PS2_DAT = 1'bz;
	else if (curr_state == START)
		PS2_DAT = 1'b0;
	else if (curr_state == DATA)
		PS2_DAT = buffer[byte_cnt] ? 1'bz : 1'b0;
	else if (curr_state == PARITY)
		PS2_DAT = (^buffer) ? 1'b0 : 1'bz;
	else
		PS2_DAT = 1'bz;

/*****************************************************************************\
|*                             Sequential logic                              *|
\*****************************************************************************/

always @ (posedge start, posedge tx_delay)
	if (tx_delay)
		tx_en <= 0;
	else
		tx_en <= 1;
				
// count 4 clock_quarter in a bit when transmitting
always @ (posedge clock_quarter)
	if (tx_en && curr_state != IDLE)
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

/*****************************************************************************\
|*                         Finite State Machine(s)                           *|
\*****************************************************************************/

always @ (posedge clock_quarter, posedge reset)
	if (reset)
		curr_state = DELAY; 
	else
		curr_state = next_state;
		
always @ *
	if (abort)
		next_state = DELAY;
	else 
	case (curr_state)
		IDLE: 
			if (tx_en)
				next_state = START;
			else
				next_state = IDLE;
		START:
			if (bit_finish)
				next_state = DATA;
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
				next_state = DELAY;
			else
				next_state = STOP;
		DELAY:
			next_state = IDLE;  // why need delay?
		default: next_state = DELAY;
	endcase
	
endmodule
