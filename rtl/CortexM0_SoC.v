
module CortexM0_SoC (
        input  wire  clk_in,
        input  wire  RSTn,
        inout  wire  SWDIO,  
        input  wire  SWCLK,
        input  wire  hongwai_in,
       //input  wire [3:0]   col,
       //output wire [3:0]   row,
        output wire [7:0]   LED,        
		output wire	 clk_25,
        output wire  hsync,
        output wire  vsync,
        output wire [23:0]  rgb,
            // SEG4     
        output wire [3:0]   smg_bit_sel, // 数码管位选
        output wire [7:0]   smg_seg_sel,  // 数码管段选        
        
		//sut        
		input wire    ioctrl_1 ,
		input wire    ioctrl_2 ,
		input wire    ioctrl_3 ,
		input wire    ioctrl_4 
		

);

pll_a(
		.refclk		(clk_in),
		.reset		(!RSTn),
		.extlock	(),
		.clk0_out	(),
		.clk1_out	(clk),
		.clk2_out	(clk_25)	
);

//led



//------------------------------------------------------------------------------
// DEBUG IOBUF 
//------------------------------------------------------------------------------

wire SWDO;
wire SWDOEN;
wire SWDI;

assign SWDI = SWDIO;
assign SWDIO = (SWDOEN) ?  SWDO : 1'bz;

//------------------------------------------------------------------------------
// Interrupt
//------------------------------------------------------------------------------

wire [31:0] IRQ;
wire interrupt_UART;
wire interrupt_VGA;
wire key_interrupt;
wire timer_interrupt;
wire hongwai_interrupt;
wire io_interrupt;
//assign interrupt_VGA = clear_finish_flag;
assign IRQ = {27'b0,io_interrupt,timer_interrupt,hongwai_interrupt,key_interrupt,interrupt_VGA};

wire RXEV;
assign RXEV = 1'b0;

//------------------------------------------------------------------------------
// AHB
//------------------------------------------------------------------------------

wire [31:0] HADDR;
wire [ 2:0] HBURST;
wire        HMASTLOCK;
wire [ 3:0] HPROT;
wire [ 2:0] HSIZE;
wire [ 1:0] HTRANS;
wire [31:0] HWDATA;
wire        HWRITE;
wire [31:0] HRDATA;
wire        HRESP;
wire        HMASTER;
wire        HREADY;

//------------------------------------------------------------------------------
// RESET AND DEBUG
//------------------------------------------------------------------------------

wire SYSRESETREQ;
reg cpuresetn;

always @(posedge clk or negedge RSTn)begin
        if (~RSTn) cpuresetn <= 1'b0;
        else if (SYSRESETREQ) cpuresetn <= 1'b0;
        else cpuresetn <= 1'b1;
end

wire CDBGPWRUPREQ;
reg CDBGPWRUPACK;

always @(posedge clk or negedge RSTn)begin
        if (~RSTn) CDBGPWRUPACK <= 1'b0;
        else CDBGPWRUPACK <= CDBGPWRUPREQ;
end


//------------------------------------------------------------------------------
// Instantiate Cortex-M0 processor logic level
//------------------------------------------------------------------------------

cortexm0ds_logic u_logic (

        // System inputs
        .FCLK           (clk),           //FREE running clock 
        .SCLK           (clk),           //system clock
        .HCLK           (clk),           //AHB clock
        .DCLK           (clk),           //Debug clock
        .PORESETn       (RSTn),          //Power on reset
        .HRESETn        (cpuresetn),     //AHB and System reset
        .DBGRESETn      (RSTn),          //Debug Reset
        .RSTBYPASS      (1'b0),          //Reset bypass
        .SE             (1'b0),          // dummy scan enable port for synthesis

        // Power management inputs
        .SLEEPHOLDREQn  (1'b1),          // Sleep extension request from PMU
        .WICENREQ       (1'b0),          // WIC enable request from PMU
        .CDBGPWRUPACK   (CDBGPWRUPACK),  // Debug Power Up ACK from PMU

        // Power management outputs
        .CDBGPWRUPREQ   (CDBGPWRUPREQ),
        .SYSRESETREQ    (SYSRESETREQ),

        // System bus
        .HADDR          (HADDR[31:0]),
        .HTRANS         (HTRANS[1:0]),
        .HSIZE          (HSIZE[2:0]),
        .HBURST         (HBURST[2:0]),
        .HPROT          (HPROT[3:0]),
        .HMASTER        (HMASTER),
        .HMASTLOCK      (HMASTLOCK),
        .HWRITE         (HWRITE),
        .HWDATA         (HWDATA[31:0]),
        .HRDATA         (HRDATA[31:0]),
        .HREADY         (HREADY),
        .HRESP          (HRESP),

        // Interrupts
        .IRQ            (IRQ),          //Interrupt
        .NMI            (1'b0),         //Watch dog interrupt
        .IRQLATENCY     (8'h0),
        .ECOREVNUM      (28'h0),

        // Systick
        .STCLKEN        (1'b0),
        .STCALIB        (26'h0),

        // Debug - JTAG or Serial wire
        // Inputs
        .nTRST          (1'b1),
        .SWDITMS        (SWDI),
        .SWCLKTCK       (SWCLK),
        .TDI            (1'b0),
        // Outputs
        .SWDO           (SWDO),
        .SWDOEN         (SWDOEN),

        .DBGRESTART     (1'b0),

        // Event communication
        .RXEV           (RXEV),         // Generate event when a DMA operation completed.
        .EDBGRQ         (1'b0)          // multi-core synchronous halt request
);

//------------------------------------------------------------------------------
// AHBlite Interconncet
//------------------------------------------------------------------------------

wire            HSEL_P0;
wire    [31:0]  HADDR_P0;
wire    [2:0]   HBURST_P0;
wire            HMASTLOCK_P0;
wire    [3:0]   HPROT_P0;
wire    [2:0]   HSIZE_P0;
wire    [1:0]   HTRANS_P0;
wire    [31:0]  HWDATA_P0;
wire            HWRITE_P0;
wire            HREADY_P0;
wire            HREADYOUT_P0;
wire    [31:0]  HRDATA_P0;
wire            HRESP_P0;

wire            HSEL_P1;
wire    [31:0]  HADDR_P1;
wire    [2:0]   HBURST_P1;
wire            HMASTLOCK_P1;
wire    [3:0]   HPROT_P1;
wire    [2:0]   HSIZE_P1;
wire    [1:0]   HTRANS_P1;
wire    [31:0]  HWDATA_P1;
wire            HWRITE_P1;
wire            HREADY_P1;
wire            HREADYOUT_P1;
wire    [31:0]  HRDATA_P1;
wire            HRESP_P1;

wire            HSEL_P2;
wire    [31:0]  HADDR_P2;
wire    [2:0]   HBURST_P2;
wire            HMASTLOCK_P2;
wire    [3:0]   HPROT_P2;
wire    [2:0]   HSIZE_P2;
wire    [1:0]   HTRANS_P2;
wire    [31:0]  HWDATA_P2;
wire            HWRITE_P2;
wire            HREADY_P2;
wire            HREADYOUT_P2;
wire    [31:0]  HRDATA_P2;
wire            HRESP_P2;

wire            HSEL_P3;
wire    [31:0]  HADDR_P3;
wire    [2:0]   HBURST_P3;
wire            HMASTLOCK_P3;
wire    [3:0]   HPROT_P3;
wire    [2:0]   HSIZE_P3;
wire    [1:0]   HTRANS_P3;
wire    [31:0]  HWDATA_P3;
wire            HWRITE_P3;
wire            HREADY_P3;
wire            HREADYOUT_P3;
wire    [31:0]  HRDATA_P3;
wire            HRESP_P3;

wire            HSEL_P4;
wire    [31:0]  HADDR_P4;
wire    [2:0]   HBURST_P4;
wire            HMASTLOCK_P4;
wire    [3:0]   HPROT_P4;
wire    [2:0]   HSIZE_P4;
wire    [1:0]   HTRANS_P4;
wire    [31:0]  HWDATA_P4;
wire            HWRITE_P4;
wire            HREADY_P4;
wire            HREADYOUT_P4;
wire    [31:0]  HRDATA_P4;
wire            HRESP_P4;

wire            HSEL_P5;
wire    [31:0]  HADDR_P5;
wire    [2:0]   HBURST_P5;
wire            HMASTLOCK_P5;
wire    [3:0]   HPROT_P5;
wire    [2:0]   HSIZE_P5;
wire    [1:0]   HTRANS_P5;
wire    [31:0]  HWDATA_P5;
wire            HWRITE_P5;
wire            HREADY_P5;
wire            HREADYOUT_P5;
wire    [31:0]  HRDATA_P5;
wire            HRESP_P5;
                        
wire            HSEL_P6;       
wire    [31:0]  HADDR_P6;      
wire    [2:0]   HBURST_P6;     
wire            HMASTLOCK_P6;  
wire    [3:0]   HPROT_P6;      
wire    [2:0]   HSIZE_P6;      
wire    [1:0]   HTRANS_P6;     
wire    [31:0]  HWDATA_P6;     
wire            HWRITE_P6;     
wire            HREADY_P6;     
wire            HREADYOUT_P6;  
wire    [31:0]  HRDATA_P6;     
wire            HRESP_P6;      

wire            HSEL_P7;       
wire    [31:0]  HADDR_P7;      
wire    [2:0]   HBURST_P7;     
wire            HMASTLOCK_P7;  
wire    [3:0]   HPROT_P7;      
wire    [2:0]   HSIZE_P7;      
wire    [1:0]   HTRANS_P7;     
wire    [31:0]  HWDATA_P7;     
wire            HWRITE_P7;     
wire            HREADY_P7;     
wire            HREADYOUT_P7;  
wire    [31:0]  HRDATA_P7;     
wire            HRESP_P7;      

wire            HSEL_P8;       
wire    [31:0]  HADDR_P8;      
wire    [2:0]   HBURST_P8;     
wire            HMASTLOCK_P8;  
wire    [3:0]   HPROT_P8;      
wire    [2:0]   HSIZE_P8;      
wire    [1:0]   HTRANS_P8;     
wire    [31:0]  HWDATA_P8;     
wire            HWRITE_P8;     
wire            HREADY_P8;     
wire            HREADYOUT_P8;  
wire    [31:0]  HRDATA_P8;     
wire            HRESP_P8;      
   
//wire            HSEL_P9;       
//wire    [31:0]  HADDR_P9;      
//wire    [2:0]   HBURST_P9;     
//wire            HMASTLOCK_P9;  
//wire    [3:0]   HPROT_P9;      
//wire    [2:0]   HSIZE_P9;      
//wire    [1:0]   HTRANS_P9;     
//wire    [31:0]  HWDATA_P9;     
//wire            HWRITE_P9;     
//wire            HREADY_P9;     
//wire            HREADYOUT_P9;  
//wire    [31:0]  HRDATA_P9;     
//wire            HRESP_P9;      

//wire            HSEL_P10;      
//wire    [31:0]  HADDR_P10;     
//wire    [2:0]   HBURST_P10;    
//wire            HMASTLOCK_P10; 
//wire    [3:0]   HPROT_P10;     
//wire    [2:0]   HSIZE_P10;     
//wire    [1:0]   HTRANS_P10;    
//wire    [31:0]  HWDATA_P10;    
//wire            HWRITE_P10;    
//wire            HREADY_P10;    
//wire            HREADYOUT_P10; 
//wire    [31:0]  HRDATA_P10;    
//wire            HRESP_P10;     
    
//wire            HSEL_P11;      
//wire    [31:0]  HADDR_P11;     
//wire    [2:0]   HBURST_P11;    
//wire            HMASTLOCK_P11; 
//wire    [3:0]   HPROT_P11;     
//wire    [2:0]   HSIZE_P11;     
//wire    [1:0]   HTRANS_P11;    
//wire    [31:0]  HWDATA_P11;    
//wire            HWRITE_P11;    
//wire            HREADY_P11;    
//wire            HREADYOUT_P11; 
//wire    [31:0]  HRDATA_P11;    
//wire            HRESP_P11;     
     
//wire            HSEL_P12;      
//wire    [31:0]  HADDR_P12;     
//wire    [2:0]   HBURST_P12;    
//wire            HMASTLOCK_P12; 
//wire    [3:0]   HPROT_P12;     
//wire    [2:0]   HSIZE_P12;     
//wire    [1:0]   HTRANS_P12;    
//wire    [31:0]  HWDATA_P12;    
//wire            HWRITE_P12;    
//wire            HREADY_P12;    
//wire            HREADYOUT_P12; 
//wire    [31:0]  HRDATA_P12;    
//wire            HRESP_P12;     
   
//wire            HSEL_P13;      
//wire    [31:0]  HADDR_P13;     
//wire    [2:0]   HBURST_P13;    
//wire            HMASTLOCK_P13; 
//wire    [3:0]   HPROT_P13;     
//wire    [2:0]   HSIZE_P13;     
//wire    [1:0]   HTRANS_P13;    
//wire    [31:0]  HWDATA_P13;    
//wire            HWRITE_P13;    
//wire            HREADY_P13;    
//wire            HREADYOUT_P13; 
//wire    [31:0]  HRDATA_P13;    
//wire            HRESP_P13;         

AHBlite_Interconnect Interconncet(
        .HCLK           (clk),
        .HRESETn        (cpuresetn),

        // CORE SIDE
        .HADDR          (HADDR),
        .HTRANS         (HTRANS),
        .HSIZE          (HSIZE),
        .HBURST         (HBURST),
        .HPROT          (HPROT),
        .HMASTLOCK      (HMASTLOCK),
        .HWRITE         (HWRITE),
        .HWDATA         (HWDATA),
        .HRDATA         (HRDATA),
        .HREADY         (HREADY),
        .HRESP          (HRESP),

        // P0
        .HSEL_P0        (HSEL_P0),
        .HADDR_P0       (HADDR_P0),
        .HBURST_P0      (HBURST_P0),
        .HMASTLOCK_P0   (HMASTLOCK_P0),
        .HPROT_P0       (HPROT_P0),
        .HSIZE_P0       (HSIZE_P0),
        .HTRANS_P0      (HTRANS_P0),
        .HWDATA_P0      (HWDATA_P0),
        .HWRITE_P0      (HWRITE_P0),
        .HREADY_P0      (HREADY_P0),
        .HREADYOUT_P0   (HREADYOUT_P0),
        .HRDATA_P0      (HRDATA_P0),
        .HRESP_P0       (HRESP_P0),

        // P1
        .HSEL_P1        (HSEL_P1),
        .HADDR_P1       (HADDR_P1),
        .HBURST_P1      (HBURST_P1),
        .HMASTLOCK_P1   (HMASTLOCK_P1),
        .HPROT_P1       (HPROT_P1),
        .HSIZE_P1       (HSIZE_P1),
        .HTRANS_P1      (HTRANS_P1),
        .HWDATA_P1      (HWDATA_P1),
        .HWRITE_P1      (HWRITE_P1),
        .HREADY_P1      (HREADY_P1),
        .HREADYOUT_P1   (HREADYOUT_P1),
        .HRDATA_P1      (HRDATA_P1),
        .HRESP_P1       (HRESP_P1),

        // P2
        .HSEL_P2        (HSEL_P2),
        .HADDR_P2       (HADDR_P2),
        .HBURST_P2      (HBURST_P2),
        .HMASTLOCK_P2   (HMASTLOCK_P2),
        .HPROT_P2       (HPROT_P2),
        .HSIZE_P2       (HSIZE_P2),
        .HTRANS_P2      (HTRANS_P2),
        .HWDATA_P2      (HWDATA_P2),
        .HWRITE_P2      (HWRITE_P2),
        .HREADY_P2      (HREADY_P2),
        .HREADYOUT_P2   (HREADYOUT_P2),
        .HRDATA_P2      (HRDATA_P2),
        .HRESP_P2       (HRESP_P2),

        // P3
        .HSEL_P3        (HSEL_P3),
        .HADDR_P3       (HADDR_P3),
        .HBURST_P3      (HBURST_P3),
        .HMASTLOCK_P3   (HMASTLOCK_P3),
        .HPROT_P3       (HPROT_P3),
        .HSIZE_P3       (HSIZE_P3),
        .HTRANS_P3      (HTRANS_P3),
        .HWDATA_P3      (HWDATA_P3),
        .HWRITE_P3      (HWRITE_P3),
        .HREADY_P3      (HREADY_P3),
        .HREADYOUT_P3   (HREADYOUT_P3),
        .HRDATA_P3      (HRDATA_P3),
        .HRESP_P3       (HRESP_P3),
        
        .HSEL_P4        (HSEL_P4),
        .HADDR_P4       (HADDR_P4),
        .HBURST_P4      (HBURST_P4),
        .HMASTLOCK_P4   (HMASTLOCK_P4),
        .HPROT_P4       (HPROT_P4),
        .HSIZE_P4       (HSIZE_P4),
        .HTRANS_P4      (HTRANS_P4),
        .HWDATA_P4      (HWDATA_P4),
        .HWRITE_P4      (HWRITE_P4),
        .HREADY_P4      (HREADY_P4),
        .HREADYOUT_P4   (HREADYOUT_P4),
        .HRDATA_P4      (HRDATA_P4),
        .HRESP_P4       (HRESP_P4),
        
        .HSEL_P5        (HSEL_P5),
        .HADDR_P5       (HADDR_P5),
        .HBURST_P5      (HBURST_P5),
        .HMASTLOCK_P5   (HMASTLOCK_P5),
        .HPROT_P5       (HPROT_P5),
        .HSIZE_P5       (HSIZE_P5),
        .HTRANS_P5      (HTRANS_P5),
        .HWDATA_P5      (HWDATA_P5),
        .HWRITE_P5      (HWRITE_P5),
        .HREADY_P5      (HREADY_P5),
        .HREADYOUT_P5   (HREADYOUT_P5),
        .HRDATA_P5      (HRDATA_P5),
        .HRESP_P5       (HRESP_P5),
        
        .HSEL_P6       (HSEL_P6       ),
    	.HADDR_P6      (HADDR_P6      ),
    	.HBURST_P6     (HBURST_P6     ),
    	.HMASTLOCK_P6  (HMASTLOCK_P6  ),
    	.HPROT_P6      (HPROT_P6      ),
    	.HSIZE_P6      (HSIZE_P6      ),
    	.HTRANS_P6     (HTRANS_P6     ),
    	.HWDATA_P6     (HWDATA_P6     ),
    	.HWRITE_P6     (HWRITE_P6     ),
    	.HREADY_P6     (HREADY_P6     ),
    	.HREADYOUT_P6  (HREADYOUT_P6  ),
    	.HRDATA_P6     (HRDATA_P6     ),
    	.HRESP_P6      (HRESP_P6      ),
        
    	.HSEL_P7       (HSEL_P7       ),
    	.HADDR_P7      (HADDR_P7      ),
    	.HBURST_P7     (HBURST_P7     ),
    	.HMASTLOCK_P7  (HMASTLOCK_P7  ),
    	.HPROT_P7      (HPROT_P7      ),
    	.HSIZE_P7      (HSIZE_P7      ),
    	.HTRANS_P7     (HTRANS_P7     ),
    	.HWDATA_P7     (HWDATA_P7     ),
    	.HWRITE_P7     (HWRITE_P7     ),
    	.HREADY_P7     (HREADY_P7     ),
    	.HREADYOUT_P7  (HREADYOUT_P7  ),
    	.HRDATA_P7     (HRDATA_P7     ),
    	.HRESP_P7      (HRESP_P7      ),
        
    	.HSEL_P8       (HSEL_P8       ),
    	.HADDR_P8      (HADDR_P8      ),
    	.HBURST_P8     (HBURST_P8     ),
    	.HMASTLOCK_P8  (HMASTLOCK_P8  ),
    	.HPROT_P8      (HPROT_P8      ),
    	.HSIZE_P8      (HSIZE_P8      ),
    	.HTRANS_P8     (HTRANS_P8     ),
    	.HWDATA_P8     (HWDATA_P8     ),
    	.HWRITE_P8     (HWRITE_P8     ),
    	.HREADY_P8     (HREADY_P8     ),
    	.HREADYOUT_P8  (HREADYOUT_P8  ),
    	.HRDATA_P8     (HRDATA_P8     ),
    	.HRESP_P8      (HRESP_P8      )
        
//    	.HSEL_P9       (HSEL_P9       ),
//    	.HADDR_P9      (HADDR_P9      ),
//    	.HBURST_P9     (HBURST_P9     ),
//    	.HMASTLOCK_P9  (HMASTLOCK_P9  ),
//    	.HPROT_P9      (HPROT_P9      ),
//    	.HSIZE_P9      (HSIZE_P9      ),
//    	.HTRANS_P9     (HTRANS_P9     ),
//    	.HWDATA_P9     (HWDATA_P9     ),
//    	.HWRITE_P9     (HWRITE_P9     ),
//    	.HREADY_P9     (HREADY_P9     ),
//    	.HREADYOUT_P9  (HREADYOUT_P9  ),
//    	.HRDATA_P9     (HRDATA_P9     ),
//    	.HRESP_P9      (HRESP_P9      ),
        
//    	.HSEL_P10      (HSEL_P10      ),
//    	.HADDR_P10     (HADDR_P10     ),
//    	.HBURST_P10    (HBURST_P10    ),
//    	.HMASTLOCK_P10 (HMASTLOCK_P10 ),
//    	.HPROT_P10     (HPROT_P10     ),
//    	.HSIZE_P10     (HSIZE_P10     ),
//    	.HTRANS_P10    (HTRANS_P10    ),
//    	.HWDATA_P10    (HWDATA_P10    ),
//    	.HWRITE_P10    (HWRITE_P10    ),
//    	.HREADY_P10    (HREADY_P10    ),
//    	.HREADYOUT_P10 (HREADYOUT_P10 ),
//    	.HRDATA_P10    (HRDATA_P10    ),
//    	.HRESP_P10     (HRESP_P10     ),
        
//    	.HSEL_P11      (HSEL_P11      ),
//    	.HADDR_P11     (HADDR_P11     ),
//    	.HBURST_P11    (HBURST_P11    ),
//    	.HMASTLOCK_P11 (HMASTLOCK_P11 ),
//    	.HPROT_P11     (HPROT_P11     ),
//    	.HSIZE_P11     (HSIZE_P11     ),
//    	.HTRANS_P11    (HTRANS_P11    ),
//    	.HWDATA_P11    (HWDATA_P11    ),
//    	.HWRITE_P11    (HWRITE_P11    ),
//    	.HREADY_P11    (HREADY_P11    ),
//    	.HREADYOUT_P11 (HREADYOUT_P11 ),
//    	.HRDATA_P11    (HRDATA_P11    ),
//    	.HRESP_P11     (HRESP_P11     ),
        
//    	.HSEL_P12      (HSEL_P12      ),
//    	.HADDR_P12     (HADDR_P12     ),
//    	.HBURST_P12    (HBURST_P12    ),
//    	.HMASTLOCK_P12 (HMASTLOCK_P12 ),
//    	.HPROT_P12     (HPROT_P12     ),
//    	.HSIZE_P12     (HSIZE_P12     ),
//    	.HTRANS_P12    (HTRANS_P12    ),
//    	.HWDATA_P12    (HWDATA_P12    ),
//    	.HWRITE_P12    (HWRITE_P12    ),
//    	.HREADY_P12    (HREADY_P12    ),
//    	.HREADYOUT_P12 (HREADYOUT_P12 ),
//    	.HRDATA_P12    (HRDATA_P12    ),
//    	.HRESP_P12     (HRESP_P12     ),
        
//    	.HSEL_P13      (HSEL_P13      ),
//    	.HADDR_P13     (HADDR_P13     ),
//    	.HBURST_P13    (HBURST_P13    ),
//    	.HMASTLOCK_P13 (HMASTLOCK_P13 ),
//    	.HPROT_P13     (HPROT_P13     ),
//    	.HSIZE_P13     (HSIZE_P13     ),
//    	.HTRANS_P13    (HTRANS_P13    ),
//    	.HWDATA_P13    (HWDATA_P13    ),
//    	.HWRITE_P13    (HWRITE_P13    ),
//    	.HREADY_P13    (HREADY_P13    ),
//    	.HREADYOUT_P13 (HREADYOUT_P13 ),
//    	.HRDATA_P13    (HRDATA_P13    ),
//    	.HRESP_P13     (HRESP_P13     )
        
        
);

//------------------------------------------------------------------------------
// AHB RAMCODE
//------------------------------------------------------------------------------

wire [31:0] RAMCODE_RDATA,RAMCODE_WDATA;
wire [11:0] RAMCODE_WADDR;
wire [11:0] RAMCODE_RADDR;
wire [3:0]  RAMCODE_WRITE;

AHBlite_Block_RAM RAMCODE_Interface(
        /* Connect to Interconnect Port 0 */
        .HCLK           (clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P0),
        .HADDR          (HADDR_P0),
        .HPROT          (HPROT_P0),
        .HSIZE          (HSIZE_P0),
        .HTRANS         (HTRANS_P0),
        .HWDATA         (HWDATA_P0),
        .HWRITE         (HWRITE_P0),
        .HRDATA         (HRDATA_P0),
        .HREADY         (HREADY_P0),
        .HREADYOUT      (HREADYOUT_P0),
        .HRESP          (HRESP_P0),
        .BRAM_WRADDR    (RAMCODE_WADDR),
        .BRAM_RDADDR    (RAMCODE_RADDR),
        .BRAM_RDATA     (RAMCODE_RDATA),
        .BRAM_WDATA     (RAMCODE_WDATA),
        .BRAM_WRITE     (RAMCODE_WRITE)
        /**********************************/
);

//------------------------------------------------------------------------------
// AHB WaterLight
//------------------------------------------------------------------------------

wire [7:0] WaterLight_mode;
wire [31:0] WaterLight_speed;

AHBlite_WaterLight WaterLight_Interface(
        /* Connect to Interconnect Port 2 */
        .HCLK                   (clk),
        .HRESETn                (cpuresetn),
        .HSEL                   (HSEL_P2       ),
        .HADDR                  (HADDR_P2      ),
        .HPROT                  (HPROT_P2      ),
        .HSIZE                  (HSIZE_P2      ),
        .HTRANS                 (HTRANS_P2     ),
        .HWDATA                 (HWDATA_P2     ),
        .HWRITE                 (HWRITE_P2     ),
        .HRDATA                 (HRDATA_P2     ),
        .HREADY                 (HREADY_P2     ),
        .HREADYOUT              (HREADYOUT_P2  ),
        .HRESP                  (HRESP_P2      ),
        .WaterLight_mode        (WaterLight_mode),
        .WaterLight_speed       (WaterLight_speed)
        /**********************************/ 
);

//------------------------------------------------------------------------------
// AHB RAMDATA
//------------------------------------------------------------------------------

wire [31:0] RAMDATA_RDATA;
wire [31:0] RAMDATA_WDATA;
wire [12:0] RAMDATA_WADDR;
wire [12:0] RAMDATA_RADDR;
wire [3:0]  RAMDATA_WRITE;


AHBlite_Block_RAM 
#(
    .ADDR_WIDTH  (13)
)
RAMDATA_Interface
(
        /* Connect to Interconnect Port 1 */
        .HCLK           (clk),
        .HRESETn        (cpuresetn),
        .HSEL           (HSEL_P1          ),
        .HADDR          (HADDR_P1         ),
        .HPROT          (HPROT_P1         ),
        .HSIZE          (HSIZE_P1         ),
        .HTRANS         (HTRANS_P1        ),
        .HWDATA         (HWDATA_P1        ),
        .HWRITE         (HWRITE_P1        ),
        .HRDATA         (HRDATA_P1        ),
        .HREADY         (HREADY_P1        ),
        .HREADYOUT      (HREADYOUT_P1     ),
        .HRESP          (HRESP_P1         ),
        .BRAM_WRADDR    (RAMDATA_WADDR),
        .BRAM_RDADDR    (RAMDATA_RADDR),
        .BRAM_WDATA     (RAMDATA_WDATA),
        .BRAM_RDATA     (RAMDATA_RDATA),
        .BRAM_WRITE     (RAMDATA_WRITE)
        /**********************************/
);

///////////////////////
//VGA
///////////////////////

  AHBVGA VGA_Interface(
  	.HCLK      (clk      	 ),
    .HCLKEN    (1'b1         ),
    .HRESETn   (cpuresetn    ),
    .HADDR     (HADDR_P3     ),
    .HWDATA    (HWDATA_P3    ),
    .HREADY    (HREADY_P3    ),
    .HWRITE    (HWRITE_P3    ),
    .HTRANS    (HTRANS_P3    ),
    .HSEL      (HSEL_P3      ),
    .HRDATA    (HRDATA_P3    ),
    .HREADYOUT (HREADYOUT_P3 ),
    .HRESP     (HRESP_P3     ),
    .clear_finish_flag(interrupt_VGA),
    .hsync     (hsync     	 ),
    .vsync     (vsync     	 ),
    .rgb       (rgb       	 )
  );
  
wire [15:0]key_data;
wire  key_clear;
AHBlite_keyboard keyboard_interface(
	.HCLK (clk),
	.HRESETn (cpuresetn),
	.HSEL (HSEL_P4),
	.HADDR (HADDR_P4),
	.HBURST (HBURST_P4),
	.HMASTLOCK (HMASTLOCK_P4),
	.HPROT (HPROT_P4),
	.HSIZE (HSIZE_P4),
	.HTRANS (HTRANS_P4),
	.HWDATA (HWDATA_P4),
	.HWRITE (HWRITE_P4),
	.HRDATA (HRDATA_P4),
	.HREADY (HREADY_P4),
	.HREADYOUT (HREADYOUT_P4),
	.HRESP (HRESP_P4),
	.key_data (key_data),
	.key_clear (key_clear)
);

AHBlite_HongWai HongWai_interface(
  .HCLK              (clk              ),
  .HRESETn           (cpuresetn         ),
  .HSEL              (HSEL_P5           ),
  .HADDR             (HADDR_P5          ),
  .HTRANS            (HTRANS_P5         ),
  .HSIZE             (HSIZE_P5          ),
  .HPROT             (HPROT_P5          ),
  .HWRITE            (HWRITE_P5         ),
  .HWDATA            (HWDATA_P5         ),
  .HREADY            (HREADY_P5         ),
  .HREADYOUT         (HREADYOUT_P5      ),
  .HRDATA            (HRDATA_P5         ),
  .HRESP             (HRESP_P5          ),
  .hongwai_in        (hongwai_in        ),
  .hongwai_interrupt (hongwai_interrupt )
);

Keyboard keyboard(
        .clk(clk)
        ,.rstn(cpuresetn)
        ,.key_clear(key_clear)
        ,.col(col)
        ,.row(row)
        // ,.key(key)
        ,.key_interrupt(key_interrupt)
        ,.key_data(key_data)
);
//assign key_interrupt = 0;
AHB_SEG4 AHB_SEG4_Interface(
    .HCLK          (clk          	  ),
    .HRESETn       (cpuresetn         ),
	.HSEL          (HSEL_P6           ),
	.HADDR         (HADDR_P6          ),
	.HTRANS        (HTRANS_P6         ),
	.HSIZE         (HSIZE_P6          ),
	.HPROT         (HPROT_P6          ),
	.HWRITE        (HWRITE_P6         ),
	.HWDATA        (HWDATA_P6         ),
	.HREADY        (HREADY_P6         ),
	.HREADYOUT     (HREADYOUT_P6      ),
	.HRDATA        (HRDATA_P6         ),
	.HRESP         (HRESP_P6          ),
    .smg_bit_sel_o (smg_bit_sel 	  ),
    .smg_seg_sel_o (smg_seg_sel       )
);

//------------------------------------------------------------------------------
// AHB TIMER
//------------------------------------------------------------------------------
AHBlite_TIMER TIMER_Interface(
        .HCLK			(clk	       ),    
        .HRESETn		(cpuresetn     ), 
        .HSEL			(HSEL_P7       ),    
        .HADDR			(HADDR_P7      ),   
        .HTRANS			(HTRANS_P7     ),  
        .HSIZE			(HSIZE_P7      ),   
        .HPROT			(HPROT_P7      ),   
        .HWRITE			(HWRITE_P7     ),  
        .HWDATA			(HWDATA_P7     ),  
        .HREADY			(HREADY_P7     ),  
        .HREADYOUT		(HREADYOUT_P7  ), 
        .HRDATA			(HRDATA_P7     ),  
        .HRESP			(HRESP_P7      ),
	    .timer_interrupt(timer_interrupt)
);  

//------------------------------------------------------------------------------
// RAM
//------------------------------------------------------------------------------
 
Block_RAM RAM_CODE(
        .clka           (clk),
        .addra          (RAMCODE_WADDR),
        .addrb          (RAMCODE_RADDR),
        .dina           (RAMCODE_WDATA),
        .doutb          (RAMCODE_RDATA),
        .wea            (RAMCODE_WRITE)
);

Block_RAM
#(
    .ADDR_WIDTH  (13)
)
 RAM_DATA(
        .clka           (clk),
        .addra          (RAMDATA_WADDR),
        .addrb          (RAMDATA_RADDR),
        .dina           (RAMDATA_WDATA),
        .doutb          (RAMDATA_RDATA),
        .wea            (RAMDATA_WRITE)
);

//------------------------------------------------------------------------------
// WaterLight
//------------------------------------------------------------------------------

WaterLight WaterLight(
        .WaterLight_mode(WaterLight_mode),
        .WaterLight_speed(WaterLight_speed),
        .clk(clk),
        .RSTn(cpuresetn),
        //.LEDclk(LEDclk),
        .LED(LED)
);        


//
 AHB_sut( 


 	.HCLK	(clk),    
 	.HRESETn	(cpuresetn), 

 	.HSEL	(HSEL_P8),    

 	.HADDR	(HADDR_P8),  
 	.HWDATA	(HWDATA_P8), 
 	.HRDATA	(HRDATA_P8), 
            
	.HTRANS	(HTRANS_P8),  
 	.HSIZE	(HSIZE_P8),   
 	.HPROT	(HPROT_P8),   
 	.HWRITE	(HWRITE_P8),  
 	.HREADY	(HREADY_P8),  

	.HREADYOUT	(HREADYOUT_P8), 
 	.HRESP		(HRESP_P8),


    //------su-03t Ports-------
     .ioctrl_1     /* synthesis keep */    (ioctrl_1),
     .ioctrl_2      /* synthesis keep */   (ioctrl_2),
     .ioctrl_3       /* synthesis keep */  (ioctrl_3),    
     .ioctrl_4      /* synthesis keep */   (ioctrl_4),  

	
	
    //--------Interrupt outputs---------    
     . io_interrupt   (io_interrupt)/* synthesis keep */
);




endmodule
