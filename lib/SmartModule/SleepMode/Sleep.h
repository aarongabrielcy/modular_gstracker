#ifndef SLEEP_H
#define SLEEP_H

#include "SimModule/SIM7600.h"
#include "config.h"

class Sleep {
    public:
        Sleep(SIM7600& sim7600);
        void InitSleepPin(int pin);
        void ActiveSleep(int pin);
        bool StateSleep(int pin);
        void DeactivateSleep(int pin);
    private:
        SIM7600& simModule;
        int pin;
};
#endif
