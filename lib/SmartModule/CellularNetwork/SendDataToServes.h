#ifndef SEND_DATA_TO_SERVES_H
#define SEND_DATA_TO_SERVES_H

#include <Arduino.h>
#include "SimModule/SIM7600.h"

class SendDataToServes {

    public:
        SendDataToServes(SIM7600& sim7600);
        String sendData(String message);
        String readData(String message, int timeout);

    private:
        SIM7600& simModule;
        void configureTCP(const String& server, int port);
        bool validTCP();

};

#endif
