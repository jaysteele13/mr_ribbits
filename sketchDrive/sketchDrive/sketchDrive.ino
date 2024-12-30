#include <avr/wdt.h>
#include "driveMotor.h"
#include "SwitchController.h"
#include "LedController.h"
#include "UltraSonicController.h"
#include "functioniseParts.cpp"
#include "BuzzerController.h"

MotorDriver moterDriver;
SwitchController switchController;
LedController ledController;
UltraSonicController ultraSonicController;
MiniServoController miniServoController;
Application application;
BuzzerController buzzerController;
//extern Application application

void setup() {
  Serial.begin(9600);
  motorDriver.MotorDriverInit();
  buzzerController.BuzzerControllerInit();
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
    buzzerController.PlayTheme();
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
}
