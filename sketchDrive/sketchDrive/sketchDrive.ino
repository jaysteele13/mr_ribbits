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
Application application;
//extern Application application

void setup() {
  Serial.begin(9600);
  motorDriver.MotorDriverInit();
  switchController.SwitchControllerInit();
  ledController.LedControllerInit();
  ultraSonicController.UltraSonicControllerInit();
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

void loop() {
  // put your main code here, to run repeatedly:
  test_all_controls();
  delay(50);
  ReturnSonicDistance(20);
}
