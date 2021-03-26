#include "IrControl.h"
#include <avr/io.h>
#include <Wire.h>

IrControl::IrControl() {}

ISR(PCINT0_vect)
{
    if (state < 64)
    {
        end = micros();
        value[state] = end - start;
        start = end;
        state++;
    }
}

void IrControl::irRead()
{
    if (value[63] != 0)
    {
        //Serial.print("{");
        for (int i = 0; i < 64; i++)
        {
            Serial.println(value[i]);
            if (i != 63)
            {
                //Serial.print(",");
            }
        }
        //Serial.println("}");
        Serial.println(" ");
        value[63] = 0;
    }
}

void IrControl::init()
{
    Serial.begin(9600);
    PCICR |= B00000001;
    PCMSK0 |= B00000001;
    Serial.println("ボタン押せ");
}