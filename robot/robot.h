#ifndef _robot_H_
#define _robot_H_
#include "arduino.h"
class robot
{
public:
    robot(int minX, int maxX, int minY, int maxY);
    robot();
    double dist();
    void motor(double spd1, double spd2, double spd3);
    int getIr(int num);
    int getLine(int num);
    void irUpdate();
    double getFlontAzim();
    double getAzim();
    void initialize();

private:
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