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

int getMaxOfThree(int a, int b, int c) {
    if (a >= b && a >= c) {
        return a;
    } else if (b >= a && b >= c) {
        return b;
    } else {
        return c;
    }
}

EdgeDetectionController::Sensor EdgeDetectionController::ScanForEdges()
{
  // Read sensor values
  int leftValue = analogRead(LEFT_SENSOR_PIN);
  int middleValue = analogRead(MIDDLE_SENSOR_PIN);
  int rightValue = analogRead(RIGHT_SENSOR_PIN);

  // Edge detection logic geberal threshold to stop robot
  if (leftValue > THRESHOLD || middleValue > THRESHOLD || rightValue > THRESHOLD) {
    // Edge detected
    // function to reutrn enum
    if(getMaxOfThree(leftValue, middleValue, rightValue) == leftValue)
    {
      return Sensor::Left;
    }
    else if(getMaxOfThree(leftValue, middleValue, rightValue) == middleValue)
    {
      return Sensor::Middle;
    }
    else {
      return Sensor::Right;
    }
  } else {
    return Sensor::None;     // Safe to move forward // this function would be autoPilot
  }

  delay(100);  // Small delay for stability
}



