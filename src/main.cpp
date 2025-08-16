#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <GyverButton.h>

#include "config.h"
#include "wifi_manager.h"
#include "time_manager.h"
#include "logger.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
GButton btn(BTN_PIN);
GButton sensor(SENSOR_PIN);

void waveFromCenter(uint32_t color, int delayTime);

bool LedsIsOn;

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS_LED);

  pinMode(SENSOR_PIN, INPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  
  Serial.begin(115200);
  delay(100);

  initWiFi(ssid, password);
  initTime("pool.ntp.org", gmtOffset_sec, daylightOffset_sec);

  print("<===START===>");
  waveFromCenter(strip.Color(0, 255, 0), 100); // волна голубая
  LedsIsOn = false;
}

void loop() {
  btn.tick();
  sensor.tick();
  
  strip.show();
}



void waveFromCenter(uint32_t baseColor, int delayTime) {
  int left = (LED_COUNT / 2) - 1;   // левая середина (21)
  int right = (LED_COUNT / 2);      // правая середина (22)

  for (int step = 0; step <= LED_COUNT / 2 + 5; step++) {
    strip.clear();
    for (int offset = 0; offset < 5; offset++) {
      // коэффициент яркости (от 1.0 в центре до 0.2 на краю)
      float brightness = 1.0 - (offset * 0.2);
      // вытащим R,G,B
      uint8_t r = (uint8_t)(((baseColor >> 16) & 0xFF) * brightness);
      uint8_t g = (uint8_t)(((baseColor >> 8) & 0xFF) * brightness);
      uint8_t b = (uint8_t)((baseColor & 0xFF) * brightness);
      // позиция слева
      int posL = left - step - offset;
      if (posL >= 0) {
        strip.setPixelColor(posL, strip.Color(r, g, b));
      }
      // позиция справа
      int posR = right + step + offset;
      if (posR < LED_COUNT) {
        strip.setPixelColor(posR, strip.Color(r, g, b));
      }
    }
    strip.show();
    delay(delayTime);
  }
  delay(500);     // пауза после волны
  strip.clear();  // погасить
  strip.show();
}

