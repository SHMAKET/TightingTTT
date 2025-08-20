#include "logger.h"
#include "time_manager.h"
#include "wifi_manager.h"

void print(String mess) {
    static String lastMessage = "";

    static unsigned long lastTime = 0;
    unsigned long now = millis();
    if (mess == lastMessage && (now - lastTime) < 1000)
        return;

    lastMessage = mess;
    lastTime = now;

    String time = (isWiFiConnected() && updateTime()) ? 
        TimeToString(currentTime) : 
        "       NoTime      ";

    Serial.println("[" + time + "] " + mess);
}