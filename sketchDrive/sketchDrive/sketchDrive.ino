#include <avr/wdt.h>
#include "driveMotor.h"
#include "SwitchController.h"
#include "LedController.h"
#include "UltraSonicController.h"
#include "functioniseParts.cpp"

MotorDriver moterDriver;
SwitchController switchController;
LedController ledController;
UltraSonicController ultraSonicController;
MiniServoController miniServoController;
Application application;
//extern Application application

void setup() {
  Serial.begin(9600);
  motorDriver.MotorDriverInit();
  switchController.SwitchControllerInit();
  ledController.LedControllerInit();
  ultraSonicController.UltraSonicControllerInit();
  miniServoController.MiniServoControllerInit();
}

void test_all_controls() 
{
  if(!switchController.isSwitchActive()) 
  {
    // Do directions
    delay(50);
    
    ledController.SetAndEnableRGB(CRGB::DarkGreen);
    for (application.directionControl = 0; application.directionControl < 9; application.directionControl = application.directionControl + 1)
    {
      delay(1000);
      ReturnSonicDistance(20);
      ControlBot(application.directionControl /*direction*/, 100 /*speed*/);
    }
  } 
  else 
  {
    // turn led light on have motors deactivated
    ledController.SetAndEnableRGB();
    ControlBot(DirectionControl::stop_it, 0);


  }
}
//awful async
unsigned long testControlsPreviousMillis = 0;   // Store the last time test_all_controls() was called
unsigned long sonicDistancePreviousMillis = 0;   // Store the last time ReturnSonicDistance() was called
const unsigned long testControlsInterval = 1000; // Interval for test_all_controls()
const unsigned long sonicDistanceInterval = 200; // Interval for sonic distance check

void loop() {
  test_all_controls();
  ReturnSonicDistance(20);
}
