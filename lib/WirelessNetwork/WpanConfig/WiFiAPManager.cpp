#include "WiFiAPManager.h"

void WiFiAPManager::startAP(const char* ssid, const char* password) {
    WiFi.softAP(ssid, password);
    Serial.println("Modo AP iniciado. Conéctese a SSID: " + String(ssid));
    Serial.print("Dirección IP AP: ");
    Serial.println(WiFi.softAPIP());
}
