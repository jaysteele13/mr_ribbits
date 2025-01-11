#ifndef CAMERA_APPLICATION_H
#define CAMERA_APPLICATION_H

#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "esp32-hal-ledc.h"
#include "sdkconfig.h"
#include "camera_index.h"
#include <Arduino.h>
#include "ModelController.h"

// #if defined(ARDUINO_ARCH_ESP32) && defined(CONFIG_ARDUHAL_ESP_LOG)
// #include "esp32-hal-log.h" // may need when communicating with other arduino.
// #endif

class Camera_Application {
public:
  void startCameraApp();
private:
  static esp_err_t stream_handler(httpd_req_t *req); // Function declaration
  static esp_err_t html_handler(httpd_req_t *req); // Function declaration

  ModelController modelController;
};

#endif 
