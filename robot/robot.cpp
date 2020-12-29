#include "robot.h"
#include <avr/io.h>
#include <Wire.h>

#define mt1cw 3
#define mt1ccw 5
#define mt2cw 10
#define mt2ccw 9
#define mt3cw 11
#define mt3ccw 6
#define cw 1
#define ccw 2
#define echo 12
#define trig 13
#define linePin 1
#define IR1 8
#define IR2 17
#define IR3 16
#define IR4 14
#define LED 4
#define framenum 5
robot::robot() {}

//以下は赤外線リモコン関連のもの
void robot::getIRinfo()
{
  Serial.println("リモコンの使用したいボタンを１回押してください");
  getIRleader();
  Serial.println("ボタンを離してください");
  delay(4000);
  Serial.println("リモコンの使用したいボタンを１回押してください");
  getFramedata();
  Serial.println("ボタンを離してください");
  delay(4000);
}

void robot::getIRleader()
{
  while (digitalRead(IR1) == 1)
  {
    now1 = micros();
  }
  while (digitalRead(IR1) == 0)
  {
    last = micros();
  }
  leader = last - now1;
  unsigned long last_leader = 0;
  while (last_leader < 10000)
  {
    while (digitalRead(IR1) == 1)
    {
    }
    while (digitalRead(IR1) == 0)
    {
      now2 = micros();
    }
    frame_num++;
    while (digitalRead(IR1) == 1)
    {
      last = micros();
    }
    last_leader = last - now2;
    frame_num++;
  }
  if (leader < 9200 && leader > 8800)
  {
    Serial.print("format :");
    Serial.println("NEC");
  }
  Serial.print("frame number :");
  Serial.println(frame_num - 2);
}

void robot::getFramedata()
{
  frame_num = frame_num + 4;
  int long frame_data[frame_num];
  int data_num = 0;
  while (data_num <= frame_num)
  {
    while (digitalRead(IR1) == 1)
    {
      now1 = micros();
    }
    while (digitalRead(IR1) == 0)
    {
      last = micros();
    }
    frame_data[data_num] = last - now1;
    data_num++;
    now2 = last;
    while (digitalRead(IR1) == 1)
    {
      last = micros();
    }
    now1 = micros();
    frame_data[data_num] = last - now2;
    data_num++;
  }
  for (int i = 2; i < frame_num - 4; i++)
  {
    frame_data[i] = roundOff(frame_data[i], 562);
  }
  Serial.print("Repeat type :");
  if (frame_data[frame_num - 1] < 2400 && frame_data[frame_num - 1] > 1600)
  {
    Serial.println("0");
  }
  else
  {
    Serial.println("1");
  }
  Serial.print("frame data :");
  Serial.print("{");
  for (int j = 1; j < (frame_num - 4) / 2; j++)
  {
    if (frame_data[j * 2] == 1 && frame_data[(j * 2) + 1] == 1)
    {
      Serial.print("0,");
    }
    else if (frame_data[j * 2] == 1 && frame_data[(j * 2) + 1] == 3)
    {
      Serial.print("1,");
    }
    else
    {
      Serial.println("N/A,");
      break;
    }
  }
  Serial.println("}");
  Serial.println("N/Aが含まれている場合、取得は失敗しています。初めからやり直してください");
}


int robot::irControl(int framedata, int *data1, int *data2, int *data3, int *data4, int *data5)
{
  unsigned long now1 = 0;
  unsigned long now2 = 0;
  unsigned long last1 = 0;
  unsigned long last2 = 0;
  unsigned long high = 0;
  unsigned long low = 0;
  int data_num = 0;
  int datacheck[framenum] = {};
  int loopbreak = 0;
  int irData[framenum][framedata] = {{}};
  for (int i = 0; i < framedata; i++)
  {
    irData[0][i] = data1[i];
    irData[1][i] = data2[i];
    irData[2][i] = data3[i];
    irData[3][i] = data4[i];
    irData[4][i] = data5[i];
  }
  int data[framedata] = {};

  while (1)
  {
    while (digitalRead(IR3) == 1)
    {
      now1 = micros();
      if (now1 - last2 > 100000)
      {
        loopbreak = 1;
        break;
      }
    }
    while (digitalRead(IR3) == 0)
    {
      last1 = micros();
    }
    high = roundOff(last1 - now1, 562);
    now2 = last1;
    while (digitalRead(IR3) == 1)
    {
      last2 = micros();
      if (last2 - now2 > 100000)
      {
        loopbreak = 1;
        break;
      }
    }
    if (loopbreak == 1)
    {
      break;
    }
    now1 = last2;
    low = roundOff(last2 - now2, 562);
    if (high == 16 && low == 4)
    {
      for (int i = 0; i < framenum; i++)
      {
        if (datacheck[i] == 0)
        {
          for (int j = 16; j < framedata; j++)
          { //NECformat専用
            if (data[j] == irData[i][j])
            {
              datacheck[i]++;
            }
            else
            {
              datacheck[i] = 0;
              break;
            }
          }
        }
      }
      for (int i = 0; i < framenum; i++)
      {
        if (datacheck[i] == 16)
        { 
          return i+1;
        }
      }
    }
    else if (high == 1 && low == 1)
    {
      data[data_num] = 0;
      data_num++;
    }
    else if (high == 1 && low == 3)
    {
      data[data_num] = 1;
      data_num++;
    }
    else if (high == 16 && low == 8)
    {
      data_num = 0;
      for (int i = 0; i < framedata; i++)
      {
        data[i] = 0;
      }
      for (int i = 0; i < framenum; i++)
      {
        datacheck[i] = 0;
      }
    }
  }
  return 0;
}

int robot::roundOff(float numerator, float denominator)
{ 
  float decimal = 0;
  int integer = 0;
  decimal = numerator / denominator;
  integer = decimal;
  if ((decimal - integer) >= 0.5)
  {
    integer = integer + 1;
  }
  return integer;
}
//ここまで

double robot::dist()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  unsigned long durationMS = pulseIn(echo, HIGH);
  double distance = durationMS / 2.0 * 0.034;
  if (distance == 2 || distance > 400)
  {
    return -1.0;
  }
  else
  {
    return distance;
  }
}

void robot::motor(double spd1, double spd2, double spd3)
{
  int offtime = 0;
  static int mt_state[3];
  double mt_power[6];

  double spd[3] = {spd1, spd2, spd3};
  for (int i = 0; i < 3; i++)
  {
    if (spd[i] > 0)
    {
      mt_power[i * 2] = spd[i];
      mt_power[i * 2 + 1] = 0;
      if (mt_state[i] == ccw)
      {
        offtime = 1;
        mt_state[i] = cw;
      }
      mt_state[i] = 1;
    }
    else if (spd[i] < 0)
    {
      mt_power[i * 2] = 0;
      mt_power[i * 2 + 1] = -spd[i];
      if (mt_state[i] == cw)
      {
        offtime = 1;
        mt_state[i] = ccw;
      }
    }
    else
    {
      mt_power[i * 2] = 0;
      mt_power[i * 2 + 1] = 0;
      mt_state[i] = 0;
    }
  }
  /*
  if (offtime == 1)
  {
    digitalWrite(mt1cw, LOW);
    digitalWrite(mt1ccw, LOW);
    digitalWrite(mt2cw, LOW);
    digitalWrite(mt2ccw, LOW);
    digitalWrite(mt3cw, LOW);
    digitalWrite(mt3ccw, LOW);
    delayMicroseconds(100);
  }
  */
  TCCR0A = 0b10100011;
  TCCR0B = 0b00000011;
  TCCR1A = 0b10100010;
  TCCR1B = 0b00011011;
  TCCR2A = 0b10100011;
  TCCR2B = 0b00000100;
  ICR1 = 255;

  OCR2B = (unsigned int)(mt_power[0]); //3  mt1cw  980hz
  OCR0B = (unsigned int)(mt_power[1]); //5  mt1ccw 980hz
  OCR1A = (unsigned int)(mt_power[3]); //(1020*(mt_power[3]/255));//10  mt2cw   490hz 
  OCR1B = (unsigned int)(mt_power[2]); //(1020*(mt_power[2]/255));//9   mt2ccw  490hz
  OCR2A = (unsigned int)(mt_power[4]); //11 mt3cw  980hz
  OCR0A = (unsigned int)(mt_power[5]); //6  mt3ccw 980hz
}

void robot::getLine(double line[4]){
  Wire.requestFrom(8, 4);
  int i=0;
  while (Wire.available()) { 
    line[i] = Wire.read(); 
    i++;
  }
}

void robot::getIr(double ir[4])
{ 
  for (int i = 0; i < 500; i++)
  {
    if (digitalRead(IR1) == LOW)
    {
      ir[0]++;
    }
    if (digitalRead(IR2) == LOW)
    {
      ir[1]++;
    }
    if (digitalRead(IR3) == LOW)
    {
      ir[2]++;
    }
    if (digitalRead(IR4) == LOW)
    {
      ir[3]++;
    }
  }
}

void robot::initialize(void)
{
  pinMode(LED, OUTPUT);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(mt1cw, OUTPUT);
  pinMode(mt1ccw, OUTPUT);
  pinMode(mt2cw, OUTPUT);
  pinMode(mt2ccw, OUTPUT);
  pinMode(mt3cw, OUTPUT);
  pinMode(mt3ccw, OUTPUT);
  digitalWrite(mt1cw, LOW);
  digitalWrite(mt1ccw, LOW);
  digitalWrite(mt2cw, LOW);
  digitalWrite(mt2ccw, LOW);
  digitalWrite(mt3cw, LOW);
  digitalWrite(mt3ccw, LOW);
  Serial.begin(115200);
  Wire.begin();        // join i2c bus (address optional for master)

}