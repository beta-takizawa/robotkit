                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           #include "robot.h"
robot ir;
#define FRAME_DATA 32 //frame_dataを書き込んでください。
int data1[FRAME_DATA] = ;//ボタン登録1
int data2[FRAME_DATA] = ;//ボタン登録2
int data3[FRAME_DATA] = ;//ボタン登録3
int data4[FRAME_DATA] = ;//ボタン登録4
int data5[FRAME_DATA] = ;//ボタン登録5

void setup() {
  ir.initialize();
}

void loop() {
  int data = ir.irControl(FRAME_DATA, data1, data2, data3, data4, data5);
  switch (data) {
    case 0:
      //リモコンからの信号がない時の動作
      ir.motor(0, 0, 0);
      //ここまでです。
      break;
    case 1:
      //以下にボタン登録1に対応する動作を書き込んでください。
      ir.motor(200, 200, 200);
      //ここまでです。
      break;
    case 2:
      //以下にボタン登録2に対応する動作を書き込んでください。
      ir.motor(200, 200, 200);
      //ここまでです。
      break;
    case 3:
      //以下にボタン登録3に対応する動作を書き込んでください。
      ir.motor(200, 200, 200);
      //ここまでです。
      break;
    case 4:
      //以下にボタン登録4に対応する動作を書き込んでください。
      ir.motor(200, 200, 200);
      //ここまでです。
      break;
    case 5:
      //以下にボタン登録5に対応する動作を書き込んでください。
      ir.motor(200, 200, 200);
      //ここまでです。
      break;
    default :
      break;
  }
}
