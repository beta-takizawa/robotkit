#ifndef _robot_H_
#define _robot_H_
#include "arduino.h"
class robot{
    public:
        robot();
        void getIRinfo();
        void getIRleader();
        void getFramedata();
        int roundOff(float numerator, float denominator);
        int irControl(int framedata,int *forward,int *right,int *back,int *left,int *turn);
        //ここまで赤外線ラジコン用
        double dist();
        void motor(double spd1,double spd2,double spd3);
        void getIr(double ir[4]);
        void getLine(double line[4]);
        void initialize(void);
    private:
        unsigned long now1 = 0;
        unsigned long now2 = 0;
        unsigned long last = 0;
        unsigned long leader = 0;
        int frame_num = 0;
        
};
#endif