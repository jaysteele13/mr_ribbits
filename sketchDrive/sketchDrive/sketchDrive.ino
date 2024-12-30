#include <avr/wdt.h>
#include "driveMotor.h"
#include "SwitchController.h"
#include "LedController.h"
#include "UltraSonicController.h"
#include "functioniseParts.cpp"
#include "BuzzerController.h"
#include "EdgeDetectionController.h"

// to clean up
/*
- Scan for edges, push it out and test more with stopping thats not int the controoler
- the controllers job is to return data, this should be parsed elsewhere in functionalise parts where everything is
- get motor to work in tandem with other controllers
*/

MotorDriver moterDriver;
SwitchController switchController;
LedController ledController;
UltraSonicController ultraSonicController;
MiniServoController miniServoController;
Application application;
BuzzerController buzzerController;
EdgeDetectionController edgeDetectionController;
//extern Application application

void setup() {
  Serial.begin(9600);
  edgeDetectionController.Init();
  motorDriver.MotorDriverInit();
  buzzerController.BuzzerControllerInit();
  switchController.SwitchControllerInit();
  ledController.LedControllerInit();
  ultraSonicController.UltraSonicControllerInit();
  miniServoController.MiniServoControllerInit();
}
unsigned long lastActionTime = 0;

// Function to check if the interval has elapsed
bool isTimeElapsed(unsigned long &lastTime, unsigned long interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - lastTime >= interval) {
    lastTime = currentMillis; // Update the last action time
    return true; // Interval elapsed
  }
  return false; // Interval not elapsed
}

// Example usage in EdgeDetectionController
const long moveInterval = 1000; // Scan interval in milliseconds

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
      edgeDetectionController.ScanForEdges();
    }
    
  }  
  else 
  {
    // turn led light on have motors deactivated
    ledController.SetAndEnableRGB();
    ControlBot(DirectionControl::stop_it, 0);
  }
}

void loop() {
  test_all_controls();
  ReturnSonicDistance(20);
  edgeDetectionController.ScanForEdges();
}
