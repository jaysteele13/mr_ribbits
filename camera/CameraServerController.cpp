#define CAMERA_MODEL_ESP32S3_EYE

#include "CameraServerController.h"
#include "esp_system.h"

void CameraServerController::Init(void) {

  // Config
  Serial.setDebugOutput(true);
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  // config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  // Will work if tooling is set up correctly
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
  }

  // Initialise esp camera.
  esp_err_t err = esp_camera_init( & config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Grab camera sensor, this is configured as long as pins are correct.
  sensor_t * s = esp_camera_sensor_get();

  // Ensure camera see's as it should.
  if (s -> id.PID == OV3660_PID) {
    s -> set_brightness(s, 1); // up the brightness just a bit
    s -> set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    // s->set_framesize(s, FRAMESIZE_QVGA);
    s -> set_framesize(s, FRAMESIZE_SVGA);
    //  s->set_framesize(s, FRAMESIZE_SXGA);
    // s->set_framesize(s, FRAMESIZE_UXGA);
    // s->set_framesize(s, FRAMESIZE_QSXGA);

  }

  // Could probs add to a general config.
  #if defined(CAMERA_MODEL_ESP32S3_EYE)
  s -> set_vflip(s, 1);
  s -> set_hmirror(s, 1);
  #endif

  // Print IP
  Serial.println(":----------------------------:");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.println(":----------------------------:");

  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, 13);

  // Start web and camera app / operations
  app.startCameraApp();

  Serial.println("Camera Ready!");

  // make this more dynamic
  Serial.print("Use 'http://");
  Serial.print(WiFi.softAPIP());
  Serial.print("/stream");
  Serial.println("' to connect.\n");

  // WiFi.begin(ssid, password);

  // Alternativly try and host server on current wifi

  // Connect to Wi-Fi and create server on it
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("WiFi connected");
  // Serial.println("Go to link:");
  // Serial.print("http://:");
  // Serial.println(WiFi.localIP());

}