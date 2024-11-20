#ifndef CONNECTION_H
#define CONNECTION_H

#include <Arduino.h>
#include "SimModule/SIM7600.h"

class Connection {
    public:
        Connection(SIM7600& sim7600);
        bool activeModuleSat(int state);
        String ReadDataGNSS();
        String ReadDataGPS();
        int getFix();
    private:
    SIM7600& simModule;
    int fix;
    double vspeed;

};
#endif
