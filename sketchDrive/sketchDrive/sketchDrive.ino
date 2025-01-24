#include "Autopilot.h"
#include "SwitchController.h"
#include <SoftwareSerial.h>

// main vars
SwitchController switchController;
LedController ledController;
AutoPilot autoPilot;

void setup() {
  // Serial.begin(9600);
  // mySerial.begin(9600); // For communication with ESP32

  autoPilot.SetAutoPilot(false);
  autoPilot.Init();
  delay(1000);
}

void loop() {

    //recieve and communicate data
    // if (mySerial.available()) {
    //     String data = mySerial.readString();
    //     Serial.println("Received from ESP32: " + data);
    // }

    // if (Serial.available()) {
    //     String data = Serial.readString();
    //     mySerial.println(data); // Send data to ESP32
    // }

    // How to Enable AutoPilot
    if(!switchController.isSwitchActive()) 
    {
      autoPilot.SetAutoPilot(true);
    }
   
    // General Lazy AutoPilot
    autoPilot.Roam();

}
