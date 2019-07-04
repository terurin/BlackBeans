
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
// Generated on 07/04/2019 at 20:10
// Component: HallSensorHead
module HallSensorHead (
	output  count,
	output  dir,
	input   clock,
	input  [2:0] halls,
	input   reset
);

//`#start body` -- edit after this line, do not edit this line

reg [2:0] now,last;
always @(clock)begin
    if (reset)begin
        now<=0;
        last<=0;
    end else begin
        now<=halls;
        last<=now;
    end
end

assign count=^(now^last);
assign dir=0;

//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
