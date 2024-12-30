#include "MiniServoController.h"
#include <Arduino.h>
/*Servo*/

Servo myservo; // create servo object to control a servo
MiniServoController::MiniServoControllerInit()
{
  myservo.attach(PIN_Servo_z, 500, 2400); //500: 0 degree  2400: 180 degree
  myservo.write(90); //sets the servo position according to the 90（middle）
  delay(500);
}

MiniServoController::SetAngle(uint8_t Position_angle)
{
  myservo.attach(PIN_Servo_z, 500, 2400);
  myservo.write(Position_angle);
  delay(450);
  myservo.detach();
}
