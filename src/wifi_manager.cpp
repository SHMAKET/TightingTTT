#include <ESP8266WiFi.h>
#include "wifi_manager.h"
#include "logger.h"

void initWiFi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    print("Connecting to <" + String(ssid) + ">");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("[NoTime] WiFi connected.");
    Serial.println("[NoTime] IP addres: " + WiFi.localIP().toString());
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}