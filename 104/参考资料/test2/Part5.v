module Part5(
input CLOCK_50,
input[3:0] KEY,
             input [15:0] SW,
				 output[4:0] LEDR,
				 inout PS2_KBCLK,PS2_KBDAT,
//             output [0:6] HEX7,HEX6,HEX5,HEX4,HEX2,HEX1,HEX0,
				 output[35:0] GPIO
				 );
				 
wire key_down, txen;
wire[7:0] key_value;
				 
keyboard kbd(
  .PS2_CLK(PS2_KBCLK), 
  .PS2_DAT(PS2_KBDAT), 
  .CLOCK_50(CLOCK_50),
  .start_tx(txen),
  .reset(!KEY[0]),
  .tx_buf(key_value),
  .state(LEDR[4:0]));
 
matrix mat(
	.COL(GPIO[3:0]),
	.ROW(GPIO[7:4]),
	.clk_50(CLOCK_50),
	.key_down(key_down));
	
fifo fifo(
	.clk(CLOCK_50),
	.rst_n(!KEY[0]),
	.buf_in(GPIO[7:0]),
	.buf_out(key_value),
	.wr_en(txen),
	.rd_en(key_down));

endmodule
