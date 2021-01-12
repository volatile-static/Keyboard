module ps2_bus #(BYTE_DELAY = 80) (  // delay between frames
	input clock_quarter, reset, enable,  // start send @ posedge enable
	input[7:0] tx_data,  // data to send
	output[7:0] rx_data,  // data received
	output reg tx_faild, 
	output tx_ready, rx_complete,  // transmit status
	inout tri PS2_CLK, PS2_DAT  // open-collector bus
);

/*****************************************************************************\
|*                           Parameter Declarations                          *|
\*****************************************************************************/
localparam
	BUS_IDEL = 3'b001,
	BUS_SEND = 3'b010,
	BUS_READ = 3'b100;

/*****************************************************************************\
|*                 Internal wires and registers Declarations                 *|
\*****************************************************************************/

reg[2:0] curr_state, next_state;

wire tx_idel, tx_abort, tx_finish;
wire rx_idel, rx_faild, rx_finish;
reg clk_sync;
reg[7:0] delay_cnt, tx_latch;

/*****************************************************************************\
|*                            Combinational logic                            *|
\*****************************************************************************/

wire start_tx = curr_state == BUS_SEND && delay_cnt > BYTE_DELAY && tx_idel;
wire start_rx = curr_state == BUS_READ && rx_idel;
assign tx_ready = curr_state == BUS_IDEL;
assign rx_complete = rx_finish & !rx_faild;

/*****************************************************************************\
|*                         Finite State Machine(s)                           *|
\*****************************************************************************/

always @(posedge clock_quarter, posedge reset)
	if (reset)
		curr_state <= BUS_IDEL;
	else
		curr_state <= next_state;

always @ *
	case (curr_state)
		BUS_IDEL:
			if (!clk_sync)
				next_state = BUS_READ;
			else if (enable)
				next_state = BUS_SEND;
			else
				next_state = BUS_IDEL;
		BUS_SEND:
			if (tx_finish || tx_abort)
				next_state = BUS_IDEL;
			else
				next_state = BUS_SEND;
		BUS_READ:
			if (rx_finish)
				next_state = BUS_IDEL;
			else
				next_state = BUS_READ;
		default:next_state = BUS_IDEL;
	endcase

/*****************************************************************************
 *                             Sequential logic                              *
 *****************************************************************************/

always @(posedge clock_quarter, posedge reset)
	if (reset)  // async reset
		delay_cnt <= 0;
	else if (rx_finish | tx_finish)  // sync reset
		delay_cnt <= 0;
	else
		delay_cnt <= delay_cnt + 1;
	
always @(posedge clock_quarter)
	clk_sync <= PS2_CLK;  // sync clock region

always @(posedge clock_quarter, posedge reset)
	if (reset)
		tx_latch <= 0;
	else if (curr_state == BUS_IDEL && enable)  // only 1 clock
		tx_latch <= tx_data;
	else
		tx_latch <= tx_latch;

always @(posedge clock_quarter, posedge reset)
	if (reset)
		tx_faild <= 0;
	else if (curr_state == BUS_SEND)
		tx_faild <= tx_faild | tx_abort;
	else if (curr_state == BUS_IDEL && enable)
		tx_faild <= 0;
	else
		tx_faild <= tx_faild;

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
	.faild(rx_faild),
	.ready(rx_idel),
	.finish(rx_finish),
	.buffer(rx_data),
	.reset(reset),
	.clock_quarter(clock_quarter)
);

endmodule
