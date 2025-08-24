#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <GyverButton.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include "config.h"
#include "wifi_manager.h"
#include "time_manager.h"
#include "logger.h"
#include "SmoothStrip.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
GButton btn(BTN_PIN);
AsyncWebServer server(80);
SmoothStrip smooth(strip, MAX_BRIGHTNESS_LED);

tm TimeOff; // время в которое лента будет выключена
bool isOn = false;

void setup() {
  Serial.begin(115200);
  delay(100);

  smooth.begin();

  // Запускаем файловую систему
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }

  pinMode(SENSOR_PIN, INPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  
  initWiFi(ssid, password);
  initTime(gmtOffset_sec, daylightOffset_sec);

  // Отдаём статические файлы из папки /site
  server.serveStatic("/", LittleFS, "/site/")
    .setDefaultFile("index.html");

  // Обработка корневой страницы
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "site/index.html", "text/html");
  });
  server.begin(); // Запуск сервера
  
  // приветсвенное мигание
  smooth.ON(strip.Color(050, 162, 252), 1000, EASE_IN);
  while (smooth.isFading()) { smooth.update(); delay(10); }
  
  smooth.OFF(1000, EASE_OUT);
  while (smooth.isFading()) { smooth.update(); delay(10); }
  
  print("<===START===>");
}

void loop() {
  if (!updateTime()) return;

  btn.tick();

  if (IsInInterval(currentTime, StartTime, EndTime)) {
    if (digitalRead(SENSOR_PIN) == HIGH) {
      TimeOff = currentTime + glowTime;
      smooth.ON(strip.Color(255, 255, 255), 500, EASE_IN);
      print("STRIP ON");
    }
    else if (currentTime == TimeOff) {
      smooth.OFF(500, EASE_OUT);
      print("STRIP OFF");
    }
  }
  
  smooth.update();
} 