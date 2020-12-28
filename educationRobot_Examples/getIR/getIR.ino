/*
 * これは赤外線リモコンのボタン一つ一つに割り当てられた値を読み取るプログラムです。
 * シリアルモニターに表示される案内に従って操作して下さい。
 * シリアルの転送速度は115200bpsです
 */
#include "robot.h"
robot ir;
void setup() {
  ir.initialize();
  ir.getIRinfo();
}

void loop() {

}
