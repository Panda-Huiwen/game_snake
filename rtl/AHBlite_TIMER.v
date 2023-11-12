module AHBlite_TIMER(
    input  wire          HCLK,    
    input  wire          HRESETn, 
    input  wire          HSEL,    
    input  wire   [31:0] HADDR,   
    input  wire    [1:0] HTRANS,  
    input  wire    [2:0] HSIZE,   
    input  wire    [3:0] HPROT,   
    input  wire          HWRITE,  
    input  wire   [31:0] HWDATA,  
    input  wire          HREADY,  
    output wire          HREADYOUT, 
    output reg    [31:0] HRDATA,  
    output wire          HRESP,
	output reg           timer_interrupt   
);
assign HRESP = 1'b0;
assign HREADYOUT = 1'b1;//总是读

parameter st_idle = 1'b0,st_count = 1'b1;
parameter int_gen = 2'b00,int_con = 2'b01,int_clr = 2'b10;
reg         state;
reg [1:0]   state1;
reg timer_interrupt_next;

reg [31:0] load;
reg [31:0] value;
reg [3:0]  control;
reg        clear;

//Prescaled clk signals
wire clk16;            // HCLK/16
wire clk256;           // HCLK/256
(*gated_clock="false"*) reg timerclk;
  
//Generate prescaled clk16 ticks
prescaler Inst_precaler_clk16(
  .inclk(HCLK),
  .rst_n(HRESETn),
  .outclk(clk16)
); 
//Generate prescaled clk16 ticks
prescaler Inst_precaler_clk256(
  .inclk(clk16),
  .rst_n(HRESETn),
  .outclk(clk256)
); 
//Prescale clk based on control[3:2] 1x= 256 ; 01 = 16 ; 00 = 1;
always @(*)begin
       case (control[3:2]) 
         2'b00   : timerclk<=HCLK;
         2'b01   : timerclk<=clk16;
         default : timerclk<=clk256;
       endcase
  end

wire trans_en = HSEL & HREADY & HTRANS[1];

wire read_en;
assign read_en=HSEL&HTRANS[1]&(~HWRITE)&HREADY;

wire write_en;
assign write_en=HSEL&HTRANS[1]&(HWRITE)&HREADY;

reg [3:0]addr_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) addr_reg <= 1'b0;
  else if(trans_en) addr_reg <= HADDR[3:0];
end

reg rd_en_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) 
    rd_en_reg <= 1'b0;
  else if(read_en) 
    rd_en_reg <= 1'b1;
  else 
    rd_en_reg <= 1'b0;
end

reg wr_en_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn)
    wr_en_reg <= 1'b0;
  else if(write_en) 
    wr_en_reg <= 1'b1;
  else  
    wr_en_reg <= 1'b0;
end

always@(posedge HCLK or negedge HRESETn)begin
  if(~HRESETn)begin
    load    <= 32'h00000000;
    control <= 4'b0000;
    clear   <= 1'b0;
  end
  else if(wr_en_reg && HREADY)begin
        if(addr_reg == 4'h8)
            control <= HWDATA[3:0];
        else if(addr_reg == 4'h0)
            load    <= HWDATA;
        else if(addr_reg == 4'hc)
            clear   <= HWDATA[0];
  end
end 

always @(*)
begin
  case(addr_reg[3:0]) 
    4'h0    :  HRDATA<=load;
    4'h4    :  HRDATA<=value;
    4'h8    :  HRDATA<=control;
    default :  HRDATA<=32'h00000000;
  endcase
end
    
always@(posedge timerclk or negedge HRESETn)
begin
  if(~HRESETn)
   begin
      timer_interrupt_next<=1'b0;
      value <= 32'h0000_0000;
      state<=st_idle;
   end 
  else
      case(state)
        st_idle:
          if(control[0])
              begin
                value<= load;
                state<= st_count;
              end
        st_count:
          if(control[0])                          //if disabled timer stops
              if(value == 32'h0000_0000)
                begin
                    timer_interrupt_next =1'b1;
                    if(control[1] == 0)           //If mode=0 timer is free-running counter
                       value<=value-1;
                    else if(control[1] == 1)      //If mode=1 timer is periodic counter;
                       value<=load;
                end
              else
                begin
                    value<= value-1;
                    timer_interrupt_next<=1'b0;
                end 
      endcase
end

always @(posedge HCLK or negedge HRESETn)
begin
 if(!HRESETn) 
    begin
      timer_interrupt<=1'b0;
      state1<=int_gen;
    end
 else
  case (state1)
    int_gen:
      begin
         if(timer_interrupt_next==1'b1)
           begin
             timer_interrupt<=1'b1;   
             state1<=int_con;
           end 
         else
            state1<=int_gen;
      end 
     int_con:
             state1<=int_clr;
     int_clr:
      begin
          timer_interrupt<=1'b0;
          if(timer_interrupt_next==1'b0)
               state1<=int_gen;
          else
               state1<=int_clr;
      end
  endcase
end       
    
endmodule