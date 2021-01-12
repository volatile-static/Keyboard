module reset_controller(
	input clock, KEY_Fn,
	output reg reset = 1
);

reg[63:0] power_on_cnt = 0, reset_cnt;

always @(posedge clock) 
	if (reset)
		power_on_cnt <= power_on_cnt + 1;

always @(posedge clock)
	if (KEY_Fn)
		reset_cnt <= 0;
	else
		reset_cnt <= reset_cnt + 1;

always @(posedge clock)
	if (power_on_cnt < `POR || reset_cnt[60])
		reset <= 1;
	else
		reset <= 0;

endmodule
