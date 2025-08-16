#pragma once

// Задайте свои данные Wi-Fi
inline const char* ssid        = "karbafos";
inline const char* password    = "Audi80b4avant";

// Пины
#define SENSOR_PIN      D1        // Датчик движения RCWL-0516
#define BTN_PIN         D2        // Кнопка (подтянута к питанию через INPUT_PULLUP)
#define LED_PIN         D4        // лента WS2812

// Количество светодиодов
#define LED_COUNT 44                // 136
#define BRIGHTNESS_LED 25          // Яркость (0-255)

const long gmtOffset_sec = 3 * 3600;    // Часовой пояс в секундах (например, Москва UTC+3 = 3*3600 = 10800)
const int daylightOffset_sec = 0;       // Коррекция летнего времени (0 если не используется)
static const unsigned long syncIntervalMs = 3600000; // 1 час = 3600*1000 мс интервал синхронизации времени с сервером