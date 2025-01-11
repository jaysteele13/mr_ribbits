#include "CameraApplication.h"

// HTML Handler
esp_err_t Camera_Application::html_handler(httpd_req_t *req) {
    // -------- Manage HTML Page -> Would rather have in sperate file, this is good enough ---------
    const char* html = R"rawliteral(<!DOCTYPE html>
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
    </style>
    <title>Mr Ribits Eyeballs</title>
</head>
<body>
    <img id="ribit" src="data:image/jpeg;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAALBAMAAACNJ7BwAAAAAXNSR0IB2cksfwAAAAlwSFlzAAALEwAACxMBAJqcGAAAAC1QTFRFLU9xlJ6lk5aTkpN4O1t8AAAAucbQIUt1hYVeTm6PCDhokJGRd4eWubu7kqW4VMUejQAAAA90Uk5Tz/////sADff/5/9a9/+75ZA/MgAAAHZJREFUeJxjeFk5pfzMmXJP93kM5bJ33ZWUVt69uIChxqJD0mXKxI6mAwwTm5UNy92FjSwkGQ4ZKRuuWiWspCHJMKlJ2WZVFVBUgKFGo1m9vNzRotGBweWQjpeLyxIlnUqGlwxQMI8hDQh27waRDKFAsHs3iAQAfHkpSLCBfwsAAAAASUVORK5CYII=" alt="Lil man balls">
    <h1 class="text-monospace font-weight-light font-italic">Ribits View</h1>
    <img id="stream" src="/stream" alt="Live Stream from ESP32 Camera" />
</body>
</html>)rawliteral";

    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, html, strlen(html));
}

esp_err_t Camera_Application::stream_handler(httpd_req_t *req) {
    // ----------------------- Manage Camera Stream, buffering and configuration -------------------------
    // httpd_req_t *req is a pointer to the HTTP request object.
    // esp_err_t is a type representing error codes returned by ESP functions.

    camera_fb_t *fb = NULL; // A pointer to a frame buffer structure that will hold the captured image.
    esp_err_t res = ESP_OK; // Holds the result of ESP function calls.
    size_t _jpg_buf_len = 0; // Length of the JPEG buffer
    uint8_t *_jpg_buf = NULL; // Pointer to JPEG buffer
    char part_buf[64]; // Buffer used to format the HTTP response headers.

    // Sets the response type to multipart which is used for streaming JPEG
    res = httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");
    if (res != ESP_OK) {
        return res; // If setting this type fails return an error.
    }

    while (true) {
        fb = esp_camera_fb_get(); // Captures a frame from the camera
        if (!fb) {
            Serial.println("Camera capture failed");
            res = ESP_FAIL; // Return error if capture doesn't happen.
        } else {
            if (fb->format != PIXFORMAT_JPEG) {
                // if image not in JPEG format we convert, this could be expensive so dk if this is any good.
                bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
                esp_camera_fb_return(fb);
                fb = NULL;
                if (!jpeg_converted) {
                    Serial.println("JPEG compression failed");
                    res = ESP_FAIL;
                }
            } else {
                // assign the frame buffer length
                _jpg_buf_len = fb->len;
                _jpg_buf = fb->buf;
            }
        }

        if (res == ESP_OK) {
            // Setting the frame boundary
            res = httpd_resp_send_chunk(req, "--frame\r\n", strlen("--frame\r\n")); // Sends the frame as part of the HTTP response in chunks.

            // Specifies and sends headers for data and frames, including the type and lengths
            snprintf(part_buf, sizeof(part_buf), "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", _jpg_buf_len);
            res = httpd_resp_send_chunk(req, part_buf, strlen(part_buf));

            // Send JPEG data to the client
            res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);

            // Send the frame end.
            res = httpd_resp_send_chunk(req, "\r\n", strlen("\r\n"));
        }

        // Frees the frame buffer or JPEG buffer.
        // Breaks the loop if there is an error.
        if (fb) {
            esp_camera_fb_return(fb);
            fb = NULL;
            _jpg_buf = NULL;
        } else if (_jpg_buf) {
            free(_jpg_buf);
            _jpg_buf = NULL;
        }
        if (res != ESP_OK) {
            break;
        }

        delay(10); // Small delay to control frame rate
        yield();    // Yield to reset the watchdog timer
    }

    return res;
}

static SmoothingFilter smoothingFilter;

void Camera_Application::startCameraApp() {
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

    modelController.SmoothingFilterInit(&smoothingFilter, 20);

    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &html_uri);
        httpd_register_uri_handler(stream_httpd, &stream_uri);
    }
}
