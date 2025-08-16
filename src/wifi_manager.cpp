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
    Serial.println("\nWiFi connected.");
}

bool isWiFiConnected() {
    return WiFi.status() == WL_CONNECTED;
}