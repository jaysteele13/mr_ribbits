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
   
    targetAngle = Position_angle;
    isMoving = true;  // Start moving
    previousMillis = millis();
    myservo.attach(PIN_Servo_z, 500, 2400); // Attach only when needed
   
}

MiniServoController::AddAngle(uint8_t Position_angle) 
{
    
        angleQueue.enqueue(Position_angle);
    
}

MiniServoController::GetAngle()
{
  return myservo.read();
}

MiniServoController::Update() 
{
  if (!isMoving && !angleQueue.isEmpty()) {
      // Start moving to the next angle in the queue
      angleQueue.dequeue(targetAngle);
      StartMove(targetAngle);
  }

  if (isMoving) {
      UpdateMovement();
  }
}

MiniServoController::StartMove(uint8_t Position_angle) {
        isMoving = true;
        myservo.attach(PIN_Servo_z, 500, 2400); // Attach only when needed
        previousMillis = millis();
    }

MiniServoController::UpdateMovement() {
    unsigned long currentMillis = millis();
    unsigned long elapsedMillis = currentMillis - previousMillis;

    // Calculate the expected angle based on elapsed time and speed
    int step = elapsedMillis * degreesPerMs;
    if (currentAngle < targetAngle) {
        currentAngle = min(targetAngle, currentAngle + step);
    } else if (currentAngle > targetAngle) {
        currentAngle = max(targetAngle, currentAngle - step);
    }

    myservo.write(currentAngle);
    previousMillis = currentMillis;

    // Stop moving if the target angle is reached
    if (currentAngle == targetAngle) {
        isMoving = false;
        delay(50); // Stabilize
        myservo.detach();
    }
}
