#include "MiniServoController.h"
#include <Arduino.h>

MiniServoController::MiniServoControllerInit()
{
  myservo.attach(PIN_Servo_z, 500, 2400); //500: 0 degree  2400: 180 degree
  myservo.write(90); //sets the servo position according to the 90（middle）
  delay(200);
}

MiniServoController::SetAngle(uint8_t Position_angle)
{
  if (Position_angle != currentAngle) 
  {
    targetAngle = Position_angle;
    myservo.attach(PIN_Servo_z, 500, 2400);  // Attach the servo with specified min/max pulse width
    myservo.write(targetAngle);              // Move to the target position
    previousMillis = millis();              // Reset the timer
    isMoving = true;                         // Mark the servo as moving
  }
  else 
  {
    myservo.detach();
  }
}

MiniServoController::GetAngle()
{
  return myservo.read();
}

MiniServoController::Update() 
{
  // If the servo is moving, check if the move is completed
  if (isMoving) {
      unsigned long currentMillis = millis();  // Get the current time

      // Check if enough time has passed for the servo to complete its move
      if (currentMillis - previousMillis >= moveDuration) {
          myservo.detach();  // Detach the servo after moving
          currentAngle = targetAngle;  // Update the current angle
          isMoving = false;   // Mark the servo as no longer moving
      }
  }
}
