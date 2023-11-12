// 1) Clock freq
// VGA runs at 25MHz. (Originally 50MHz)
// If HCLK running at 50MHz, uncomment the following line
//`define HCLK_IS_50MHZ
// The design also allows the AHB clock (HCLK) to be Nx of 
// actual AHB clock frequency (HCLKEN need to be correctly generated).
//
// 2) Color depth
//  In this design the color is 8-bit. 
//
// 3) Memory map
// The address decoding is changed.
//
module AHBVGA(
  input  wire        HCLK,
  input  wire        HRESETn,
  input  wire        HCLKEN,  // MODIFICATION : Added to allow VGA core to run at Nx HCLK freq
  input  wire [23:0] HADDR,
  input  wire [31:0] HWDATA,
  input  wire        HREADY,
  input  wire        HWRITE,
  input  wire  [1:0] HTRANS,
  input  wire        HSEL,
  
  output wire [31:0] HRDATA,
  output wire        HREADYOUT,
  output wire        HRESP,
  
  output wire        clear_finish_flag,
  output wire        hsync,
  output wire        vsync,
  output wire [23:0]  rgb  
);

  parameter  XYADDRBIT   = 4'd13;
  // Register locations
  localparam RCOLORADDR  = 4'h8;
  localparam IMAGEADDR   = 4'h4; 
  localparam CLEARADDR   = 4'h0;

`ifdef VGA_16BIT_IMAGE
  localparam PWIDTH  = 16;  // Pixel width
`else
  localparam PWIDTH  = 8;
`endif
 
  // Internal AHB signals
  reg        last_HWRITE;
  reg        last_HSEL;
  reg  [1:0] last_HTRANS;
  reg  [3:0] last_HADDR;
           
  wire  [23:0] rgb_reg; // rgb register account for delay
  wire       video_on;       // display area
  wire [9:0] pixel_x;        // current x pixel
  wire [9:0] pixel_y;        // current y pixel
  wire       p_tick;         // 25Mhz clk tick
  
  wire		   write_en;
  wire       read_en;
  reg        clear_write;    //write to console
  reg        image_write;    //write to image
  reg        rdcolor_write;
  reg  [PWIDTH+XYADDRBIT-1:0]  reg_image_wdata;      //21bit data[20:13] color data  data[12:0] xydata
  reg  [PWIDTH - 1:0]          reg_clear_wdata;
  reg  [XYADDRBIT-1:0]         rd_color_address;
  
  //wire       clear_finish_flag;       //clear all black finish ,only 1 cycle

  wire [PWIDTH-1:0]  image_rgb;      //image color
  wire [PWIDTH-1:0]  rd_rgb;

  wire       sel_clear;       
  wire       sel_image;
  wire       sle_rd_color;
  //////////////////////////////////////////////////////
  // De-pipeline AHB controls
  always @(posedge HCLK)
  if(HCLKEN & HREADY)
    begin
      last_HADDR  <= HADDR[3:0];
      last_HWRITE <= HWRITE;
      last_HSEL   <= HSEL;
      last_HTRANS <= HTRANS;
    end
    
  // Give time for the screen to refresh before writing
  assign HREADYOUT = 1'b1; 
  assign HRESP = 1'b0;  
  
  //write_en
  assign write_en = last_HWRITE & last_HSEL & last_HTRANS[1] & HREADY & HCLKEN;
  assign read_en  = (~last_HWRITE) & last_HSEL & last_HTRANS[1] & HREADY & HCLKEN;
///////////////////////////////////////////////////////
  // Counters for image clock 
  vga_sync u_vga_sync(
    .clk        (HCLK),
    .resetn     (HRESETn),
`ifdef HCLK_IS_50MHZ
    .clk_cfg    (1'b0),    // 0 : clk=50MHz, 1 : clk=25MHz 
`else    
    .clk_cfg    (1'b1),    // 0 : clk=50MHz, 1 : clk=25MHz 
`endif    
    .hsync      (hsync),
    .vsync      (vsync),
    .video_on   (video_on),
    .p_tick     (p_tick),
    .pixel_x    (pixel_x),
    .pixel_y    (pixel_y)
  );

  vga_image u_vga_image(
    .clk        (HCLK),
    .resetn     (HRESETn),
    .address    (reg_image_wdata[XYADDRBIT-1:0]),
    .video_on   (video_on),
    .pixel_x    (pixel_x),
    .pixel_y    (pixel_y),
    .clear_we   (clear_write),
    .clear_data (reg_clear_wdata),
    .image_we   (image_write),
    .image_data (reg_image_wdata[PWIDTH+XYADDRBIT-1:XYADDRBIT]),
    .rdcadress_we(rdcolor_write),
    .rdcadress   (rd_color_address),
    .clear_finish_flag   (clear_finish_flag),
    .rd_rgb     (rd_rgb),
    .image_rgb  (image_rgb)
    );

  assign sel_clear = (last_HADDR == CLEARADDR);
  assign sel_image = (last_HADDR == IMAGEADDR); 
  assign sel_rd_color = (last_HADDR == RCOLORADDR);

  // Set console write
  always @(posedge HCLK or negedge HRESETn)
  begin
    if(~HRESETn)
      clear_write <= 1'b0;
    else if(write_en & sel_clear)
      clear_write <= 1'b1;
    else
      clear_write <= 1'b0;
  end
  
  // Set image write and image write data
  always @(posedge HCLK or negedge HRESETn)
  begin
    if(~HRESETn)
      image_write <= 1'b0;
    else if(write_en & sel_image)
      image_write <= 1'b1;
    else
      image_write <= 1'b0;
  end

  // Set rd_color
  always @(posedge HCLK or negedge HRESETn)
  begin
    if(~HRESETn)
      rdcolor_write <= 1'b0;
    else if(write_en & sel_rd_color)
      rdcolor_write <= 1'b1;
    else
      rdcolor_write <= 1'b0;
  end  

  // Register write data
 always @(posedge HCLK or negedge HRESETn)
  begin
    if(~HRESETn)
      reg_image_wdata <= {21{1'b0}};
    else if(write_en & sel_image)
      reg_image_wdata <= HWDATA[PWIDTH+XYADDRBIT-1:0];
    else
      reg_image_wdata <= reg_image_wdata;
 end       

 always @(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn)begin
    reg_clear_wdata <= {PWIDTH{1'b0}};
  end
  else if(write_en & sel_clear)begin
    reg_clear_wdata <= HWDATA[PWIDTH-1:0];
  end
  else begin
    reg_clear_wdata <= reg_clear_wdata;
  end
 end

 always @(posedge HCLK or negedge HRESETn) begin
  if(~HRESETn)begin
    rd_color_address <= {XYADDRBIT{1'b0}};
  end
  else if(write_en & sel_rd_color)begin
    rd_color_address <= HWDATA[XYADDRBIT-1:0];
  end
  else begin
    rd_color_address <= rd_color_address;
  end
 end

  //assign rgb_reg = {image_rgb[7:5],2'b0,image_rgb[4:2],3'b0,image_rgb[1:0],3'b00}; 
assign rgb_reg = {image_rgb[7:5],5'b00000,image_rgb[4:2],5'b00000,image_rgb[1:0],6'd000000}; 
  // Set output
  //assign rgb = (video_on) ? rgb_reg : {PWIDTH{1'b0}};
  assign rgb = (video_on) ? rgb_reg : {24{1'b0}};
  assign HRDATA = (read_en == 1'b1 && last_HADDR == 4'd8) ? {{24{1'b0}},rd_rgb} :{32{1'b0}};

endmodule
  
  
