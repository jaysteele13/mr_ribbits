#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "esp32-hal-ledc.h"
#include "esp32-hal-log.h"
#include "sdkconfig.h"
#include "camera_index.h"
#include <vector>
// Models
#include "human_face_detect_msr01.hpp"
#include "human_face_detect_mnp01.hpp"
#include "face_recognition_tool.hpp"
#include "face_recognition_112_v1_s8.hpp"
#include <Arduino.h>


    // ------------- Structs --------------
typedef struct
{
  size_t size;   //number of values used for filtering
  size_t index;  //current value index
  size_t count;  //value count
  int sum;
  int *values;  //array to be filled with values
} SmoothingFilter;

// --------- Structs End ------------

class ModelController
{

public:

  // --------- Variables -----------
  FaceRecognition112V1S8 recognizer; // define library which uses model, to implement my own woould need ot understand how this one works

  // -------- Functions ------------
  SmoothingFilter *SmoothingFilterInit(SmoothingFilter *filter, size_t sample_size);
  void DrawBoxesOnFaces(fb_data_t *fb, std::list<dl::detect::result_t> *results, int face_id);
  int RunFaceRecognition(fb_data_t *fb, std::list<dl::detect::result_t> *results);
  void startCameraApp();
  static esp_err_t html_handler(httpd_req_t *req);
  static esp_err_t toggle_handler(httpd_req_t *req);

  // Temp
  static esp_err_t stream_handler(httpd_req_t *req);
  uint8_t GetDirection(int x, int w, fb_data_t *fb);

private:
// need
/*
draw_face_boxes

function to do with face detection

learn with streaming  function how to incorporate model

*/

  //  --------------- Functions -------------------

  void printToFrameHelper(fb_data_t *fb, uint32_t color, const char *str); 
  int printToFrame(fb_data_t *fb, uint32_t color, const char *format, ...);
  
  esp_err_t HeaderAvailableStatus(httpd_req_t *req, char **obuf);

  //  ------------ Functions - End -------------------

 

  // ---------- Variable ---------------

  int8_t detection_enabled = 0;
  bool is_enrolling = 0;
  SmoothingFilter smoothingFilter;
  bool recognition_enabled = true;

  // ------ Variable end --------------



  // Face Define Detection variables
  #define CONFIG_ESP_FACE_RECOGNITION_ENABLED 0 // what kind of person
  #define CONFIG_ESP_FACE_DETECT_ENABLED 1 // general face

  // Face Id Config
  #define QUANT_TYPE 0  //if set to 1 => very large firmware, very slow, reboots when streaming...
  #define ENROLL_CONFIRM_TIMES 5
  #define FACE_ID_SAVE_NUMBER 7
  #define TWO_STAGE 1 /*<! 1: detect by two-stage which is more accurate but slower(with keypoints). */

  // Colors
  #define FACE_COLOR_WHITE 0x00FFFFFF
  #define FACE_COLOR_BLACK 0x00000000
  #define FACE_COLOR_RED 0x000000FF
  #define FACE_COLOR_GREEN 0x0000FF00
  #define FACE_COLOR_BLUE 0x00FF0000
  #define FACE_COLOR_YELLOW (FACE_COLOR_RED | FACE_COLOR_GREEN)
  #define FACE_COLOR_CYAN (FACE_COLOR_BLUE | FACE_COLOR_GREEN)
  #define FACE_COLOR_PURPLE (FACE_COLOR_BLUE | FACE_COLOR_RED)

  // Streaming Counterpart
  // #define PART_BOUNDARY "123456789000000000000987654321"
  // const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
  // const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
  // const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\nX-Timestamp: %d.%06d\r\n\r\n";
  

};

#endif