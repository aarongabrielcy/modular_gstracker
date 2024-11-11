#include "ConfigStorage.h"

ConfigStorage::ConfigStorage() : serverIP("192.168.1.100"), serverPort(8080) {}

void ConfigStorage::begin() {
    preferences.begin("config", false);
    serverIP = preferences.getString("serverIP", serverIP);
    serverPort = preferences.getInt("serverPort", serverPort);
}

void ConfigStorage::saveConfig(const String& ip, int port) {
    serverIP = ip;
    serverPort = port;
    preferences.putString("serverIP", serverIP);
    preferences.putInt("serverPort", serverPort);
}

String ConfigStorage::getServerIP() {
    return serverIP;
}

int ConfigStorage::getServerPort() {
    return serverPort;
}
