#include "CameraServerController.h"
#include <WiFi.h>
#include "esp_camera.h"
#include "LittleFS.h"

/*

Thurs 9th:

- Bleed in some face detction logic from other file
- Try and test between every logic bled
- Have AI stuff prepped to show saugat, mainly model training approach and cuurent scores saved on notes


*/

WiFiServer server(100);

#define RXD2 3
#define TXD2 40

CameraServerController cameraServerController;

// NOTE: esp32 espressiv board must be at v.2.0.14 also here is how the tools should be configured:
//            USB CDC On Boot ---> Enabled
//            Flash Size ---> 8MB(64Mb)
//            Partition Scheme ---> 8M with spiffs (3MB APP/1.5MB SPIFFS)
//            PSRAM ---> OPI PSRAM

// void listFiles() {
//     Serial.println("Listing files:");
//     File root = LittleFS.open("/");
//     File file = root.openNextFile();
//     while (file) {
//         Serial.print("FILE: ");
//         Serial.println(file.name());
//         file = root.openNextFile();
//     }
// }


void setup()
{
  // Initialise Serials
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);


  // need this to get local files!
  // To send files go cmd shift p and type "Upload little FS/PICO..."
  // if (!LittleFS.begin()) {
  //   Serial.println("Failed to initialize LittleFS");
  //   return;
  // }

  // listFiles();

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
  // may need to have this in model controller
  if (Serial1.available()) {
        String data = Serial1.readString();
        Serial.println("Received from Arduino Uno: " + data);
    }

    if (Serial.available()) {
        String data = Serial.readString();
        Serial1.println(data); // Send data to Arduino Uno
    }
  // empty
}
