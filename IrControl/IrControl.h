#ifndef _IrControl_H_
#define _IrControl_H_
#include "arduino.h"
class IrControl
{
public:
    IrControl();
    void irRead();

private:
    volatile unsigned long start = 0;
    volatile unsigned long end = 0;
    volatile int value[64] = {};
    volatile int state = 0;
};
#endif