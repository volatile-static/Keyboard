module reset_controller(
	input clock, KEY_Fn,
	output reg reset = 1
);

reg[15:0] clk_cnt;
reg[31:0] reset_cnt;
reg[1:0] power_on_cnt = 0;
wire clk1 = clk_cnt == 16'hffff;

always @(posedge clock) 
	clk_cnt <= clk_cnt + 1;

always @(posedge clock)
	if (clk1 && power_on_cnt < 3)
		power_on_cnt <= power_on_cnt + 1;

always @(posedge clock)
	if (KEY_Fn)
		reset_cnt <= 0;
	else if (clk1)
		reset_cnt <= reset_cnt + 1;

always @(posedge clock)
	if (power_on_cnt < 2 || reset_cnt > 32'hDead_beef)
		reset <= 1;
	else
		reset <= 0;

endmodule
