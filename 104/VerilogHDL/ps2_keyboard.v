module ps2_keyboard(
	input clock_50, reset, data_valid, special, break,
	input[7:0] scan_code,
	output reg num_lock, scroll_lock, caps_lock,
	inout tri PS2_CLK, PS2_DAT
);

/*****************************************************************************\
|*                           Parameter Declarations                          *|
\*****************************************************************************/

localparam 
	INIT    = 7'b000_0001,
	LISTEN  = 7'b000_0010,
	SPECIAL = 7'b000_0100,
	BREAK   = 7'b000_1000,
	NORMAL  = 7'b001_0000,
	RECEIVE = 7'b010_0000,
	ACK     = 7'b100_0000;

/*****************************************************************************\
|*                 Internal wires and registers Declarations                 *|
\*****************************************************************************/

reg[7:0] tx_buffer;
wire[7:0] rx_data;
wire tx_ready, tx_faild;
reg[6:0] curr_state = INIT, next_state;
wire clock_quarter;

/*****************************************************************************\
|*                            Combinational logic                            *|
\*****************************************************************************/

wire write_en = tx_ready & (
	curr_state == INIT |
	curr_state == SPECIAL | 
	curr_state == BREAK | 
	curr_state == NORMAL | 
	curr_state == ACK);

always @*
	case (curr_state)
 		INIT: tx_buffer = 8'haa;
		// LISTEN: ;
		SPECIAL: tx_buffer = 8'he0;
		BREAK: tx_buffer = 8'hf0;
		NORMAL: tx_buffer = scan_code;
		// RECEIVE: ;
 		ACK: tx_buffer = 8'hfa;
 		default: tx_buffer = 0;
 	endcase

/*****************************************************************************\
|*                         Finite State Machine(s)                           *|
\*****************************************************************************/

always @(posedge clock_quarter, posedge reset)
	if (reset)
		curr_state <= INIT;
	else
		curr_state <= next_state;

always @ *
	if (rx_complete)
		next_state = ACK;  // highest priority
	else if (tx_ready)
 		case (curr_state)
 			INIT: 
			 	if (tx_faild)
				 	next_state = INIT;
				else
			 		next_state = LISTEN;
 			LISTEN:
 				if (!data_valid)
 					next_state = LISTEN;
 				else if (special)
 					next_state = SPECIAL;
				else if (break)
					next_state = BREAK;
				else 
					next_state = NORMAL;
			SPECIAL:
				if (break)
 					next_state = BREAK;
				else
					next_state = NORMAL;
			BREAK:
 				next_state = NORMAL;
			NORMAL:
				next_state = LISTEN;
			RECEIVE:
				next_state = RECEIVE;
 			ACK:
			 	case (rx_data)
 					8'hff: next_state = INIT;
 					8'hed: next_state = RECEIVE;
 					8'hf3: next_state = RECEIVE;
 					default: next_state = LISTEN;
 				endcase
 			default: next_state = INIT;
 		endcase
	else
		next_state = curr_state;

/*****************************************************************************\
|*                              Internal Modules(s)                          *|
\*****************************************************************************/

divider #(499) clk_div(clock_50, clock_quarter);  // 50MHZ / 2(499 + 1) = 20us

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

endmodule
