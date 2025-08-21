#pragma once
#include <time.h>
#include <Arduino.h>
#include "config.h"

extern struct tm currentTime;

void initTime(long gmtOffset_sec, int daylightOffset_sec);
bool updateTime();
String TimeToString(const tm &timeStruct);
bool operator<(const tm& a, const tm& b);
bool operator<=(const tm& a, const tm& b);
bool operator>(const tm& a, const tm& b);
bool operator>=(const tm& a, const tm& b);
bool operator==(const tm& a, const tm& b);
bool operator!=(const tm& a, const tm& b);

bool IsInInterval(const tm& now, const tm& start, const tm& end);

tm operator+(const tm& a, const tm& b);