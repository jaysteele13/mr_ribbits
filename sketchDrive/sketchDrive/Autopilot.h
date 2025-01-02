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
#include "SwitchController.h"

class AutoPilot 
{
  public:
    // have default configuration
    enum class RobotState {
      Moving,
      Stopped
    };
    enum DirectionControl
    {
      Forward,
      Backward,
      Left,
      Right,
      LeftForward,
      LeftBackward,
      RightForward,
      RightBackward,
      stop_it
    };
    // defaults
    static const uint8_t defaultSensorThreshold = 20;
    const uint8_t defaultSpeed = 45;

    // public functions
    Init(void);
    Move(DirectionControl direction, uint8_t _speed); 
    Roam(void);
    Stop(void);
    StopBySwitch(void);
    bool isActive;
    SetAutoPilot(bool toggle);
    PivotBySensor(uint8_t threshold = defaultSensorThreshold);
    DetectObstacle(uint8_t threshold = defaultSensorThreshold);
    Reverse180(void);
    PivotByEdge(void);
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
    SwitchController switchController;
};

#endif