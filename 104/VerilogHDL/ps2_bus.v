module ps2_bus #(BYTE_DELAY = 80) (  // delay between frames
	input clock_quarter, reset, tx_valid,  
	input[7:0] tx_payload,  // data sending to host
	output[7:0] rx_payload,  // data received from host
	output rx_valid, tx_ready, // tx stream & rx flow
	output reg tx_failed,  // transmition status
	inout tri PS2_CLK, PS2_DAT  // open-collector bus
);

/*****************************************************************************\
|*                           Parameter Declarations                          *|
\*****************************************************************************/
localparam
	BUS_IDLE = 3'b001,
	BUS_SEND = 3'b010,
	BUS_READ = 3'b100;

/*****************************************************************************\
|*                 Internal wires and registers Declarations                 *|
\*****************************************************************************/

reg[2:0] curr_state, next_state;

wire tx_idel, tx_abort, tx_finish;
wire rx_idel, rx_failed, rx_finish;
reg clk_sync;
reg[7:0] delay_cnt, tx_latch;

/*****************************************************************************\
|*                            Combinational logic                            *|
\*****************************************************************************/

wire start_tx = curr_state == BUS_SEND && tx_idel && delay_cnt > BYTE_DELAY;
wire start_rx = curr_state == BUS_READ && rx_idel;
assign tx_ready = curr_state == BUS_IDLE;
assign rx_valid = rx_finish & !rx_failed;

/*****************************************************************************\
|*                         Finite State Machine(s)                           *|
\*****************************************************************************/

always @(posedge clock_quarter, posedge reset)
	if (reset)
		curr_state <= BUS_IDLE;
	else
		curr_state <= next_state;

always @ *
	case (curr_state)
		BUS_IDLE:
			if (!clk_sync)
				next_state = BUS_READ;
			else if (tx_valid)
				next_state = BUS_SEND;
			else
				next_state = BUS_IDLE;
		BUS_SEND:
			if (tx_finish || tx_abort)
				next_state = BUS_IDLE;
			else
				next_state = BUS_SEND;
		BUS_READ:
			if (rx_finish)
				next_state = BUS_IDLE;
			else
				next_state = BUS_READ;
		default:next_state = BUS_IDLE;
	endcase

/*****************************************************************************\
|*                             Sequential logic                              *|
\*****************************************************************************/

always @(posedge clock_quarter, posedge reset)
	if (reset)  // async reset
		delay_cnt <= 0;
	else if (rx_finish | tx_finish)  // sync reset
		delay_cnt <= 0;
	else
		delay_cnt <= delay_cnt + 1;
	
always @(posedge clock_quarter)
	clk_sync <= PS2_CLK;  // sync clock domain

always @(posedge clock_quarter, posedge reset)
	if (reset)
		tx_latch <= 0;
	else if (curr_state == BUS_IDLE && tx_valid)  // only 1 clock
		tx_latch <= tx_payload;
	else
		tx_latch <= tx_latch;

always @(posedge clock_quarter, posedge reset)
	if (reset)
		tx_failed <= 0;
	else if (curr_state == BUS_SEND)
		tx_failed <= tx_failed | tx_abort;
	else if (curr_state == BUS_IDLE && tx_valid)
		tx_failed <= 0;
	else
		tx_failed <= tx_failed;

/*****************************************************************************\
|*                              Internal Modules                             *|
\*****************************************************************************/

ps2_tx transmitter(
	.PS2_CLK(PS2_CLK), 
	.PS2_DAT(PS2_DAT),
	.start(start_tx),
	.abort(tx_abort),
	.ready(tx_idel),
	.finish(tx_finish),
	.buffer(tx_latch),
	.reset(reset),
	.clock_quarter(clock_quarter)
);

ps2_rx receiver(
	.PS2_CLK(PS2_CLK), 
	.PS2_DAT(PS2_DAT),
	.start(start_rx),
	.faild(rx_failed),
	.ready(rx_idel),
	.finish(rx_finish),
	.buffer(rx_payload),
	.reset(reset),
	.clock_quarter(clock_quarter)
);

endmodule
