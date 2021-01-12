module keyboard_send(
	input clock, reset, row_sync,
	input[102:0] key_down,
	output[7:0] key_event,
	output on_event
);

reg[6:0] cnt;
wire[102:0] key_valid, key_value;

always @(posedge clock, posedge reset)
	if (reset)  // async
		cnt <= 0;
	else if (cnt >= 102)  // sync
		cnt <= 0;
	else
		cnt <= cnt + 1;

assign on_event = key_valid[cnt];
assign key_event = on_event ? {key_value[cnt], cnt} : 0;

key_jitter filter(
	.clock(clock),
	.reset(reset),
	.key_raw(~key_down),
	.key_valid(key_valid),
	.key_value(key_value)
);

endmodule
