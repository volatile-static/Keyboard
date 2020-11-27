module divider#(parameter n)(input clk_in, output clk_out);
	reg[31:0] counter;
	reg out;
	assign clk_out = out;
	
	always @ (posedge clk_in)
		if (counter >= n)  // n*2分频
		  begin
			 counter <= 0;
			 out <= ~out;
		  end
		else
			counter <= counter + 1;
endmodule
