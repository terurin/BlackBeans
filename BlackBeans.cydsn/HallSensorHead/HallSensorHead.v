
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
        now<=decode(halls);
        last<=now;
    end
end

//qei decode
reg [2:0] up_down;
always @(posedge clock)begin
    if (reset)begin
        up_down<=0;
    end else begin
        up_down<=qei_up_down(last,now);
    end
end
//write out
assign up= up_down[0];
assign down= up_down[1];

//QEIの入力をデコードする
function [2:0] qei_up_down;
input [3:0] last;
input [3:0] now;
begin
    case ({last,now})
    //正転
    6'o01:qei_up_down=2'b01;
    6'o12:qei_up_down=2'b01;
    6'o23:qei_up_down=2'b01;
    6'o34:qei_up_down=2'b01;
    6'o45:qei_up_down=2'b01;
    6'o56:qei_up_down=2'b01;
    6'o60:qei_up_down=2'b01;
    //逆転
    6'o06:qei_up_down=2'b10;
    6'o65:qei_up_down=2'b10;
    6'o54:qei_up_down=2'b10;
    6'o43:qei_up_down=2'b10;
    6'o32:qei_up_down=2'b10;
    6'o21:qei_up_down=2'b10;
    6'o10:qei_up_down=2'b10;
    //Error
    default:qei_up_down=2'b00;
    endcase
end
endfunction

//角度を求める
function [3:0] decode;
input [3:0]halls;
begin
    case (halls)
    3'b001:decode=0;
    3'b011:decode=1;
    3'b010:decode=2;
    3'b110:decode=3;
    3'b100:decode=4;
    3'b101:decode=5;
    default:decode=7;//Error
    endcase
end
endfunction

//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
