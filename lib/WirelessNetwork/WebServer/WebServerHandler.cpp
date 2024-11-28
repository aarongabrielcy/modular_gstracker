#include "WebServerHandler.h"

WebServerHandler::WebServerHandler(ConfigStorage& config) 
    : server(80), configStorage(config) {}

//agregar algún tipo de validación por seguridad y no quede todo expuesto.
void WebServerHandler::begin() {
    server.on("/", HTTP_GET, [this]() { handleRoot(); });
    server.on("/setConfig", HTTP_POST, [this]() { handleSetConfig(); });
    server.begin();
}

void WebServerHandler::handleClient() {
    server.handleClient();
}

bool WebServerHandler::handleRoot() {
    String message = "Configuración actual:\n";
    if(configStorage.getServerIP()){
        message += "IP del servidor: " + configStorage.getServerIP() + "\n";
        message += "Puerto del servidor: " + String(configStorage.getServerPort());
        server.send(200, "text/plain", message);
        return true;
    }
    return false;
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

String WebServerHandler::getServerIP() {
    return serverIP;
}

int WebServerHandler::getServerPort() {
    return serverPort;
}
