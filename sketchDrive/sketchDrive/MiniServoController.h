#ifndef MINISERVOCONTROLLER_H
#define MINISERVOCONTROLLER_H

/*Servo*/
#include <Servo.h>
class MiniServoController
{
public:
  MiniServoControllerInit(void);
  SetAngle(uint8_t Position_angle);
  Update(void);

private:
  #define PIN_Servo_z 10
  Servo myservo;
  unsigned long previousMillis = 0;  // Last time the servo was updated
  uint8_t currentAngle = 0;          // Current position of the servo
  uint8_t targetAngle = 0;           // The new target angle for the servo
  bool isMoving = false;             // Track if the servo is still moving
  unsigned long moveDuration = 100;  // Duration for the move (in milliseconds)
};

#endif
