#include "EdgeDetectionController.h"
#include <Arduino.h>

EdgeDetectionController::Init()
{
  pinMode(LEFT_SENSOR_PIN, INPUT);
  pinMode(MIDDLE_SENSOR_PIN, INPUT);
  pinMode(RIGHT_SENSOR_PIN, INPUT);
}

EdgeDetectionController::reverseRobotBasedOnSensor(int leftValue, int middleValue, int rightValue) {
  if (leftValue < THRESHOLD) {
    Serial.println("Reversing and turning right...");
  } else if (rightValue < THRESHOLD) {
    // Edge detected on the right; turn left while reversing
    Serial.println("Reversing and turning left...");
  } else if (middleValue < THRESHOLD) {
    // Edge detected in the middle; reverse straight back
    Serial.println("Reversing straight...");
  }
  delay(500);
  motorDriver.Stop();  
}



void moveForward() {
  Serial.println("Moving forward...");
}

EdgeDetectionController::ScanForEdges()
{
  // Read sensor values
  int leftValue = analogRead(LEFT_SENSOR_PIN);
  int middleValue = analogRead(MIDDLE_SENSOR_PIN);
  int rightValue = analogRead(RIGHT_SENSOR_PIN);

  // // Debugging output
  // Serial.print("Left: ");
  // Serial.print(leftValue);
  // Serial.print(" Middle: ");
  // Serial.print(middleValue);
  // Serial.print(" Right: ");
  // Serial.println(rightValue);

  // Edge detection logic geberal threshold to stop robot
  if (leftValue > THRESHOLD || middleValue > THRESHOLD || rightValue > THRESHOLD) {
    // Edge detected
    motorDriver.Stop();     // Stop the robot immediately
    buzzerController.PlayTheme();
    reverseRobotBasedOnSensor(leftValue, middleValue, rightValue);    // Reverse a little to get away from the edge
  } else {
    moveForward();     // Safe to move forward // this function would be autoPilot
  }

  delay(100);  // Small delay for stability
}



