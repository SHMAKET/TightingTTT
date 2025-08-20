#include "logger.h"
#include "time_manager.h"
#include "wifi_manager.h"

void print(String mess) {
    if (isWiFiConnected() && updateTime()){
        Serial.println("[" + TimeToString(currentTime) + "] " + mess);
    }
    else {
        Serial.println("[       NoTime      ]  " + mess);
    }
}