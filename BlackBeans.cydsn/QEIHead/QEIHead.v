
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
// Generated on 03/31/2019 at 03:38
// Component: QEIHead
module QEIHead (
	output  down,
	output  up,
	input   clock,
	input   x,
	input   y
);

//`#start body` -- edit after this line, do not edit this line

//fetch
reg [1:0] now,last;
always @(posedge clock)begin
    now <= {x,y};
    last <=now;
end

//decode
reg [1:0] up_down;
assign up=up_down[1],down=up_down[0];
always @(posedge clock)begin
    case ({last,now})
    //00
    4'b00_10:up_down<=2'b01;
    4'b00_01:up_down<=2'b10;
    //01
    4'b01_00:up_down<=2'b01;
    4'b01_11:up_down<=2'b10;
    //11
    4'b11_01:up_down<=2'b01;
    4'b11_10:up_down<=2'b10;
    //10
    4'b10_11:up_down<=2'b01;
    4'b10_00:up_down<=2'b10;
    //error or no changed
    default:up_down<=2'b00;
endcase
end
//        Your code goes here

//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
