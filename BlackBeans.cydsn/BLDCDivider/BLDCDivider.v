
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
// Generated on 03/31/2019 at 04:46
// Component: BLDCDivider
module BLDCDivider (
	output [2:0] pwms,
	output [2:0] resets,
	input   clock,
	input   dir,
	input  [2:0] halls,
	input   kill,
	input   pulse
);

//`#start body` -- edit after this line, do not edit this line

//`#start footer` -- edit after this line, do not edit this line

reg [5:0] resets_mask;
always @(posedge clock or posedge kill)begin
    if (kill)begin 
        resets_mask<=6'b111_000;
    end else if (!pulse)begin
        case ({dir,halls})
            //front
            4'b0001:resets_mask<=6'b010_100;//A->C
            4'b0011:resets_mask<=6'b100_010;//B->C
            4'b0010:resets_mask<=6'b001_010;//B->A
            4'b0110:resets_mask<=6'b010_001;//C->A
            4'b0100:resets_mask<=6'b100_001;//C->B
            4'b0101:resets_mask<=6'b001_100;//A->B
            //back
            4'b1001:resets_mask<=6'b100_001;//C->B
            4'b1011:resets_mask<=6'b001_100;//A->B
            4'b1010:resets_mask<=6'b010_100;//A->C
            4'b1110:resets_mask<=6'b100_010;//B->C
            4'b1100:resets_mask<=6'b001_010;//B->A
            4'b1101:resets_mask<=6'b010_001;//C->A
            //error
            default:resets_mask<=6'b111000;
        endcase
    end else begin
        resets_mask<=resets_mask;//hold
    end
end

//output
assign resets=resets_mask[5:3];
assign pwms=resets_mask[2:0]&(pulse?3'b111:3'b000);


//`#end` -- edit above this line, do not edit this line
endmodule 