#ifndef WEBSERVERHANDLER_H
#define WEBSERVERHANDLER_H

#include <WebServer.h>
#include "Config/ConfigStorage.h"

class WebServerHandler {
public:
    WebServerHandler(ConfigStorage& config);
    void begin();
    void handleClient();
    bool handleRoot();
    String getServerIP();
    int getServerPort();
private:
    WebServer server;
    ConfigStorage& configStorage;
    void handleSetConfig();
    String serverIP;
    int serverPort;
};

#endif
