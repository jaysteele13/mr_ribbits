
#ifndef EDGEDETECTIONCONTROLLER_H
#define EDGEDETECTIONCONTROLLER_H

#include <Arduino.h>
#include <avr/wdt.h>
#include "driveMotor.h"
#include "BuzzerController.h"

class EdgeDetectionController 
{
  public:
    enum class Sensor {Left, Middle, Right, None};
    Init(void);
    Sensor ScanForEdges();
  private:
    reverseRobotBasedOnSensor(int leftValue, int middleValue, int rightValue);
    #define LEFT_SENSOR_PIN A0  // Pin for the left sensor
    #define MIDDLE_SENSOR_PIN A1  // Pin for the middle sensor
    #define RIGHT_SENSOR_PIN A2  // Pin for the right
    #define THRESHOLD 500  // Threshold for edge detection (adjust based on testing)
    #define GROUNDED_THRESHOLD 1015
};

#endif