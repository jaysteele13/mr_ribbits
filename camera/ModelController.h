#ifndef MODELCONTROLLER_H
#define MODELCONTROLLER_H

#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "esp32-hal-ledc.h"
#include "sdkconfig.h"
#include "camera_index.h"
#include <vector>
#include "human_face_detect_msr01.hpp"
#include "face_recognition_tool.hpp"
#include "face_recognition_112_v1_s8.hpp"

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
  SmoothingFilter *SmoothingFilterInit(SmoothingFilter *filter, size_t sample_size);
  esp_err_t HeaderAvailableStatus(httpd_req_t *req, char **obuf);

  //  ------------ Functions - End -------------------

 

  // ---------- Variable ---------------

  int8_t detection_enabled = 0;
  SmoothingFilter smoothingFilter;

  // ------ Variable end --------------



  // Face Define Detection variables
  #define CONFIG_ESP_FACE_RECOGNITION_ENABLED 1
  #define CONFIG_ESP_FACE_DETECT_ENABLED 1

  // Face Id Config
  #define QUANT_TYPE 0  //if set to 1 => very large firmware, very slow, reboots when streaming...
  #define ENROLL_CONFIRM_TIMES 5
  #define FACE_ID_SAVE_NUMBER 7

  // Colors
  #define FACE_COLOR_WHITE 0x00FFFFFF
  #define FACE_COLOR_BLACK 0x00000000
  #define FACE_COLOR_RED 0x000000FF
  #define FACE_COLOR_GREEN 0x0000FF00
  #define FACE_COLOR_BLUE 0x00FF0000
  #define FACE_COLOR_YELLOW (FACE_COLOR_RED | FACE_COLOR_GREEN)
  #define FACE_COLOR_CYAN (FACE_COLOR_BLUE | FACE_COLOR_GREEN)
  #define FACE_COLOR_PURPLE (FACE_COLOR_BLUE | FACE_COLOR_RED)

};

#endif