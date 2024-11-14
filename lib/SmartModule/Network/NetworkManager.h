#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <Arduino.h>
#include "SimModule/SIM7600.h"
#include "littles_emus.h"

class NetworkManager {
    public:
        NetworkManager(SIM7600& sim7600);
        bool initializeModule();
        void basicConfigCDMs();
        bool readSIMInsert();
        int readCompanySIM();
        void configureAPN(const String& apn, int c_id, const String& pdp_type);
        bool configurePDP();
        bool readConfiguredAPN(int commpany);
        bool validatePDP();
        void configureTCP(const String& server, int port);
        bool validTCP();
        String getApn1();
        String getApn2();
        String getPublicIp1();
        String getPublicIp2();
        
    private:
        SIM7600& simModule;
        String apn_1, apn_2, public_ip_1, public_ip_2, company_1, company_2;
};
#endif
