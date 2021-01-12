module key_jitter(
	input clock, reset, 
	input[103:1] key_raw,
	output[103:1] key_valid, key_value
);

genvar i;
generate
	for (i = 1; i < 104; i = i + 1)
		begin: filters
		  	key_debounce(
				.sys_clk(clock),
				.sys_rst_n(!reset),
				.key(key_raw[i]),
				.key_flag(key_valid[i]),
				.key_value(key_value[i])
			);
		end
endgenerate

endmodule
