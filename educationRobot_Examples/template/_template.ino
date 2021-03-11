#include <QMC5883LCompass.h>
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           #include <robot.h>
QMC5883LCompass compass;

#define IR1 1
#define IR2 2
#define IR3 3
#define IR4 4
#define LED 4

robot ir;

boolean initComp = false;//キャリブレーションに必要な変数
double flontDeg = 0; //ロボットの起動時に前方方向が向いている方位

void interrupt()
{
  ir.motor(0, 0, 0);
  digitalWrite(LED, LOW);
  initComp = true;

}

void setup() {
  //触ってはいけないコード
  compass.init();
  ir.initialize();
  attachInterrupt(0, interrupt, LOW);
  Serial.begin(115200);
  //calibration
  boolean calibComp = false;
  while (!initComp) {
    unsigned long t = millis();
    while (!calibComp) {
      //compass calibration
      double calibData[3][2];
      boolean changed = false;
      double x, y = 0;
      unsigned long t1, t2 = 0;
      compass.read();
      x = compass.getX();
      y = compass.getY();
      if (x < calibData[0][0]) {
        calibData[0][0] = x;
        changed = true;
      }
      if (x > calibData[0][1]) {
        calibData[0][1] = x;
        changed = true;
      }

      if (y < calibData[1][0]) {
        calibData[1][0] = y;
        changed = true;
      }
      if (y > calibData[1][1]) {
        calibData[1][1] = y;
        changed = true;
      }
      if (changed) {
        t1 = millis();
      }
      t2 = millis();
      if ((t - t2) > 8000) {
        if ((t2 - t1) > 5000) {
          compass.setCalibration(calibData[0][0], calibData[0][1], calibData[1][0], calibData[1][1], 0, 0);
          calibComp = true;
        }
      }
    }
    digitalWrite(LED, HIGH); //calibration complete
  }
  compass.read();
  flontDeg = compass.getAzimuth();
  //calibration end
  //ここからコードを書いてください。

}

void loop() {
  //触ってはいけないコード
  digitalWrite(LED, HIGH);//毎プログラムに入れないといけないやつ
  //ここからコードを書いてください。

}
