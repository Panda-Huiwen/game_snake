module AHBlite_WaterLight(
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
    output wire   [31:0] HRDATA,  
    output wire          HRESP,
    output reg     [7:0] WaterLight_mode,
    output reg     [31:0] WaterLight_speed  
);

//AHB-Lite只有两种响应（1位）：低电平反应传输正常，高电平反应传输异常
assign HRESP = 1'b0;
//HREADYOUT用于表示总线上是否传输已完成，高电平有效
//HREADY信号在Slave端是inout信号，或者在设计的时候单独引出一条HREADYOUT信号
assign HREADYOUT = 1'b1;

wire write_en;
assign write_en = HSEL & HTRANS[1] & HWRITE & HREADY;

reg addr_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) addr_reg <= 1'b0;
  else if(HSEL & HREADY & HTRANS[1]) addr_reg <= HADDR[2];
end

reg wr_en_reg;
always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) wr_en_reg <= 1'b0;
  else if(write_en) wr_en_reg <= 1'b1;
  else wr_en_reg <= 1'b0;
end

always@(posedge HCLK) begin
    if(~HRESETn) begin
        WaterLight_mode <= 8'h00;
        WaterLight_speed <= 32'h00000000;
    end else if(wr_en_reg && HREADY) begin
        if(~addr_reg)
            WaterLight_mode <= HWDATA[7:0];
        else    
            WaterLight_speed <= HWDATA;
    end
end

assign HRDATA = (addr_reg) ? WaterLight_speed : {24'b0,WaterLight_mode};

endmodule


