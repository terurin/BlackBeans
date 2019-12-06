
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
// Generated on 12/05/2019 at 19:15
// Component: HallSensorHead
module HallSensorHead (
	output  down,
	output  up,
	input   clock,
	input  [2:0] halls,
	input   reset
);

//`#start body` -- edit after this line, do not edit this line

//capture
reg [2:0] now;
reg [2:0] last;
always @(posedge clock)begin
    if (reset)begin
        now<=0;
        last<=0;
    end else begin
        now<=filter(halls,now);
        last<=now;
    end
end

wire [2:0]up_down=decode(now,last);
assign up =up_down[1];
assign down =up_down[0];

function [2:0]decode;
input [3:0]now;
input [3:0]last;
begin
    casex ({now,last})
    //0
    6'b000_xxx:decode=2'b00;
    //1
    6'b001_010:decode=2'b01;
    6'b001_011:decode=2'b01;
    6'b001_001:decode=2'b00;
    6'b001_101:decode=2'b10;
    6'b001_100:decode=2'b10;
    6'b001_xxx:decode=2'b00;
    //2
    6'b011_101:decode=2'b01;
    6'b011_001:decode=2'b01;
    6'b011_011:decode=2'b00;
    6'b011_010:decode=2'b10;
    6'b011_110:decode=2'b10;
    6'b011_xxx:decode=2'b00;
    //3
    6'b010_001:decode=2'b01;
    6'b010_011:decode=2'b01;
    6'b010_010:decode=2'b00;
    6'b010_110:decode=2'b10;
    6'b010_100:decode=2'b10;
    6'b010_xxx:decode=2'b00;
    //4
    6'b110_011:decode=2'b01;
    6'b110_010:decode=2'b01;
    6'b110_110:decode=2'b00;
    6'b110_100:decode=2'b10;
    6'b110_101:decode=2'b10;
    6'b110_xxx:decode=2'b00;
    //5
    6'b100_010:decode=2'b01;
    6'b100_110:decode=2'b01;
    6'b100_100:decode=2'b00;
    6'b100_101:decode=2'b10;
    6'b100_001:decode=2'b10;
    6'b100_xxx:decode=2'b00;
    //6
    6'b101_110:decode=2'b01;
    6'b101_100:decode=2'b01;
    6'b101_101:decode=2'b00;
    6'b101_001:decode=2'b10;
    6'b101_011:decode=2'b10;
    6'b101_xxx:decode=2'b00;
    //7
    6'b111_xxx:decode=2'b00;
    endcase
end
endfunction

function [3:0]filter;
input [3:0]inputs;
input [3:0]history;
begin
    case(inputs)
    //NG
    //3'b000:filter=history;
    //3'b111:filter=history;
    //OK
    default:filter=inputs;
    endcase
end
endfunction

//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
