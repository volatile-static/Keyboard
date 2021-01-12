//****************************************Copyright (c)***********************************//
//技术支持：www.openedv.com
//淘宝店铺：http://openedv.taobao.com 
//关注微信公众平台微信号："正点原子"，免费获取FPGA & STM32资料。
//版权所有，盗版必究。
//Copyright(C) 正点原子 2018-2028
//All rights reserved	                               
//----------------------------------------------------------------------------------------
// File name:           key_debounce
// Last modified Date:  2018/4/24 9:56:36
// Last Version:        V1.1
// Descriptions:        按键消抖
//----------------------------------------------------------------------------------------
// Created by:          正点原子
// Created date:        2018/3/29 10:55:56
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
// Modified by:		    正点原子
// Modified date:	    2018/4/24 9:56:36
// Version:			    V1.1
// Descriptions:	    通过计数消除机械按键抖动
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

module key_debounce(
    input            sys_clk,          //外部50M时钟
    input            sys_rst_n,        //外部复位信号，低有效
    
    input            key,              //外部按键输入
    output reg       key_flag,         //按键数据有效信号
	output reg       key_value         //按键消抖后的数据  
    );

//reg define    
reg [15:0] delay_cnt;
reg [6:0]  latch_cnt;
reg        key_reg;

//*****************************************************
//**                    main code
//*****************************************************
always @(posedge sys_clk or negedge sys_rst_n) begin 
    if (!sys_rst_n) begin 
        key_reg   <= 1'b1;
        delay_cnt <= 16'd0;
    end
    else begin
        key_reg <= key;
        if(key_reg != key)             //一旦检测到按键状态发生变化(有按键被按下或释放)
            delay_cnt <= 16'hffff;  //给延时计数器重新装载初始值（计数时间为20ms）
        else if(key_reg == key) begin  //在按键状态稳定时，计数器递减，开始20ms倒计时
                 if(delay_cnt > 16'd0)
                     delay_cnt <= delay_cnt - 1'b1;
                 else
                     delay_cnt <= delay_cnt;
             end           
    end   
end

always @(posedge sys_clk or negedge sys_rst_n) begin 
    if (!sys_rst_n) begin 
        key_flag  <= 1'b0;
        key_value <= 1'b1;      
        latch_cnt <= 7'd0;    
    end
    else begin
        if(delay_cnt == 16'd1) begin   //当计数器递减到1时，说明按键稳定状态维持了20ms
            key_flag  <= 1'b1;         //此时消抖过程结束，给出103个时钟周期的标志信号
            key_value <= key;          //并寄存此时按键的值
            latch_cnt <= 7'd0;         
        end
        else if (latch_cnt >= 7'd102) begin
            key_flag  <= 1'b0;
            key_value <= key_value; 
            latch_cnt <= 7'd0;
        end  
        else begin
            key_flag  <= key_flag;
            key_value <= key_value; 
            latch_cnt <= latch_cnt + 1;
        end
    end   
end
    
endmodule 
