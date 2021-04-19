#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>

#include "camera_pins.h"
#include "esp_camera.h"

// 左右前后拉高的io
const int ioL = 13;
const int ioR = 12;
const int ioF = 14;
const int ioB = 15;
long lastSetLRFB = 0;

void startCameraServer();

// 设置电机
void setLRFB(bool l, bool r, bool f, bool b) {
    digitalWrite(ioL, l);
    digitalWrite(ioR, r);
    digitalWrite(ioF, f);
    digitalWrite(ioB, b);
    if (l || r || f || b) {
        // 超时停车
        lastSetLRFB = millis() + 500;
    }
}
// 手机9键数字方向
void setLRFB(int t9) {
    switch (t9) {
        case 1:
            setLRFB(1, 0, 1, 0);
            break;
        case 2:
            setLRFB(0, 0, 1, 0);
            break;
        case 3:
            setLRFB(0, 1, 1, 0);
            break;
        case 4:
            setLRFB(1, 0, 0, 0);
            break;
        case 6:
            setLRFB(0, 1, 0, 0);
            break;
        case 7:
            setLRFB(1, 0, 0, 1);
            break;
        case 8:
            setLRFB(0, 0, 0, 1);
            break;
        case 9:
            setLRFB(0, 1, 0, 1);
            break;
        default:
            setLRFB(0, 0, 0, 0);
            break;
    }
}

void setup() {
    pinMode(ioL, OUTPUT);
    pinMode(ioR, OUTPUT);
    pinMode(ioF, OUTPUT);
    pinMode(ioB, OUTPUT);
    setLRFB(0, 0, 0, 0);

    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    WiFi.softAP("大块头", "20050803");

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
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
    //                      for larger pre-allocated frame buffer.
    if (psramFound()) {
        config.frame_size = FRAMESIZE_UXGA;
        config.jpeg_quality = 10;
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

#if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
#endif

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t* s = esp_camera_sensor_get();
    // 默认分辨率 保证流畅
    s->set_framesize(s, FRAMESIZE_VGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);
#endif

    // WiFi.begin(ssid, password);

    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(500);
    //     Serial.print(".");
    // }
    // Serial.println("");
    // Serial.println("WiFi connected");

    startCameraServer();

    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");

    // for (int i = 0; i <11;i++) {
    //   setLRFB(i);
    //   delay(1000);
    // }
}

void loop() {
    // 要是断连了要让它停下
    if (lastSetLRFB != 0) {
        if (lastSetLRFB < millis()) {
            lastSetLRFB = 0;
            setLRFB(0, 0, 0, 0);
        }
    }
    delay(100);
}