module fifo #(DATA_WIDTH = 8, ADDR_WIDTH = 8) (
    input clk, rst_n, w_en, r_en,
    input[DATA_WIDTH-1:0] data_in,
    output full, empty,
    output reg[DATA_WIDTH-1:0] data_out
);

reg[ADDR_WIDTH   : 0] data_count;
reg[ADDR_WIDTH-1 : 0] w_ptr, r_ptr;
reg[DATA_WIDTH-1 : 0] mem[0 : 2**ADDR_WIDTH-1];

assign empty = (data_count == 'd0) ? 1 : 0;
assign full = (data_count == 2**ADDR_WIDTH) ? 1 : 0;

always @ (posedge clk or negedge rst_n) 
    if (!rst_n)
        data_count <= 'd0;
    else if (w_en && r_en)
        data_count <= data_count;
    else if (!full && w_en)
        data_count <= data_count + 1'b1;
    else if (!empty && r_en)
        data_count <= data_count - 1'b1;
    else
        data_count <= data_count;

always @ (posedge clk  or negedge rst_n) 
    if (!rst_n)
        mem[w_ptr] <= 'd0;
    else if (!full && w_en)
        mem[w_ptr] <= data_in;

always @ (posedge clk or negedge rst_n) 
    if (!rst_n)
        w_ptr <= 'd0;
    else if (w_en && !full)
        w_ptr <= w_ptr + 1'b1;
    else
        w_ptr <= w_ptr;
		
always @ (posedge clk or negedge rst_n) 
    if (!rst_n)
        r_ptr <= 'd0;
    else if (r_en && !empty)
        r_ptr <= r_ptr + 1'b1;
    else
        r_ptr <= r_ptr;

always @ (posedge clk  or negedge rst_n) 
    if (!rst_n)
        data_out <= 'd0;
    else if (!empty && r_en)
        data_out <= mem[r_ptr];

endmodule
