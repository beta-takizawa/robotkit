#include <Arduino.h>
#include <robot.h>
robot ir(-736, 1903, -1752, 1475); //ここは任意で書き変えてください。


void setup()
{
  // put your setup code here, to run once:
  ir.initialize();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
