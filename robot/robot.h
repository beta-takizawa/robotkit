#ifndef _robot_H_
#define _robot_H_
#include "arduino.h"
class robot
{
public:
    robot(int minX, int maxX, int minY, int maxY);
    //void getIRinfo();
    //void getIRleader();
    //void getFramedata();
    //int roundOff(float numerator, float denominator);
    //int irControl(int framedata,int *forward,int *right,int *back,int *left,int *turn);
    //ここまで赤外線ラジコン用
    double dist();
    void motor(double spd1, double spd2, double spd3);
    int getIr(int num);
    int getLine(int num);
    void irUpdate();
    double getFlontAzim();
    double getAzim();
    void initialize();

private:
    //unsigned long now1 = 0;
    //unsigned long now2 = 0;
    //unsigned long last = 0;
    //unsigned long leader = 0;
    //int frame_num = 0;
    double flontDeg = 0;
    int _ir1;
    int _ir2;
    int _ir3;
    int _ir4;
    int _maxX;
    int _minX;
    int _maxY;
    int _minY;
};
#endif