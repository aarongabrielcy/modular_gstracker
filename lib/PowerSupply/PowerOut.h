#ifndef POWEROUT_H
#define POWEROUT_H

#include <Arduino.h>
#include "config.h"

class PowerOut {
    public:
    void powerKey();
    void powerModule();
    void engineCutOn();
    void engineCutOff();
    private:

};
#endif
