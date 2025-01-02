#include <avr/wdt.h>

#include "Autopilot.h"
#include "BuzzerController.h"
#include "EdgeDetectionController.h"
#include "LedController.h"
#include "SwitchController.h"
#include "UltraSonicController.h"
#include "driveMotor.h"

// main vars
MotorDriver motorDriver;
SwitchController switchController;
LedController ledController;
UltraSonicController ultraSonicController;
MiniServoController miniServoController;
BuzzerController buzzerController;
EdgeDetectionController edgeDetectionController;
AutoPilot autoPilot;

void setup() {
  Serial.begin(9600);
  autoPilot.SetAutoPilot(false);
  edgeDetectionController.Init();
  motorDriver.MotorDriverInit();
  buzzerController.BuzzerControllerInit();
  switchController.SwitchControllerInit();
  ledController.LedControllerInit();
  ultraSonicController.UltraSonicControllerInit();
  miniServoController.MiniServoControllerInit();
}


void loop() {

    if(!switchController.isSwitchActive()) 
    {
      autoPilot.SetAutoPilot(true);
    }

    
    if(autoPilot.isActive) 
    {
      ledController.SetAndEnableRGB(CRGB::DarkGreen);
    }
    else 
    {
      ledController.SetAndEnableRGB();
      // ledController.LedBlink() have it blink when close;
    }
    autoPilot.Roam();

}
