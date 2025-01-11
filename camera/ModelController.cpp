
#include "ModelController.h"

// Helper function to print text to frame
void ModelController::printToFrameHelper(fb_data_t *fb, uint32_t color, const char *str) {
  fb_gfx_print(fb, (fb->width - (strlen(str) * 14)) / 2, 10, color, str);
}

// function draws text to the jpeg captured
int ModelController::printToFrame(fb_data_t *fb, uint32_t color, const char *format, ...) {
  char loc_buf[64]; // allocating buffer memory.
  char *temp = loc_buf;
  int len;
  va_list arg;
  va_list copy;
  va_start(arg, format);
  va_copy(copy, arg);
  len = vsnprintf(loc_buf, sizeof(loc_buf), format, arg);
  va_end(copy);
  if (len >= sizeof(loc_buf)) {
    temp = (char *)malloc(len + 1);
    if (temp == NULL) {
      return 0;
    }
  }
  vsnprintf(temp, len + 1, format, arg);
  va_end(arg);
  printToFrameHelper(fb, color, temp);
  if (len > 64) {
    free(temp);
  }
  return len;
}

// Smooths and allocates memory for a better streaming experience
SmoothingFilter* ModelController::SmoothingFilterInit(SmoothingFilter *filter, size_t sample_size) {
  memset(filter, 0, sizeof(SmoothingFilter));

  filter->values = (int *)malloc(sample_size * sizeof(int));
  if (!filter->values) {
    return NULL;
  }
  memset(filter->values, 0, sample_size * sizeof(int));

  filter->size = sample_size;
  return filter;
}

// Returns status if URI header is active.
esp_err_t ModelController::HeaderAvailableStatus(httpd_req_t *req, char **obuf) {
  char *buf = NULL;
  size_t buf_len = 0;

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char *)malloc(buf_len);
    if (!buf) {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      *obuf = buf;
      return ESP_OK;
    }
    free(buf);
  }
  httpd_resp_send_404(req);
  return ESP_FAIL;
}



// Function to draw boxes on faces
void ModelController::DrawBoxesOnFaces(fb_data_t *fb, std::list<dl::detect::result_t> *results, int face_id) {
  // results is imported model, of which it can predict, this function is purely to draw boxes

  int x, y, w, h; // dimensions and coords
  uint32_t color = FACE_COLOR_YELLOW;
  if (face_id < 0) {
    color = FACE_COLOR_RED;
  } else if (face_id > 0) {
    color = FACE_COLOR_GREEN;
  }
  if (fb->bytes_per_pixel == 2) {
    //color = ((color >> 8) & 0xF800) | ((color >> 3) & 0x07E0) | (color & 0x001F);
    color = ((color >> 16) & 0x001F) | ((color >> 3) & 0x07E0) | ((color << 8) & 0xF800);
  }
  int i = 0;
  // parse results which are models predictions
  for (std::list<dl::detect::result_t>::iterator prediction = results->begin(); prediction != results->end(); prediction++, i++) {
    // rectangle boxes, per shape, coords and boxes are given to where in the image
    x = (int)prediction->box[0];
    y = (int)prediction->box[1];
    w = (int)prediction->box[2] - x + 1;
    h = (int)prediction->box[3] - y + 1;

    // Adjust box if out of bounds
    if ((x + w) > fb->width) {
      w = fb->width - x;
    }
    if ((y + h) > fb->height) {
      h = fb->height - y;
    }

    // built in function to draw line into frame where face may be detected.
    fb_gfx_drawFastHLine(fb, x, y, w, color);
    fb_gfx_drawFastHLine(fb, x, y + h - 1, w, color);
    fb_gfx_drawFastVLine(fb, x, y, h, color);
    fb_gfx_drawFastVLine(fb, x + w - 1, y, h, color);
  }
}

// Here we use results to run face recognition, have we processed this face yet?
// Takes image buffer and model results. If face not recognised, run model.
int ModelController::RunFaceRecognition(fb_data_t *fb, std::list<dl::detect::result_t> *results) {
  // Retrieve landmarks from initial results trying to find if this is id is registered
  std::vector<int> landmarks = results->front().keypoint; 
  int id = -1;

  Tensor<uint8_t> tensor;
  // Turning image into tensor object for model to work with
  tensor.set_element((uint8_t *)fb->data).set_shape({ fb->height, fb->width, 3 }).set_auto_free(false); 

  int enrolled_count = recognizer.get_enrolled_id_num(); // chexks how many people have been identified thus far

  // sets face id if not a current one
  if (enrolled_count < FACE_ID_SAVE_NUMBER && is_enrolling) {
    id = recognizer.enroll_id(tensor, landmarks, "", true);

    // Print to frame
    printToFrame(fb, FACE_COLOR_CYAN, "Person [%u]", id);
  }

  // function tries to recognise face -> using model
  face_info_t recognize = recognizer.recognize(tensor, landmarks);
  if (recognize.id >= 0) {
    printToFrame(fb, FACE_COLOR_GREEN, "Person [%u]: %.2f", recognize.id, recognize.similarity);
  } else {
    printToFrameHelper(fb, FACE_COLOR_RED, "Glimble Alert!");
  }
  return recognize.id;
}

// HTML Handler
esp_err_t ModelController::html_handler(httpd_req_t *req) {
    // HTML with a toggle button
    const char* html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
        <style>
            body {
                background-color: #fdf1e6; /* Pastel peach */
                color: #333;
                display: flex;
                flex-direction: column;
                align-items: center;
                justify-content: center;
                height: 100vh;
                margin: 0;
                font-family: "Roboto", sans-serif;
            }
            h1 {
                font-size: 6rem;
                color: #6b705c; /* Pastel green */
            }
            #stream {
                width: 800px;
                height: 800px;
                border-radius: 8px;
                box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            }
            #ribit {
                width: 40px;
                border-radius: 2px;
            }
            #toggleButton {
                margin-top: 20px;
                padding: 10px 20px;
                background-color: #6b705c;
                border: none;
                border-radius: 4px;
                cursor: pointer;
                font-size: 2rem;
                width: 3rem;
                height: 3rem;
            }
        </style>
        <title>Mr Ribits Eyeballs</title>
    </head>
    <body>
        <img id="ribit" src="data:image/jpeg;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAALBAMAAACNJ7BwAAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAC1QTFRFLU9xlJ6lk5aTkpN4O1t8AAAAucbQIUt1hYVeTm6PCDhokJGRd4eWubu7kqW4VMUejQAAAA90Uk5Tz/////sADff/5/9a9/+75ZA/MgAAAHZJREFUeJxjeFk5pfzMmXJP93kM5bJ33ZWUVt69uIChxqJD0mXKxI6mAwwTm5UNy92FjSwkGQ4ZKRuuWiWspCHJMKlJ2WZVFVBUgKFGo1m9vNzRotGBweWQjpeLyxIlnUqGlwxQMI8hDQh27waRDKFAsHs3iAQAfHkpSLCBfwsAAAAASUVORK5CYII=" alt="Lil man balls">
        <h1 class="text-monospace font-weight-light font-italic">Ribits View</h1>
        <img id="stream" src="/stream" alt="Live Stream from ESP32 Camera" />
        <button id="toggleButton">Enroll Face</button>

        <script>
            const button = document.getElementById('toggleButton');

            button.addEventListener('click', () => {
                fetch('/toggle', {
                    method: 'POST'
                })
                .then(response => response.json())
                .then(data => {
                    console.log('Current state:', data.state);
                })
                .catch(error => console.error('Error:', error));
            });
        </script>
    </body>
    </html>)rawliteral";

    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, html, strlen(html));
}


esp_err_t ModelController::toggle_handler(httpd_req_t *req) {
    ModelController instance;
    bool enroll = instance.is_enrolling;
    enroll = !enroll;

    char response[32];
    snprintf(response, sizeof(response), "{\"state\": %s}", true ? "true" : "false");
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, strlen(response));

    return ESP_OK;
}














esp_err_t ModelController::stream_handler(httpd_req_t *req) {
  ModelController instance;

  // Cam Variables
  camera_fb_t *fb = NULL;
  struct timeval _timestamp;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t *_jpg_buf = NULL;
  char *part_buf[64];


  // Stream Variables
  #define PART_BOUNDARY "123456789000000000000987654321"
  const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
  const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
  const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\nX-Timestamp: %d.%06d\r\n\r\n";

#if CONFIG_ESP_FACE_DETECT_ENABLED
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
  // Timing Variables
  bool detected = false;
  int64_t fr_ready = 0;
  int64_t fr_recognize = 0;
  int64_t fr_encode = 0;
  int64_t fr_face = 0;
  int64_t fr_start = 0;
#endif
  // Face ID variables
  int face_id = 0;
  size_t out_len = 0, out_width = 0, out_height = 0;
  uint8_t *out_buf = NULL;
  bool s = false;
#if TWO_STAGE
  HumanFaceDetectMSR01 s1(0.1F, 0.5F, 10, 0.2F);
  HumanFaceDetectMNP01 s2(0.5F, 0.3F, 5);
#else
  HumanFaceDetectMSR01 s1(0.3F, 0.5F, 10, 0.2F);
#endif
#endif

  static int64_t last_frame = 0;
  if (!last_frame) {
    last_frame = esp_timer_get_time();
  }

  // Initialise Streaming
  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK) {
    return res;
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  httpd_resp_set_hdr(req, "X-Framerate", "30");


  while (true) {
#if CONFIG_ESP_FACE_DETECT_ENABLED
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
    detected = false;
#endif
    face_id = 0;
#endif
    // Get Frame Buffer
    fb = esp_camera_fb_get();
    if (!fb) {
      log_e("Camera capture failed");
      res = ESP_FAIL;
    } else {
      // Time stamps to track logs
      _timestamp.tv_sec = fb->timestamp.tv_sec;
      _timestamp.tv_usec = fb->timestamp.tv_usec;
#if CONFIG_ESP_FACE_DETECT_ENABLED
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
      fr_start = esp_timer_get_time();
      fr_ready = fr_start;
      fr_encode = fr_start;
      fr_recognize = fr_start;
      fr_face = fr_start;
#endif
      if (fb->width > 400) { // Checking how big screen is
#endif
        if (fb->format != PIXFORMAT_JPEG) {
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if (!jpeg_converted) {
            log_e("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
#if CONFIG_ESP_FACE_DETECT_ENABLED
      } else {
        // IF RGB565!
        if (fb->format == PIXFORMAT_RGB565
        ) {
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
          fr_ready = esp_timer_get_time();
#endif
#if TWO_STAGE
          std::list<dl::detect::result_t> &candidates = s1.infer((uint16_t *)fb->buf, { (int)fb->height, (int)fb->width, 3 });
          std::list<dl::detect::result_t> &results = s2.infer((uint16_t *)fb->buf, { (int)fb->height, (int)fb->width, 3 }, candidates);
#else
          std::list<dl::detect::result_t> &results = s1.infer((uint16_t *)fb->buf, { (int)fb->height, (int)fb->width, 3 });
#endif
#if CONFIG_ESP_FACE_DETECT_ENABLED && ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
          fr_face = esp_timer_get_time();
          fr_recognize = fr_face;
#endif
          if (results.size() > 0) {
            fb_data_t rfb;
            rfb.width = fb->width;
            rfb.height = fb->height;
            rfb.data = fb->buf;
            rfb.bytes_per_pixel = 2;
            rfb.format = FB_RGB565;
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
            detected = true;
#endif
            #if CONFIG_ESP_FACE_RECOGNITION_ENABLED
                  face_id = instance.RunFaceRecognition(&rfb, &results);
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
                  fr_recognize = esp_timer_get_time();
#endif
                
#endif
            instance.DrawBoxesOnFaces(&rfb, &results, face_id);
          }

          // Why do this code
          s = fmt2jpg(fb->buf, fb->len, fb->width, fb->height, PIXFORMAT_RGB565, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if (!s) {
            log_e("fmt2jpg failed");
            res = ESP_FAIL;
          }
#if CONFIG_ESP_FACE_DETECT_ENABLED && ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
          fr_encode = esp_timer_get_time();
#endif
        } else {
          out_len = fb->width * fb->height * 3;
          out_width = fb->width;
          out_height = fb->height;
          out_buf = (uint8_t *)malloc(out_len);
          if (!out_buf) {
            log_e("out_buf malloc failed");
            res = ESP_FAIL;
          } else {
            s = fmt2rgb888(fb->buf, fb->len, fb->format, out_buf);
            esp_camera_fb_return(fb);
            fb = NULL;
            if (!s) {
              free(out_buf);
              log_e("To rgb888 failed");
              res = ESP_FAIL;
            } else {
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
              fr_ready = esp_timer_get_time();
#endif

              fb_data_t rfb;
              rfb.width = out_width;
              rfb.height = out_height;
              rfb.data = out_buf;
              rfb.bytes_per_pixel = 3;
              rfb.format = FB_BGR888;

#if TWO_STAGE
              std::list<dl::detect::result_t> &candidates = s1.infer((uint8_t *)out_buf, { (int)out_height, (int)out_width, 3 });
              std::list<dl::detect::result_t> &results = s2.infer((uint8_t *)out_buf, { (int)out_height, (int)out_width, 3 }, candidates);
#else
              std::list<dl::detect::result_t> &results = s1.infer((uint8_t *)out_buf, { (int)out_height, (int)out_width, 3 });
#endif

#if CONFIG_ESP_FACE_DETECT_ENABLED && ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
              fr_face = esp_timer_get_time();
              fr_recognize = fr_face;
#endif

              if (results.size() > 0) {
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
                detected = true;
#endif
#if CONFIG_ESP_FACE_RECOGNITION_ENABLED
                  face_id = instance.RunFaceRecognition(&rfb, &results);
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
                  fr_recognize = esp_timer_get_time();
#endif
                
#endif
                instance.DrawBoxesOnFaces(&rfb, &results, face_id);
              }
              s = fmt2jpg(out_buf, out_len, out_width, out_height, PIXFORMAT_RGB888, 90, &_jpg_buf, &_jpg_buf_len);
              free(out_buf);
              if (!s) {
                log_e("fmt2jpg failed");
                res = ESP_FAIL;
              }
#if CONFIG_ESP_FACE_DETECT_ENABLED && ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
              fr_encode = esp_timer_get_time();
#endif
            }
          }
        }
      }
#endif
    }
    // Streaming here
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if (res == ESP_OK) {
      size_t hlen = snprintf((char *)part_buf, 128, _STREAM_PART, _jpg_buf_len, _timestamp.tv_sec, _timestamp.tv_usec);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if (_jpg_buf) {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if (res != ESP_OK) {
      log_e("Send frame failed");
      break;
    }
    int64_t fr_end = esp_timer_get_time();

#if CONFIG_ESP_FACE_DETECT_ENABLED && ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
    int64_t ready_time = (fr_ready - fr_start) / 1000;
    int64_t face_time = (fr_face - fr_ready) / 1000;
    int64_t recognize_time = (fr_recognize - fr_face) / 1000;
    int64_t encode_time = (fr_encode - fr_recognize) / 1000;
    int64_t process_time = (fr_encode - fr_start) / 1000;
#endif

    int64_t frame_time = fr_end - last_frame;
    frame_time /= 1000;
#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
    uint32_t avg_frame_time = ra_filter_run(&ra_filter, frame_time);
#endif
    log_i("MJPG: %uB %ums (%.1ffps), AVG: %ums (%.1ffps)"
#if CONFIG_ESP_FACE_DETECT_ENABLED
          ", %u+%u+%u+%u=%u %s%d"
#endif
          ,
          (uint32_t)(_jpg_buf_len),
          (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time,
          avg_frame_time, 1000.0 / avg_frame_time
#if CONFIG_ESP_FACE_DETECT_ENABLED
          ,
          (uint32_t)ready_time, (uint32_t)face_time, (uint32_t)recognize_time, (uint32_t)encode_time, (uint32_t)process_time,
          (detected) ? "DETECTED " : "", face_id
#endif
    );
  }

  return res;
}

static SmoothingFilter smoothingFilter;

void ModelController::startCameraApp() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG(); //  A structure containing configuration parameters for the HTTP server, such as server task priority, stack size, and port.
    config.max_uri_handlers = 8; // can handle 8 uri paths like /stream, /jpeg
    httpd_handle_t stream_httpd = NULL; 

     // Register the HTML handler
    httpd_uri_t html_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = html_handler,
        .user_ctx = NULL
    };

    // define general stream handler
    httpd_uri_t stream_uri = {
        .uri = "/stream",
        .method = HTTP_GET,
        .handler = stream_handler,
        .user_ctx = NULL
    };

    httpd_uri_t toggleuri = {
        .uri = "/toggle",
        .method = HTTP_GET,
        .handler = toggle_handler,
        .user_ctx = NULL
    };

    SmoothingFilterInit(&smoothingFilter, 20);

    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &html_uri);
        httpd_register_uri_handler(stream_httpd, &stream_uri);
    }
}




