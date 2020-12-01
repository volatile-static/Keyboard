module keyboard(inout tri PS2_CLK,
                PS2_DAT,
                input CLOCK_50,
                start_tx,
                reset,
                input[7:0] tx_buf,
                output[4:0] state);
    
    localparam start = 5'b000001,
                data = 5'b000010,
              parity = 5'b000100,
                stop = 5'b001000,
               delay = 6'b010000,
                idel = 5'b100000;
    
    reg clk_out, dat_out;
    wire clk_in, dat_in;
    wire txen, rxen;
    wire clock, half_clk;

    reg[2:0] bit_cnt;
    reg[1:0] bit_state;
    reg[4:0] curr_state, next_state;
    
    divider #(24)(.clk_in(CLOCK_50), .clk_out(clock));  // 1us
    divider #(9)(.clk_in(clock), .clk_out(half_clk));  // 20us
    
    assign state = curr_state;

    ps2_bus ps2_bus(
    .PS2_CLK(PS2_CLK),
    .PS2_DAT(PS2_DAT),
    .clock(clock),
    .clk_in(clk_in),
    .clk_out(clk_out),
    .dat_in(dat_in),
    .dat_out(dat_out)
    );
    
    always @ (posedge clk_out)
        if (curr_state != data)
            bit_cnt <= 0;
        else  
            bit_cnt <= bit_cnt + 1; 

    always @ (posedge clock)
        if (reset)
            curr_state <= idel;
        else
            curr_state <= next_state;
    
    always @ *
    case (curr_state)
        idel: 
            if (start_tx)
                next_state <= start;
            else 
			    next_state <= idel;

        start: 
            if (bit_state > 2)
                next_state <= data;
            else 
			    next_state <= start;

        data: 
            if (bit_cnt == 7)
                next_state <= parity;
            else 
			    next_state <= data;

        parity: 
            if (bit_state > 2)
                next_state <= stop;
            else 
                next_state <= parity;
                
        stop: 
            if (bit_state > 2)
                next_state <= delay;
            else
                next_state <= stop;

        delay:
            next_state <= delay;

        default: 
            next_state <= idel;
    endcase

    always @ (posedge half_clk)
        begin
            if (reset | bit_state > 2)
				bit_state <= 0;
            else 
                bit_state <= bit_state + 1;
            
            if (start_tx)
                case (bit_state)
                    1: clk_out <= 0;
                    3: clk_out <= 1;
                    default: clk_out <= clk_out;
                endcase
            else clk_out <= 1;

            if (bit_state)
                dat_out <= dat_out;
            else if (start_tx)
                case (curr_state)
                    idel: dat_out <= 1;
                    start: dat_out <= 0;
                    data: dat_out <= tx_buf[bit_cnt];
                    parity: dat_out <= !(^tx_buf);
                    stop: dat_out <= 1;
                    delay: dat_out <= 1;
                    default: dat_out <= dat_out;
                endcase
            else dat_out <= 1;
        end
endmodule
