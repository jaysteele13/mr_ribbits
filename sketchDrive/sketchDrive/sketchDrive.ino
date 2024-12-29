#include <avr/wdt.h>
#include "driveMotor.h"
#include "SwitchController.h"
#include "LedController.h"
#include "functioniseParts.cpp"

MotorDriver moterDriver;
SwitchController switchController;
LedController ledController;
//extern Application application

void setup() {
  motorDriver.MotorDriverInit();
  switchController.SwitchControllerInit();
  ledController.LedControllerInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!switchController.isSwitchActive()) 
  {
    // Do directions
    delay(50);
    ControlBot(DirectionControl::Forward, 100);
    delay(2000);
    ControlBot(DirectionControl::Forward, 100);
    ControlBot(DirectionControl::Backward, 200);
    delay(1000);
    ControlBot(DirectionControl::LeftBackward, 100);
    delay(4000);
    ControlBot(DirectionControl::stop_it, 0);
    delay(2000);
  } 
  else 
  {
    // turn led light on have motors deactivated
    ledController.SetAndEnableRGB();
    ControlBot(DirectionControl::stop_it, 0);
  }
  
  // if button is pressed start driving sequence wait 5 seconds after done flashing led light when waiting

  // start with start when button is pressed


}
