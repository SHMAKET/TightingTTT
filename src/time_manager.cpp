#include "time_manager.h"
#include "logger.h"

struct tm currentTime; // глобальная переменная

// Внутренние переменные
static unsigned long lastSyncMillis = 0;   // время последней синхронизации (millis)
static time_t lastUnixTime = 0;            // время с сервера в секундах Unix

void initTime(const char* ntpServer, long gmtOffset_sec, int daylightOffset_sec) {
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    lastSyncMillis = 0; // чтобы первая синхронизация была сразу
    print("time: ok");   
}

bool updateTime() {
    unsigned long nowMillis = millis();

    // Если давно не синхронизировались — запрос к NTP
    if (nowMillis - lastSyncMillis >= syncIntervalMs || lastSyncMillis == 0) {
        if (!getLocalTime(&currentTime)) {
            print("Failed to obtain time");
            return false;
        }
        // Сохраняем время Unix (в секундах)
        lastUnixTime = mktime(&currentTime);
        lastSyncMillis = nowMillis;
        print("NTP sync done");
        return true;
    }
    
    // Иначе считаем время от последнего значения NTP
    unsigned long elapsedSec = (nowMillis - lastSyncMillis) / 1000;
    time_t simulatedTime = lastUnixTime + elapsedSec;
    localtime_r(&simulatedTime, &currentTime);

    return true;
}

String TimeToString(const tm &timeStruct) {
    char buf[60];
    snprintf(
        buf,
        sizeof(buf),
        "%02d:%02d:%02d %02d.%02d.%04d",
        timeStruct.tm_hour,
        timeStruct.tm_min,
        timeStruct.tm_sec,
        timeStruct.tm_mday,
        timeStruct.tm_mon + 1,
        timeStruct.tm_year + 1900
    );
    return String(buf);
}

inline int TimeToSeconds(const tm& t) {
    return t.tm_hour * 3600 + t.tm_min * 60 + t.tm_sec;
}

// Перегрузка операторов
bool operator<(const tm& a, const tm& b)  { return TimeToSeconds(a) <  TimeToSeconds(b); }
bool operator<=(const tm& a, const tm& b) { return TimeToSeconds(a) <= TimeToSeconds(b); }
bool operator>(const tm& a, const tm& b)  { return TimeToSeconds(a) >  TimeToSeconds(b); }
bool operator>=(const tm& a, const tm& b) { return TimeToSeconds(a) >= TimeToSeconds(b); }
bool operator==(const tm& a, const tm& b) { return TimeToSeconds(a) == TimeToSeconds(b); }
bool operator!=(const tm& a, const tm& b) { return !(a == b); }

bool IsInInterval(const tm& now, const tm& start, const tm& end) {
    int nowSec   = TimeToSeconds(now);
    int startSec = TimeToSeconds(start);
    int endSec   = TimeToSeconds(end);

    if (startSec <= endSec) {
        // интервал внутри одного дня
        return nowSec >= startSec && nowSec <= endSec;
    } else {
        // интервал переходит через полночь
        return nowSec >= startSec || nowSec <= endSec;
    }
}