
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
        now<=filter(now,halls);
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
    6'b001_011:qei_up_down=2'b1_0;
    6'b011_010:qei_up_down=2'b1_0;
    6'b010_110:qei_up_down=2'b1_0;
    6'b110_100:qei_up_down=2'b1_0;
    6'b100_101:qei_up_down=2'b1_0;
    6'b101_001:qei_up_down=2'b1_0;
    //逆転
    6'b001_101:qei_up_down=2'b0_1;
    6'b101_100:qei_up_down=2'b0_1;
    6'b100_110:qei_up_down=2'b0_1;
    6'b110_010:qei_up_down=2'b0_1;
    6'b010_011:qei_up_down=2'b0_1;
    6'b011_001:qei_up_down=2'b0_1;
    //エラ-及び移動なし
    default:qei_up_down=2'b0_0;
    endcase
end
endfunction

//不正な入力を避ける
function [3:0] filter;
input [2:0]last;
input [2:0]now;
begin
   case (now)
   //異常値
   3'b111:filter=last;
   3'b000:filter=last;
   //正常値
   default:filter=now;
   endcase
end
endfunction

//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
