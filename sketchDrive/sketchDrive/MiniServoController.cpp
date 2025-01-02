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
   if (Position_angle != currentAngle) 
   {
    targetAngle = Position_angle;
    isMoving = true;  // Start moving
    previousMillis = millis();
    myservo.attach(PIN_Servo_z, 500, 2400); // Attach only when needed
   }
}

MiniServoController::GetAngle()
{
  return myservo.read();
}

MiniServoController::Update() 
{
  if (isMoving) 
  {
    unsigned long currentMillis = millis();
    unsigned long elapsedMillis = currentMillis - previousMillis;

    // Calculate the expected angle based on elapsed time and speed
    int step = elapsedMillis * degreesPerMs;
    if (targetAngle > currentAngle) {
        currentAngle = min(targetAngle, currentAngle + step);
    } else if (targetAngle < currentAngle) {
        currentAngle = max(targetAngle, currentAngle - step);
    }

    myservo.write(currentAngle);
    previousMillis = currentMillis; // Reset timer

    // Stop moving if the target angle is reached
    if (currentAngle == targetAngle) {
        isMoving = false;
        delay(50);       // Short delay to stabilize
        myservo.detach(); // Detach after motion stops
    }
  }
}
