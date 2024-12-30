#ifndef MINISERVOCONTROLLER_H
#define MINISERVOCONTROLLER_H

/*Servo*/
#include <Servo.h>
class MiniServoController
{
public:
  MiniServoControllerInit(void);
  SetAngle(uint8_t Position_angle);

private:
  #define PIN_Servo_z 10
};

#endif
