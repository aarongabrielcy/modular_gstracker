#ifndef WEBSERVERHANDLER_H
#define WEBSERVERHANDLER_H

#include <WebServer.h>
#include "Config/ConfigStorage.h"

class WebServerHandler {
public:
    WebServerHandler(ConfigStorage& config);
    void begin();
    void handleClient();

private:
    WebServer server;
    ConfigStorage& configStorage;
    void handleRoot();
    void handleSetConfig();
};

#endif
