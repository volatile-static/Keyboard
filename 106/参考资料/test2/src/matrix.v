module matrix(
	input clk_50,
	input[4:0] COL,
	output reg[4:0] ROW,
	output reg key_down
);

reg[2:0] col_cnt, row_cnt;
wire clock;

divider #(999) divider(clk_50, clock);

always @ (posedge clock)
    begin
        col_cnt <= col_cnt + 1;
		  if (!col_cnt)
				row_cnt <= row_cnt + 1;
			else
				row_cnt <= row_cnt;
				
			if (COL[col_cnt])
				key_down <= 1;
			else
				key_down <= 0;
				
        case(row_cnt)
            0: ROW      <= 4'b1110;
            1: ROW      <= 4'b1101;
            2: ROW      <= 4'b1011;
            3: ROW      <= 4'b0111;
            default:ROW <= 4'b1111;
        endcase
    end

endmodule
	 