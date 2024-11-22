#ifndef WIFIAPMANAGER_H
#define WIFIAPMANAGER_H

#include <WiFi.h>
#include <Arduino.h>

class WiFiAPManager {
public:
    void startAP(const char* ssid, const char* password);
};

#endif
