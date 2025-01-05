#define CAMERA_MODEL_ESP32S3_EYE

#include "CameraServerController.h"
#include "esp_system.h"

void CameraServerController::Init(void) {

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

  // init with high specs to pre-allocate larger buffers
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
    #if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
    #endif
  }

  // camera initial
  esp_err_t err = esp_camera_init( & config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();

  if (s -> id.PID == OV3660_PID) {
    s -> set_vflip(s, 1); // flip it back
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

  #if defined(CAMERA_MODEL_ESP32S3_EYE)
  s -> set_vflip(s, 1);
  s -> set_hmirror(s, 1);
  #endif

  // Configure Wifi
  uint64_t chipid = ESP.getEfuseMac();
  char string[10];
  sprintf(string, "%04X", (uint16_t)(chipid >> 32));
  String mac0_default = String(string);
  sprintf(string, "%08X", (uint32_t) chipid);
  String mac1_default = String(string);
  String url = password + mac0_default + mac1_default;
  const char * mac_default = url.c_str();

  Serial.println(":----------------------------:");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.println(":----------------------------:");
  wifi_name = mac0_default + mac1_default;

  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, 13);
  app.startCameraApp();

  Serial.println("Camera Ready!");

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