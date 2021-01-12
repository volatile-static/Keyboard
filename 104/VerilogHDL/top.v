`define SCAN_KEY_PERIOD 666 
`define SCAN_LED_PERIOD 299 
`define POR 50

module top(
	input CLK_50, KEY_Fn,
	input[20:0] COL,
	output[5:0] ROW, LED_A, 
	output[34:0] LED_K,
	output TXD, LED_R6,
	inout tri PS2_CLK, PS2_DAT
);


wire reset, on_event;
wire[7:0] key_event;
wire[103:1] key_down;

assign LED_R6 = on_event;

reset_controller rst_ctl(.clock(CLK_50), .KEY_Fn(KEY_Fn), .reset(reset));
scan_key scan_k(.clock(CLK_50), .reset(reset), .COL(COL), .ROW(ROW), .enabled(1), .key_down(key_down));
scan_led scan_d(.clock(CLK_50), .reset(reset), .LED_K(LED_K), .LED_A(LED_A), .enabled(1), .led_matrix(0));
keyboard_send com(.clock(CLK_50), .reset(reset), .key_down(key_down), .key_event(key_event), .on_event(on_event));
keyboard_ps2 kbd_ps2(.PS2_CLK(PS2_CLK), .PS2_DAT(PS2_DAT), .clock_50(CLK_50), .reset(reset), .key_pending(on_event));
keyboard_hid kbd_hid(.clock(CLK_50), .reset(reset), .key_event(key_event), .on_event(on_event), .TXD(TXD));

// wire[7:0] deb;
// wire uart_en, tx_ready;
// ps2_keyboard(
// 	.clock_50(CLK_50),
// 	.reset(reset),
// 	.data_valid(0),
// 	.special(0),
// 	.break(0),
// 	.scan_code(0),
// 	.tx_buffer(deb),
// 	.write_en(uart_en),
// 	.PS2_CLK(PS2_CLK),
// 	.PS2_DAT(PS2_DAT)
// );
// uart_tx #(.BAUD_RATE(9600)) ch9328(
// 	.clk(CLK_50), 
// 	.rst_n(!reset), 
// 	.tx_data(deb), 
// 	.tx_data_valid(uart_en && tx_ready), 
// 	.tx_data_ready(tx_ready), 
// 	.tx_pin(TXD)
// );

//wire[127:0] rom_q;
//rom (0, CLK_50, rom_q);
//assign ROW = rom_q[5:0];
//assign LED_A = rom_q[11:6];
//assign LED_K = rom_q[46:12];

endmodule
