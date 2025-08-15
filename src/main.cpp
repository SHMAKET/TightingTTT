#include <Arduino.h>
#include <ESP8266WiFi.h>

#pragma region  properties

// Пины
#define SENSOR_PIN      D5        // Датчик движения RCWL-0516
#define LED_PIN         D2        // WS2812
#define BTN_PIN         D1        // Кнопка (подтянута к питанию через INPUT_PULLUP)

// Количество светодиодов
#define NUM_LEDS 1

// Задайте свои данные Wi-Fi
const char* ssid        = "karbafos";
const char* password    = "Audi80b4avant";

const long gmtOffset_sec = 3 * 3600;    // Часовой пояс в секундах (например, Москва UTC+3 = 3*3600 = 10800)
const int daylightOffset_sec = 0;       // Коррекция летнего времени (0 если не используется)

#pragma endregion
#pragma region function definition

void INIT_WIFI();
void INIT_NTP();
tm GetTime();

#pragma endregion

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  
  Serial.begin(115200);
  delay(100);
  
  INIT_WIFI();
  INIT_NTP();
}

void loop() {
  bool buttonPressed = digitalRead(BTN_PIN) == LOW;     // Нажата ли кнопка?

  if (buttonPressed) {
    GetTime();
    delay(300);
  }
}

tm GetTime() {
  time_t now = time(nullptr);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);

  // Формируем строку времени
  char timeString[128];
  snprintf(timeString, sizeof(timeString), "Получено время: %02d:%02d:%02d/%04d-%02d-%02d",
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec,
            timeinfo.tm_year + 1900,
            timeinfo.tm_mon + 1,
            timeinfo.tm_mday);

  Serial.println(timeString);

  return timeinfo;
}

void INIT_WIFI() {
  // Подключаемся к Wi-Fi
  Serial.printf("Подключение к %s", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi подключен");
}

void INIT_NTP() {
  // Настройка времени через NTP
  configTime(gmtOffset_sec, daylightOffset_sec,  "pool.ntp.org", "time.nist.gov");

  Serial.println("Ожидание синхронизации времени...");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println();
  Serial.println("Время синхронизировано!");
}