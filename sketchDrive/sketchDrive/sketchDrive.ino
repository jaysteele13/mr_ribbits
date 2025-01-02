#include <avr/wdt.h>
#include "driveMotor.h"
#include "SwitchController.h"
#include "LedController.h"
#include "UltraSonicController.h"
#include "BuzzerController.h"
#include "EdgeDetectionController.h"
#include "Autopilot.h"

// main vars
MotorDriver motorDriver;
SwitchController switchController;
LedController ledController;
UltraSonicController ultraSonicController;
MiniServoController miniServoController;
BuzzerController buzzerController;
EdgeDetectionController edgeDetectionController;
AutoPilot autoPilot;
bool enableAuto = true;


void setup() {
  Serial.begin(9600);
  autoPilot.SetAutoPilot(true);
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
    enableAuto = !enableAuto;
  }

  if(enableAuto)
  // if()
  {
    ledController.SetAndEnableRGB(CRGB::DarkGreen);
    //
    autoPilot.Roam();
  }
  else 
  {
    ledController.SetAndEnableRGB();
    autoPilot.SetAutoPilot(false);
  }
  // test_all_controls();
  // ReturnSonicDistance(20);
//  edgeDetectionController.ScanForEdges();
}
