#include "WebServerHandler.h"

WebServerHandler::WebServerHandler(ConfigStorage& config) 
    : server(80), configStorage(config) {}

void WebServerHandler::begin() {
    server.on("/", HTTP_GET, [this]() { handleRoot(); });
    server.on("/setConfig", HTTP_POST, [this]() { handleSetConfig(); });
    server.begin();
}

void WebServerHandler::handleClient() {
    server.handleClient();
}

void WebServerHandler::handleRoot() {
    String message = "Configuración actual:\n";
    message += "IP del servidor: " + configStorage.getServerIP() + "\n";
    message += "Puerto del servidor: " + String(configStorage.getServerPort());
    server.send(200, "text/plain", message);
}

void WebServerHandler::handleSetConfig() {
    if (server.hasArg("ip") && server.hasArg("port")) {
        String ip = server.arg("ip");
        int port = server.arg("port").toInt();
        configStorage.saveConfig(ip, port);
        
        String message = "Nueva configuración guardada:\n";
        message += "IP del servidor: " + ip + "\n";
        message += "Puerto del servidor: " + String(port);
        server.send(200, "text/plain", message);
    } else {
        server.send(400, "text/plain", "Error: Faltan parámetros 'ip' y 'port'");
    }
}
