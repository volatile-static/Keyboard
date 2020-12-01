//
// Permission:
//
//   Terasic grants permission to use and modify this code for use
//   in synthesis for all Terasic Development Boards and Altera Development 
//   Kits made by Terasic.  Other use of this code, including the selling 
//   ,duplication, or modification of any portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL/Verilog or C/C++ source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Terasic provides no warranty regarding the use 
//   or functionality of this code.
//
// --------------------------------------------------------------------
//           
//                     Terasic Technologies Inc
//                     356 Fu-Shin E. Rd Sec. 1. JhuBei City,
//                     HsinChu County, Taiwan
//                     302
//
//                     web: http://www.terasic.com/
//                     email: support@terasic.com
//
// --------------------------------------------------------------------
//
// Major Functions:	DE2_115_PS2 Mouse Controller 
//
// --------------------------------------------------------------------
//
// Revision History :
// --------------------------------------------------------------------
//   Ver  :| Author                    :| Mod. Date :| Changes Made:
//   V1.0 :| Johnny FAN,HdHuang        :| 05/16/10  :| Initial Revision
// --------------------------------------------------------------------
module ps2(
           iSTART,   //press the button for transmitting instrucions to device;
           iRST_n,   //FSM reset signal;
           iCLK_50,  //clock source;
           PS2_CLK,  //ps2_clock signal inout;
           PS2_DAT,  //ps2_data  signal inout;
           oLEFBUT,  //left button press display;
           oRIGBUT,  //right button press display;
           oMIDBUT,  //middle button press display;
           oX_MOV1,  //lower SEG of mouse displacement display for X axis.
           oX_MOV2,  //higher SEG of mouse displacement display for X axis.
           oY_MOV1,  //lower SEG of mouse displacement display for Y axis.
           oY_MOV2   //higher SEG of mouse displacement display for Y axis.
           ); 
           //interface;
//=======================================================
//  PORT declarations
//=======================================================

input iSTART;
input iRST_n;
input iCLK_50;

inout PS2_CLK;
inout PS2_DAT;

output oLEFBUT;
output oRIGBUT;
output oMIDBUT;
output [6:0] oX_MOV1;
output [6:0] oX_MOV2;
output [6:0] oY_MOV1;
output [6:0] oY_MOV2;

//instantiation
SEG7_LUT U1(.oSEG(oX_MOV1),.iDIG(x_latch[3:0]));
SEG7_LUT U2(.oSEG(oX_MOV2),.iDIG(x_latch[7:4]));
SEG7_LUT U3(.oSEG(oY_MOV1),.iDIG(y_latch[3:0]));
SEG7_LUT U4(.oSEG(oY_MOV2),.iDIG(y_latch[7:4]));
//instruction define, users can charge the instruction byte here for other purpose according to ps/2 mouse datasheet.
//the MSB is of parity check bit, that's when there are odd number of 1's with data bits, it's value is '0',otherwise it's '1' instead.

parameter enable_byte =9'b011110100;


//=======================================================
//  REG/WIRE declarations
//=======================================================
reg [1:0] cur_state,nex_state;
reg ce,de;
reg [3:0] byte_cnt,delay;
reg [5:0] ct;
reg [7:0] x_latch,y_latch,cnt;
reg [8:0] clk_div;
reg [9:0] dout_reg;
reg [32:0] shift_reg;
reg       leflatch,riglatch,midlatch;
reg       ps2_clk_in,ps2_clk_syn1,ps2_dat_in,ps2_dat_syn1;
wire      clk,ps2_dat_syn0,ps2_clk_syn0,ps2_dat_out,ps2_clk_out,flag;

//=======================================================
//  PARAMETER declarations
//=======================================================
//state define
parameter listen =2'b00,
          pullclk=2'b01,
          pulldat=2'b10,
          trans  =2'b11;
          
//=======================================================
//  Structural coding
//=======================================================          
//clk division, derive a 97.65625KHz clock from the 50MHz source;

always@(posedge iCLK_50)
	begin
		clk_div <= clk_div+1;
	end
	
assign clk = clk_div[8];
//tristate output control for PS2_DAT and PS2_CLK;
assign PS2_CLK = ce?ps2_clk_out:1'bZ;
assign PS2_DAT = de?ps2_dat_out:1'bZ;
assign ps2_clk_out = 1'b0;
assign ps2_dat_out = dout_reg[0];
assign ps2_clk_syn0 = ce?1'b1:PS2_CLK;
assign ps2_dat_syn0 = de?1'b1:PS2_DAT;
//
assign oLEFBUT = leflatch;
assign oRIGBUT = riglatch;
assign oMIDBUT = midlatch;
//multi-clock region simple synchronization
always@(posedge clk)
	begin
		ps2_clk_syn1 <= ps2_clk_syn0;
		ps2_clk_in   <= ps2_clk_syn1;
		ps2_dat_syn1 <= ps2_dat_syn0;
		ps2_dat_in   <= ps2_dat_syn1;
	end
//FSM shift
always@(*)
begin
   case(cur_state)
     listen  :begin
              if ((!iSTART) && (cnt == 8'b11111111))
                  nex_state = pullclk;
              else
                  nex_state = listen;
                         ce = 1'b0;
                         de = 1'b0;
              end
     pullclk :begin
              if (delay == 4'b1100)
                  nex_state = pulldat;
              else
                  nex_state = pullclk;
                         ce = 1'b1;
                         de = 1'b0;
              end
     pulldat :begin
                  nex_state = trans;
                         ce = 1'b1;
                         de = 1'b1;
              end
     trans   :begin
              if  (byte_cnt == 4'b1010)
                  nex_state = listen;
              else    
                  nex_state = trans;
                         ce = 1'b0;
                         de = 1'b1;
              end
     default :    nex_state = listen;
   endcase
end
//idle counter
always@(posedge clk)
begin
  if ({ps2_clk_in,ps2_dat_in} == 2'b11)
	begin
		cnt <= cnt+1;
    end
  else begin
		cnt <= 8'd0;
       end
end
//periodically reset ct; ct counts the received data length;
assign flag = (cnt == 8'hff)?1:0;
always@(posedge ps2_clk_in,posedge flag)
begin
  if (flag)
     ct <= 6'b000000;
  else
     ct <= ct+1;
end
//latch data from shift_reg;outputs is of 2's complement;
//Please treat the cnt value here with caution, otherwise wrong data will be latched.
always@(posedge clk,negedge iRST_n)
begin
   if (!iRST_n) 
   begin
      leflatch <= 1'b0;
      riglatch <= 1'b0;
      midlatch <= 1'b0;
      x_latch  <= 8'd0;
      y_latch  <= 8'd0;
   end
   else if (cnt == 8'b00011110 && (ct[5] == 1'b1 || ct[4] == 1'b1))
   begin
      leflatch <= shift_reg[1];
      riglatch <= shift_reg[2];
      midlatch <= shift_reg[3];
      x_latch  <= x_latch+shift_reg[19 : 12];
      y_latch  <= y_latch+shift_reg[30 : 23];
   end
end

//pull ps2_clk low for 100us before transmit starts;
always@(posedge clk)
begin
  if (cur_state == pullclk)
     delay <= delay+1;
  else
     delay <= 4'b0000;
end
//transmit data to ps2 device;eg. 0xF4
always@(negedge ps2_clk_in)
begin
  if (cur_state == trans)
     dout_reg <= {1'b0,dout_reg[9:1]};
  else
     dout_reg <= {enable_byte,1'b0};
end
//transmit byte length counter
always@(negedge ps2_clk_in)
begin
  if (cur_state == trans)
     byte_cnt <= byte_cnt+1;
  else
     byte_cnt <= 4'b0000;
end
//receive data from ps2 device;
always@(negedge ps2_clk_in)
begin
  if (cur_state == listen)
     shift_reg <= {ps2_dat_in,shift_reg[32:1]};
end
//FSM movement
always@(posedge clk,negedge iRST_n)
begin
  if (!iRST_n)
     cur_state <= listen;
  else
     cur_state <= nex_state;
end
endmodule


     


     


