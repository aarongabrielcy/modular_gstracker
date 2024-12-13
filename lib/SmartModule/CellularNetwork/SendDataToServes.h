#ifndef SEND_DATA_TO_SERVES_H
#define SEND_DATA_TO_SERVES_H

#include <Arduino.h>
#include "SimModule/SIM7600.h"
#include "Generated/Generated.h"

class SendDataToServes {

    public:
        SendDataToServes(SIM7600& sim7600);
        bool sendData(String message);
        String readData(String message, int timeout);
        bool configureTCP(const String& server, int port);
        bool validTcpNet();
    private:
        SIM7600& simModule;
        Generated genereted;
        void activeTCP();
        bool getPositionServer();
        String extractCMDData(String input);

};

#endif

