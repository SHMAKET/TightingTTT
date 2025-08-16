#pragma once
#include <time.h>
#include <Arduino.h>
#include "config.h"

extern struct tm currentTime;

void initTime(const char* ntpServer, long gmtOffset_sec, int daylightOffset_sec);
bool updateTime();
String TimeToString(const tm &timeStruct);