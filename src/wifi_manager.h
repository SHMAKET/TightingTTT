#pragma once
#include <Arduino.h>

void initWiFi(const char* ssid, const char* password);
bool isWiFiConnected();