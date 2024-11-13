#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <Arduino.h>
#include "SimModule/SIM7600.h"

class NetworkManager {
    public:
        NetworkManager(SIM7600& sim7600);
        bool initializeModule();
        void basicConfigCDMs();
        void configureAPN(const String& apn);
        bool configurePDP();
        bool validateAPN();
        bool validatePDP();
        void configureTCP(const String& server, int port);
        bool validTCP();
        String getAPN();
        String getPublicIp();
        
    private:
        SIM7600& simModule;
        String apn, public_ip;
};
#endif
