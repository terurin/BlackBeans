
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
	input           clock,
	input           direction,
	input  [2:0]    halls,
	input   kill,
	input   pulse,
	output [2:0] pwms,
	output [2:0] resets_n,
    output error
);

//`#start body` -- edit after this line, do not edit this line

//`#start footer` -- edit after this line, do not edit this line

reg [6:0] error_resets_mask;
always @(posedge clock or posedge kill)begin
    if (kill)begin 
         error_resets_mask<=7'b0_111_000;
    end else if (!pulse)begin
        casex ({direction,halls})
            //front 
            // duvw                       e uvw uvw
            4'b0001:error_resets_mask<=7'b0_001_010;//B->C
            4'b0011:error_resets_mask<=7'b0_100_010;//B->A
            4'b0010:error_resets_mask<=7'b0_010_100;//C->A
            4'b0110:error_resets_mask<=7'b0_001_100;//C->B
            4'b0100:error_resets_mask<=7'b0_100_001;//A->B
            4'b0101:error_resets_mask<=7'b0_010_001;//A->C
            4'b0xxx:error_resets_mask<={1'b1,error_resets_mask[5:0]};
            //back
            4'b1001:error_resets_mask<=7'b0_010_100;//C->A
            4'b1011:error_resets_mask<=7'b0_001_100;//C->B
            4'b1010:error_resets_mask<=7'b0_100_001;//A->B
            4'b1110:error_resets_mask<=7'b0_010_001;//A->C
            4'b1100:error_resets_mask<=7'b0_001_010;//B->C
            4'b1101:error_resets_mask<=7'b0_100_010;//B->A
            4'b1xxx:error_resets_mask<={1'b1,error_resets_mask[5:0]};
            //default:error_resets_mask<=7'b1_111_000;
        endcase
    end else begin
        error_resets_mask<=error_resets_mask;//hold
    end
end

//output
assign error= error_resets_mask[6];
assign resets_n=~error_resets_mask[5:3];
assign pwms=error_resets_mask[2:0]&(pulse?3'b111:3'b000);

//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line

//`#end` -- edit above this line, do not edit this line
