module get_edge(
	input clock, reset, signal,
	output rising, falling
);

reg r0, r1;

assign rising  = (~r0) & r1;
assign falling = (~r1) & r0;
                                                 
//对发送使能信号uart_en延迟两个时钟周期
always @(posedge clock, posedge reset)         
    if (reset) 
		begin
        r0 <= 1'b0;                                  
        r1 <= 1'b0;
		end                                                      
    else 
		begin                                               
        r0 <= signal;                               
        r1 <= r0;                            
		end

endmodule
