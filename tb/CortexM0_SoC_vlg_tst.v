`timescale 1 ns/ 1 ns
module CortexM0_SoC_vlg_tst();

reg clk;
reg RSTn;
                        
CortexM0_SoC i1 (
    .clk(clk),
    .RSTn(RSTn)
);

initial begin                                                  
    clk = 0;
    RSTn=0;
    #100
    RSTn=1;

end  
    
always begin                                                  
    #10 clk = ~clk;
end       

endmodule
