#include <avr/wdt.h>

#include "Autopilot.h"
#include "SwitchController.h"

// main vars
SwitchController switchController;
LedController ledController;
AutoPilot autoPilot;

void setup() {
  Serial.begin(9600);
  autoPilot.SetAutoPilot(false);
  autoPilot.Init();
  delay(1000);
}

void loop() {

    // How to Enable AutoPilot
    if(!switchController.isSwitchActive()) 
    {
      autoPilot.SetAutoPilot(true);
    }
   
    // General Lazy AutoPilot
    autoPilot.Roam();

}
