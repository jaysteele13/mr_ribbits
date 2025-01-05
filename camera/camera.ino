#include "CameraServerController.h"
#include <WiFi.h>
#include "esp_camera.h"
WiFiServer server(100);

#define RXD2 3
#define TXD2 40

CameraServerController cameraServerController;

// NOTE: esp32 espressiv board must be at v.2.0.14 also here is how the tools should be configured:
//            USB CDC On Boot ---> Enabled
//            Flash Size ---> 8MB(64Mb)
//            Partition Scheme ---> 8M with spiffs (3MB APP/1.5MB SPIFFS)
//            PSRAM ---> OPI PSRAM

void setup()
{
  // Initialise Serials
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  // Initialise these pins (dk why but why but have too)
  pinMode(46, OUTPUT);
  digitalWrite(46, HIGH);

  // Initialise Cam Server and Application
  cameraServerController.Init();

  // Stabalisation
  delay(100);

  // Begin WiFi Server for Mr_Ribits
  server.begin();
}
void loop()
{
  // empty
}
