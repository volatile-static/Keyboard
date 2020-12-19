module keyboard_send(
	input clock, reset, row_sync,
	input[102:0] key_down,
	output[7:0] key_event,
	output on_event
);

reg[6:0] cnt;
reg[102:0] key_last;

always @(posedge clock, posedge reset)
	if (reset)
		key_last <= 0;
	else if (row_sync)
		key_last <= key_down;
	else
		key_last <= key_last;

always @(posedge clock, posedge reset)
	if (reset)  // async
		cnt <= 0;
	else if (cnt >= 102)  // sync
		cnt <= 0;
	else
		cnt <= cnt + 1;

assign on_event = key_last[cnt] ^ key_down[cnt];
assign key_event = on_event ? {key_last[cnt], cnt} : 0;

endmodule
