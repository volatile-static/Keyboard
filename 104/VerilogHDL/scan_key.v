module scan_key(
	input clock, reset, enabled,
	input[20:0] COL,
	output[5:0] ROW,
	output reg[103:1] key_down
);

reg[2:0] row_cnt;
reg[20:0] col_sync;

always @(posedge clock, posedge reset)
	if (reset)
		row_cnt <= 0;
	else if (row_cnt >= 5)
		row_cnt <= 0;
	else
		row_cnt <= row_cnt + 1;

assign ROW = enabled ? ~(6'h1 << row_cnt) : 6'b111111;

always @(posedge clock)
	col_sync <= COL;

always @(posedge clock, posedge reset)
	if (reset)
		key_down = 0;
	else begin: key_decode
		key_down = key_down;
		case (row_cnt)
			1: begin
				key_down[1] = col_sync[0];
				key_down[16:2] = col_sync[16:2];
			end
			2: begin
			  	key_down[37:17] = col_sync[20:0];
			end
			3: begin
			  	key_down[58:38] = col_sync[20:0];
			end
			4: begin
			  	key_down[70:59] = col_sync[11:0];
				key_down[71] = col_sync[13];
				key_down[74:72] = col_sync[19:17];
			end
			5: begin
			  	key_down[85:75] = col_sync[10:0];
				key_down[86] = col_sync[13];
				key_down[87] = col_sync[15];
				key_down[90:88] = col_sync[19:17];
			end
			0: begin
				key_down[93:91] = col_sync[2:0];
				key_down[94] = col_sync[5];
				key_down[96:95] = col_sync[10:9];
				key_down[101:97] = col_sync[17:13];
				key_down[103:102] = col_sync[20:19];
			end
			default: key_down = key_down;
		endcase
	end

endmodule
