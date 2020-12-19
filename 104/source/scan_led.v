module scan_led(
	input clock, reset, enabled,
	input[74:0] led_matrix,
	output[5:0] LED_A,
	output reg[34:0] LED_K
);

reg[2:0] row_cnt;
wire clk_scan;

divider #(`SCAN_LED_PERIOD) (clock, clk_scan);

always @(posedge clk_scan, posedge reset)
	if (reset)
		row_cnt <= 0;
	else if (row_cnt >= 5)
		row_cnt <= 0;
	else
		row_cnt <= row_cnt + 1;

always @ *
	begin: led_encode
		LED_K = ~35'h0;
		case (row_cnt)
			1: begin
				LED_K[0] = led_matrix[0];
				LED_K[13:2] = led_matrix[12:1];
			end
			2: begin
				LED_K[13:0] = led_matrix[26:13];
			end
			3: begin
				LED_K[13:0] = led_matrix[40:27];
			end
			4: begin
				LED_K[11:0] = led_matrix[52:41];
				LED_K[13] = led_matrix[53];
			end
			5: begin
				LED_K[10:0] = led_matrix[64:54];
				LED_K[13] = led_matrix[65];
			end
			0: begin
				LED_K[2:0] = led_matrix[68:66];
				LED_K[5] = led_matrix[69];
				LED_K[13:9] = led_matrix[74:70];
			end
			default: LED_K = ~35'h0;
		endcase
	end

assign LED_A = enabled ? ~(6'h1 << row_cnt) : 6'b111111;

endmodule
