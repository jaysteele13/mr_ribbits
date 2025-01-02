#include "HardwareSerial.h"
#include "MiniServoController.h"
#include <Arduino.h>

MiniServoController::MiniServoControllerInit()
{
  int initialAngle = 90;
  currentAngle = initialAngle;  // Set to the desired initial angle
  targetAngle = initialAngle;
  myservo.attach(PIN_Servo_z, 500, 2400);
  myservo.write(initialAngle); // Move to the initial position
  delay(50);                   // Allow time for the servo to stabilize
  myservo.detach();
}

MiniServoController::SetAngle(uint8_t Position_angle)
{
    myservo.attach(PIN_Servo_z, 500, 2400);
    myservo.write(Position_angle);
   
}

MiniServoController::GetAngle()
{
  return myservo.read();
}

