# 簡易版コマンドリスト

## 接続方法
UART(115200bps,パリティなし)
USB(CDC)

## コマンド(仮)
コマンドは、\nで終端される文字列から構成され、空白で分割される。
"コマンド名　引数1 引数2 引数3 ...\n"の順で与えられ、
"戻り値1 戻り値2 戻り値3\n"となる。 

* 原則として10進法で表現を行う
* a^nはaをn回繰り返すことを意味する

| コマンド名 |              引数              |       戻り値       |
| :--------: | :----------------------------: | :----------------: |
|     mf     |    モーターのduty比の分子^3    |        なし        |
|     mq     | モーターのduty比(Q15-Format)^3 |        なし        |
|     me     |              なし              | モーターの回転数^3 |