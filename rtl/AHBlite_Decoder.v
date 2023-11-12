module AHBlite_Decoder
#(
    /*RAMCODE enable parameter*/
    parameter Port0_en = 1,
    /************************/

    /*WaterLight enable parameter*/
    parameter Port1_en = 1,
    /************************/

    /*RAMDATA enable parameter*/
    parameter Port2_en = 1,
    /************************/

    /*UART enable parameter*/
    parameter Port3_en = 1,
    /************************/
    
    /*KEYBOARD enable parameter*/
    parameter Port4_en = 1,
    /************************/
    
    /*HONGWAI enable parameter*/
    parameter Port5_en = 1,
    /************************/
    
    /*HONGWAI enable parameter*/
    parameter Port6_en = 1,
    /************************/
    
    /*HONGWAI enable parameter*/
    parameter Port7_en = 1,
    /************************/
    
    /*HONGWAI enable parameter*/
    parameter Port8_en = 1,
    /************************/
    
    /*HONGWAI enable parameter*/
    parameter Port9_en = 0,
    /************************/
    
    /*HONGWAI enable parameter*/
    parameter Port10_en = 0,
    /************************/
    
    /*HONGWAI enable parameter*/
    parameter Port11_en = 0,
    /************************/
    
    /*HONGWAI enable parameter*/
    parameter Port12_en = 0,
    /************************/
    
    /*HONGWAI enable parameter*/
    parameter Port13_en = 0
    /************************/
)(
    input [31:0] HADDR,

    /*RAMCODE OUTPUT SELECTION SIGNAL*/
    output wire P0_HSEL,

    /*WaterLight OUTPUT SELECTION SIGNAL*/
    output wire P1_HSEL,

    /*RAMDATA OUTPUT SELECTION SIGNAL*/
    output wire P2_HSEL,

    /*VGA OUTPUT SELECTION SIGNAL*/
    output wire P3_HSEL,
    
    /*KEYBOARD OUTPUT SELECTION SIGNAL*/
    output wire P4_HSEL, 
    
    /*HONGWAI OUTPUT SELECTION SIGNAL*/
    output wire P5_HSEL,
    
    /*HONGWAI OUTPUT SELECTION SIGNAL*/
    output wire P6_HSEL,
    
    /*HONGWAI OUTPUT SELECTION SIGNAL*/
    output wire P7_HSEL,
    
//    /*HONGWAI OUTPUT SELECTION SIGNAL*/
    output wire P8_HSEL
    
//    /*HONGWAI OUTPUT SELECTION SIGNAL*/
//    output wire P9_HSEL,
    
//    /*HONGWAI OUTPUT SELECTION SIGNAL*/
//    output wire P10_HSEL,   
    
//    /*HONGWAI OUTPUT SELECTION SIGNAL*/
//    output wire P11_HSEL,    
    
//    /*HONGWAI OUTPUT SELECTION SIGNAL*/
//    output wire P12_HSEL,    
    
//   /*HONGWAI OUTPUT SELECTION SIGNAL*/
//    output wire P13_HSEL     
           
);

//RAMCODE-----------------------------------

//0x00000000-0x0000ffff
/*Insert RAMCODE decoder code there*/
assign P0_HSEL = (HADDR[31:16] == 16'h0000) ? Port0_en : 1'b0; 
/***********************************/

//RAMDATA-----------------------------
//0X20000000-0X2000FFFF
/*Insert RAMDATA decoder code there*/
assign P1_HSEL = (HADDR[31:16] == 16'h2000) ? Port1_en : 1'b0;
/***********************************/

//PERIPHRAL-----------------------------

//0X40000000 WaterLight MODE
//0x40000004 WaterLight SPEED
/*Insert WaterLight decoder code there*/
assign P2_HSEL = (HADDR[31:4] == 28'h4000000) ? Port2_en : 1'b0;
/***********************************/


//VGA
//0x4000_0010 vga_clear
//0x4000_0014 vga_image
//0x4000_0018 vga_rd   w/r write rdaddress;read color data
/*Insert VGA decoder code there*/
assign P3_HSEL = (HADDR[31:4] == 28'h4000001) ? Port3_en : 1'b0;
/***********************************/


//KEYBOARD-----------------------------
////0x40000002 key_data/key_clear
/*Insert RAMDATA decoder code there*/
assign P4_HSEL = (HADDR[31:4] == 28'h4000002) ? Port4_en : 1'b0;

//HONGWAI-----------------------------
////0x40000003 hongwai_data/hongwai_clear
/*Insert RAMDATA decoder code there*/
assign P5_HSEL = (HADDR[31:4] == 28'h4000003) ? Port5_en : 1'b0;

//SEG-----------------------------
////0x40000004/5 
/*Insert RAMDATA decoder code there*/
assign P6_HSEL = (HADDR[31:4] == 28'h4000004 || HADDR[31:4] == 28'h4000005) ? Port6_en : 1'b0;

//TIMER1-----------------------------
////0x40000006 
/*Insert RAMDATA decoder code there*/
assign P7_HSEL = (HADDR[31:4] == 28'h4000006) ? Port7_en : 1'b0;

//SUT-----------------------------
////0x40000007 
/*Insert RAMDATA decoder code there*/
assign P8_HSEL = (HADDR[31:4] == 28'h4000007) ? Port8_en : 1'b0;

//UART-----------------------------
////0x40000008 
/*Insert RAMDATA decoder code there*/
assign P9_HSEL = (HADDR[31:4] == 28'h4000008) ? Port9_en : 1'b0;

//LCD-----------------------------
////0x40000009 
/*Insert RAMDATA decoder code there*/
assign P10_HSEL = (HADDR[31:4] == 28'h4000009) ? Port10_en : 1'b0;

//BUZZ-----------------------------
////0x40000009 
/*Insert RAMDATA decoder code there*/
assign P11_HSEL = (HADDR[31:4] == 28'h4000010) ? Port11_en : 1'b0;

//MP3-----------------------------
////0x40000009 
/*Insert RAMDATA decoder code there*/
assign P12_HSEL = (HADDR[31:4] == 28'h4000011) ? Port12_en : 1'b0;

//MP3-----------------------------
////0x40000009 
/*Insert RAMDATA decoder code there*/
assign P13_HSEL = (HADDR[31:4] == 28'h4000012) ? Port13_en : 1'b0;

endmodule