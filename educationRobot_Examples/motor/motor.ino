/*
   モーターを動かして、機体を時計回りに回転させるだけのプログラムです。
*/
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           #include "robot.h"
robot ir;
void setup() {
  // put your setup code here, to run once:
  ir.initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 256; i++) {
    ir.motor(i, i, i);
    delay(40);
  }
  for (int i = 255; i >= 0; i--) {
    ir.motor(i, i, i);
    delay(40);
  }
}
