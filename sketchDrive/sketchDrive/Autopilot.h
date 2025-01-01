#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include <Arduino.h>
#include <avr/wdt.h>
#include "driveMotor.h"
#include "UltraSonicController.h"
#include "BuzzerController.h"
#include "LedController.h"
#include "MiniServoController.h"
#include "EdgeDetectionController.h"
#include "functioniseParts.cpp"

class AutoPilot 
{
  public:
    enum class RobotState {
      Moving,
      Stopped
    };
    Init(void);
    Roam(int duration);
    Stop(void);
    boolean isActive;
    ScanSonicDistance(uint8_t threshold);
    DetectObstacle(uint8_t threshold);
    bool DetectEdge();
    void TiltHead();
    void SoundBuzzer();
  private:
    RobotState state;
    EdgeDetectionController::Sensor sensorProxy;
    int sonic_distance;
  public:
  
    MotorDriver motorDriver;
    BuzzerController buzzerController;
    UltraSonicController ultraSonicController;
    EdgeDetectionController edgeDetectionController;
    LedController ledController;
    MiniServoController miniServoController;
};

#endif