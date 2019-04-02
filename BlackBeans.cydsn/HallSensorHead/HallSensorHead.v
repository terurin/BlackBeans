
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
// Generated on 03/31/2019 at 04:51
// Component: HallSensorHead
module HallSensorHead (
	output  down,
	output  up,
	input   clock,
	input  [2:0] halls
);

//`#start body` -- edit after this line, do not edit this line
reg [2:0] now,last;
always @(posedge clock)begin
    now<=halls;
    last<=now;
end

//compute difference
reg [1:0] up_down;
assign up=up_down[1];
assign down=up_down[0];
always @(posedge clock)begin
    case ({last,now})
    //001
    6'b001_101:up_down<=2'b01;
    6'b001_011:up_down<=2'b10;
    //011
    6'b011_001:up_down<=2'b01;
    6'b011_010:up_down<=2'b10;
    //010
    6'b010_011:up_down<=2'b01;
    6'b010_110:up_down<=2'b10;
    //110
    6'b110_010:up_down<=2'b01;
    6'b110_100:up_down<=2'b10;
    //100
    6'b100_110:up_down<=2'b01;
    6'b100_101:up_down<=2'b10;
    //101
    6'b101_100:up_down<=2'b01;
    6'b101_001:up_down<=2'b10;
    //no difference or error
    default:up_down<=2'b00;
endcase
end
//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
