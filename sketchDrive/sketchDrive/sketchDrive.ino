#include <avr/wdt.h>

#include "Autopilot.h"
#include "SwitchController.h"
#include "MPU6050Controller.h"

// main vars
SwitchController switchController;
LedController ledController;
AutoPilot autoPilot;
MPU6050Controller mpu6050Controller;

void setup() {
  Serial.begin(9600);
  mpu6050Controller.Init();
  autoPilot.SetAutoPilot(false);
  autoPilot.Init();
  delay(1000);
}

void loop() {

  mpu6050Controller.GetTemperature();
  delay(500);
    //How to Enable AutoPilot
    // if(!switchController.isSwitchActive()) 
    // {
    //   autoPilot.SetAutoPilot(true);
    // }
   
    // // General Lazy AutoPilot
    // autoPilot.Roam();

}
