//file name:       key_jitter.v
//author:           BigTree
//date:             2017.7.23
//function:        按键消抖
//log:              

module key_jitter(
		input clk,				//50MHz
		input key_n,			//低电平有效
		output reg key_valid //高电平有效
);

reg [19:0] cnt0;

always @(posedge clk)
begin
	cnt0 <= cnt0 + 1'b1;
end

reg [3:0] key_down_check;

always @(posedge clk)
begin
	if(cnt0 == 20'hfffff)
		key_down_check <= {key_down_check[2:0],~key_n};
	else
			key_down_check <= key_down_check;
end

reg key_valid_r0;

always @(posedge clk)
begin
	key_valid_r0 <= (key_down_check==4'b0011)?1'b1:1'b0;
end

reg key_valid_r1;
reg key_valid_r2;

always @(posedge clk)
begin
	key_valid_r1 <= key_valid_r0;
	key_valid_r2 <= key_valid_r1;
end

wire key_valid_pos;

assign key_valid_pos = ({key_valid_r2,key_valid_r1}==2'b01)?1'b1:1'b0;

always @(posedge clk)
begin
	key_valid <= (key_valid_pos==1'b1)?1'b1:1'b0;
end


endmodule

