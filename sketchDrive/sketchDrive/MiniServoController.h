#ifndef MINISERVOCONTROLLER_H
#define MINISERVOCONTROLLER_H

/*Servo*/
#include <Servo.h>
class MiniServoController
{
public:
  MiniServoControllerInit(void);
  SetAngle(uint8_t Position_angle);
  int GetAngle();
private:
  #define PIN_Servo_z 10
  Servo myservo;
  uint8_t currentAngle = 0;       // Current servo angle (initialize at 0 for stability)
  uint8_t targetAngle = 0;        // Target servo angle
};

#endif
