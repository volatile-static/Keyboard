module keyboard_ps2(
	input clock_50, reset, key_pending, 
	input[7:0] key_value,
	output reg num_lock, scroll_lock, caps_lock,
	inout tri PS2_CLK, PS2_DAT
);

/*****************************************************************************\
|*                 Internal wires and registers Declarations                 *|
\*****************************************************************************/

wire[7:0] tx_data;

/*****************************************************************************\
|*                              Internal Modules(s)                          *|
\*****************************************************************************/

ps2_keyboard(
	.clock_50(clock_50),
	.reset(reset),
	.data_valid(key_pending),
	.special(0),
	.break(0),
	.scan_code(8'haa),
	.PS2_CLK(PS2_CLK),
	.PS2_DAT(PS2_DAT)
);

fifo(
	.clk(clock_50),
	.rst_n(!reset),
	.data_out(tx_data)
);

endmodule
