#include <QMC5883LCompass.h>
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           #include <robot.h>
#include <math.h>
robot ir;
QMC5883LCompass compass;

#define LED 4//毎プログラムに入れないといけないやつ
#define LINE_VALUE 200//ラインセンサーの値はここをいじってね！！
#define IR_AROUND 45//赤外線の回り込みの角度
#define Kp 20  //比例ゲイン
#define Ki 0.8 //積分ゲイン
#define Kd 0.8 //微分ゲイン
double flontDeg = 0; //前方方向の方位
double proportion, integral, differential, preP = 0;
boolean initComp = false;

void comp() {
  int deg = 0;
  double times = micros();
  double dt = 0;
  double dev = 0;
  compass.read();
  deg = compass.getAzimuth();
  while ((flontDeg - deg) <= -5 || (flontDeg - deg) >= 5) {
    dt = (micros() - times) / 1000000;
    times = micros();
    dev = pid(flontDeg, deg, dt);
    dev = dev / 180 * 220;
    ir.motor(dev, dev, dev);
    compass.read();
    deg = compass.getAzimuth();
  }
  ir.motor(0, 0, 0);
}

double pid(int target, int nowValue, double dt) {
  double dev = 0;
  proportion = target - nowValue;
  if (proportion > 0) {
    if (proportion > 180) {
      proportion = proportion - 360;
    }
  } else {
    if (proportion < -180) {
      proportion = proportion + 360;
    }
  }
  integral += proportion * dt;
  differential = (proportion - preP) / dt;
  preP = proportion;

  //Serial.println(differential, 10);
  dev = Kp * proportion + Ki * integral + Kd * differential;
  return dev;
}

void irTrack() {
  int irValue[4] = {};
  double x, y, deg = 0;
  ir.getIr(irValue);
  x = irValue[1] - irValue[3];
  y = irValue[0] - irValue[2];
  deg = atanDeg(y, x);
  if (deg == NAN) {
    ir.motor(0, 0, 0);
  } else {
    if (deg <= 20 || deg >= 340) {
      deg = 0;
    } else {
      if (deg >= 180) {
        deg = deg - 45;
      } else {
        deg = deg + 45;
      }
      mt(deg, 255);
      delay(100);
    }
  }
}

double atanDeg(double num1, double num2) { //(y,x)
  double deg = 0;
  if (num2 == 0 && num1 == 0) {
    deg = NAN;
  } else if (num2 == 0) {
    if (num1 > 0) {
      deg = 360;
    } else {
      deg = 180;
    }
  } else if (num1 == 0) {
    if (num2 > 0) {
      deg = 90;
    } else {
      deg = 270;
    }
  } else {
    if (abs(num1) > abs(num2)) {
      if (num1 > 0) {
        //0<deg<=45&&315<=deg<360,y=num1,x=num2
        deg = atan(num1 / num2) / M_PI * 180;
        if (deg < 0) {
          deg = 360 + deg;
        }
      } else {
        //45<=deg<=135,y=num1,x=num2
        deg = atan(num1 / num2) / M_PI * 180;
        deg += 180;
      }
    } else {
      if (num2 > 0) {
        //135<=deg<=225,y=num2,x=num1
        deg = atan(num2 / num1) / M_PI * 180;
        //if (deg > 0) {
        //   deg = 180 + deg;
        //} else {
        deg = 180 + deg;
        // }
      } else {
        //225<=deg<=315,y=num2,x=num1
        deg = atan(num2 / num1) / M_PI * 180;
        if (deg > 0) {
          deg = 270 + deg;
        } else {
          deg = 270 - deg;
        }
      }
    }
  }
  return deg;
}

void line() {
  int line[4] = {};
  ir.getLine(line);
  if (line[0] < LINE_VALUE) {
    mt(180, 255);
    delay(100);
  }
  if (line[1] < LINE_VALUE) {
    mt(270, 255);
    delay(100);
  }
  if (line[2] < LINE_VALUE) {
    mt(0, 255);
    delay(100);
  }
  if (line[3] < LINE_VALUE) {
    mt(90, 255);
    delay(100);
  }
}

void mt(double deg, double spd) {
  int mt[3];
  double maxValue = 0;
  mt[0] = sin(((deg - 60) / 180) * M_PI) * spd ;
  mt[1] = sin(((deg - 180) / 180) * M_PI) * spd ;
  mt[2] = sin(((deg - 300) / 180) * M_PI) * spd ;
  maxValue = max(max(mt[0], mt[1]), mt[2]);
  for (int i = 0; i < 3; i++) {
    mt[i] = mt[i] / maxValue * 255;
  }
  ir.motor(mt[0], mt[1], mt[2]);
}
void interrupt()
{ //毎プログラムに入れないといけないやつ
  ir.motor(0, 0, 0);
  digitalWrite(LED, LOW);
  initComp = true;
}
void setup() {
  // put your setup code here, to run once:
  compass.init();
  ir.initialize();
  attachInterrupt(0, interrupt, LOW);//毎プログラムに入れないといけないやつ
  boolean calibComp = false;
  while (!initComp) {
    unsigned long t = millis();
    while (!calibComp) {
      //compass calibration
      double calibData[3][2];
      boolean changed = false;
      double x, y, z = 0;
      unsigned long t1, t2 = 0;
      compass.read();
      x = compass.getX();
      y = compass.getY();
      z = compass.getZ();
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

      if (z < calibData[2][0]) {
        calibData[2][0] = z;
        changed = true;
      }
      if (z > calibData[2][1]) {
        calibData[2][1] = z;
        changed = true;
      }
      if (changed) {
        t1 = millis();
      }
      t2 = millis();
      if ((t - t2) > 8000) {
        if ((t2 - t1) > 5000) {
          compass.setCalibration(calibData[0][0], calibData[0][1], calibData[1][0], calibData[1][1], calibData[2][0], calibData[2][1]);
          calibComp = true;
        }
      }
    }
    digitalWrite(LED, HIGH); //calibration complete
  }
  compass.read();
  flontDeg = compass.getAzimuth();
}
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);//毎プログラムに入れないといけないやつ
  comp();
  //line();
  //irTrack();
  /*
    int irValue[4] = {};
    double x, y, deg = 0;
    ir.getIr(irValue);
    for (int i = 0; i < 4; i++) {
    Serial.println(i);
    Serial.println(irValue[i]);
    }*/
}
