#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <Arduino.h>
#include "SimModule/SIM7600.h"

class NetworkManager {
    public:
        NetworkManager(SIM7600& sim7600);
        void configureAPN(const String& apn);
        void configurePDP();
        bool validationAPN(const String& static_apn);
        String getAPN();
    private:
        SIM7600& simModule;
        bool validatePDP();
        String apn;
};
#endif
