	module clk_frep(clk, rst_n, clk_38khz);

	input clk, rst_n;
	output reg  clk_38khz;
	
	reg [9:0] count;
	
	//分频出红外模块所用的38Khz的时钟
	//也可以用占空比为1:3的38khz的时钟
	//50_000_000 / 38000 / 2 - 1
	always @ (posedge clk or negedge rst_n)
		if(!rst_n)
			begin
				count <= 0;
				clk_38khz <= 1;
			end
		else if( count == 657 )
			begin
				clk_38khz <= ~clk_38khz;
				count <= 0;
			end
		else
			count <= count + 1'd1;

	endmodule 