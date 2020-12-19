`define SCAN_KEY_PERIOD 25 
`define SCAN_LED_PERIOD 299 
`define POR 50

module top(
	input CLK_50, KEY_Fn,
	input[20:0] COL,
	output[5:0] ROW, LED_A, 
	output[34:0] LED_K,
	output TXD, LED_R6,
	inout PS2_CLK, PS2_DAT
);

wire reset, txen, row_sync;
wire[7:0] key_event;
wire[103:1] key_down;

assign LED_R6 = reset;

reset_controller rst_ctl(.clock(CLK_50), .KEY_Fn(KEY_Fn), .reset(reset));
scan_key scan_k(.clock(CLK_50), .reset(reset), .COL(COL), .ROW(ROW), .enabled(1), .key_down(key_down), .row_sync(row_sync));
scan_led scan_d(.clock(CLK_50), .reset(reset), .LED_K(LED_K), .LED_A(LED_A), .enabled(1));
keyboard_send com(.clock(CLK_50), .reset(reset), .key_down(key_down), .key_event(key_event), .on_event(txen), .row_sync(row_sync));
keyboard_ps2 kbd_ps2(.PS2_CLK(PS2_CLK), .PS2_DAT(PS2_DAT), .clock_50(CLK_50), .reset(reset));
uart_tx uart(.sys_clk(CLK_50), .sys_rst_n(!reset), .uart_din(key_event), .uart_txd(TXD), .uart_en(txen));


endmodule
