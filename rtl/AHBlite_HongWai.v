module AHBlite_HongWai(
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
    
    input  wire          hongwai_in,
    output wire          hongwai_interrupt /*synthesis keep*/
);

wire [7:0] data_out;/*synthesis keep*/
wire clk_38khz;

  clk_frep clk_frep_dut(
    .clk(HCLK),
    .rst_n(HRESETn),
    .clk_38khz(clk_38khz)
  );
    
 hongwai u_hongwai(
 	.clk_m0           (HCLK             ),
	.clk              (clk_38khz        ),
	.rst_n            (HRESETn          ),
	.rx               (hongwai_in       ),
	.show_data_reg_en (show_data_reg_en ),
  .show_data_out    (data_out      	  )
 );


reg [7:0]   hongwai_state;/*synthesis keep*/
wire write_en;
wire read_en;
reg addr_reg;
reg wr_en_reg;
reg rd_en_reg;

assign HRESP = 1'b0;
assign HREADYOUT = 1'b1;


assign write_en = HSEL & HTRANS[1] & HWRITE & HREADY;

assign read_en = HSEL & HTRANS[1] & (~HWRITE) & HREADY;

always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn) 
    addr_reg <= 1'b0;
  else if(HSEL & HREADY & HTRANS[1])
    addr_reg <= HADDR[2];
end

always@(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn)
    wr_en_reg <= 1'b0;
  else if(write_en)
    wr_en_reg <= 1'b1;
  else
    wr_en_reg <= 1'b0;
end

always @(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn)begin
    rd_en_reg <= 1'b0;
  end
  else if(read_en)begin
    rd_en_reg <= 1'b1;
  end
  else begin
    rd_en_reg <= 1'b0;
  end
end

always@(posedge HCLK or negedge HRESETn) begin
    if(~HRESETn)
        hongwai_state <= 8'd0;
    else if(wr_en_reg)
        hongwai_state <= HWDATA[7:0];
    else if(show_data_reg_en) 
        hongwai_state <= data_out;
    else
        hongwai_state <= hongwai_state;
  end

assign HRDATA = (rd_en_reg == 1'b1) ? {24'b0,hongwai_state} : 32'b0;

assign hongwai_interrupt = show_data_reg_en;



endmodule

