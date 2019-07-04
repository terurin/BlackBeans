
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
	output  reg dir,
	input   clock,
	input  [2:0] halls,
	input   reset
);

//`#start body` -- edit after this line, do not edit this line
//capture
assign count = ^halls;
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
//
wire last_now ={last,now}; 
always @(last_now)begin
    case ({last_now})
        6'b001_011:dir<=1;
        6'b011_010:dir<=1;
        6'b010_110:dir<=1;
        6'b110_100:dir<=1;
        6'b100_101:dir<=1;
        6'b101_001:dir<=1;
        default:dir<=0;
    endcase
end
//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
