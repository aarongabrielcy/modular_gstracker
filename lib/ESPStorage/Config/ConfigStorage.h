#ifndef CONFIGSTORAGE_H
#define CONFIGSTORAGE_H

#include <Preferences.h>
#include <Arduino.h>

class ConfigStorage {
public:
    ConfigStorage();
    void begin();
    void saveConfig(const String& ip, int port);
    String getServerIP();
    int getServerPort();

private:
    Preferences preferences;
    String serverIP;
    int serverPort;
};

#endif
