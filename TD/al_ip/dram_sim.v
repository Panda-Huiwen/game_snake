// Verilog netlist created by TD v5.0.43066
// Mon Mar 20 16:16:07 2023

`timescale 1ns / 1ps
module dram  // dram.v(14)
  (
  addr,
  ba,
  cas_n,
  cke,
  clk,
  cs_n,
  dm0,
  dm1,
  dm2,
  dm3,
  ras_n,
  we_n,
  dq
  );

  input [10:0] addr;  // dram.v(19)
  input [1:0] ba;  // dram.v(20)
  input cas_n;  // dram.v(17)
  input cke;  // dram.v(27)
  input clk;  // dram.v(15)
  input cs_n;  // dram.v(22)
  input dm0;  // dram.v(23)
  input dm1;  // dram.v(24)
  input dm2;  // dram.v(25)
  input dm3;  // dram.v(26)
  input ras_n;  // dram.v(16)
  input we_n;  // dram.v(18)
  inout [31:0] dq;  // dram.v(21)


  EG_PHY_CONFIG #(
    .DONE_PERSISTN("ENABLE"),
    .INIT_PERSISTN("ENABLE"),
    .JTAG_PERSISTN("DISABLE"),
    .PROGRAMN_PERSISTN("DISABLE"))
    config_inst ();
  EG_PHY_SDRAM_2M_32 sdram (
    .addr(addr),
    .ba(ba),
    .cas_n(cas_n),
    .cke(cke),
    .clk(clk),
    .cs_n(cs_n),
    .dm0(dm0),
    .dm1(dm1),
    .dm2(dm2),
    .dm3(dm3),
    .ras_n(ras_n),
    .we_n(we_n),
    .dq(dq));  // dram.v(29)

endmodule 

