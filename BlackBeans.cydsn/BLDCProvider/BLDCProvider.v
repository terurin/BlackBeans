
//`#start header` -- edit after this line, do not edit this line
// ========================================
//
// Copyright YOUR COMPANY, THE YEAR
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF your company.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
// Generated on 06/21/2019 at 16:05
// Component: BLDCProvider
module BLDCProvider (
	output [2:0] PWMS,
	output [2:0] RESETS_N,
	input   Clock,
	input   Direction,
	input  [2:0] Halls,
	input   Kill,
	input   Pulse
);

//`#start body` -- edit after this line, do not edit this line

//`#start footer` -- edit after this line, do not edit this line

reg [5:0] resets_mask;
always @(posedge Clock or posedge Kill)begin
    if (Kill)begin 
        resets_mask<=6'b111_000;
    end else if (!Pulse)begin
        case ({Direction,Halls})
            //front
            4'b0001:resets_mask<=6'b000_011;//
            4'b0011:resets_mask<=6'b000_010;//
            4'b0010:resets_mask<=6'b000_110;//
            4'b0110:resets_mask<=6'b000_100;//
            4'b0100:resets_mask<=6'b000_101;//
            4'b0101:resets_mask<=6'b000_001;//
            //back
            4'b1001:resets_mask<=6'b000_101;//
            4'b1011:resets_mask<=6'b000_011;//
            4'b1010:resets_mask<=6'b000_010;//
            4'b1110:resets_mask<=6'b000_010;//
            4'b1100:resets_mask<=6'b000_110;//
            4'b1101:resets_mask<=6'b000_100;//
            //error
            default:resets_mask<=6'b111_000;
        endcase
    end else begin
        resets_mask<=resets_mask;//hold
    end
end

//output
assign RESETS_N=~resets_mask[5:3];
assign PWMS=resets_mask[2:0]&(Pulse?3'b111:3'b000);

//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line

//`#end` -- edit above this line, do not edit this line
