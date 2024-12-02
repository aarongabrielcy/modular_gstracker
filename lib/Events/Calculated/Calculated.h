#ifndef CALCULATED_H
#define CALCULATED_H

#include <Arduino.h>

class Calculated {
    public:
    void stateEvent();
    struct ioCal {
        int engineCut;
        int engineOf;
        int engineOn;
        int panicBtn;
        int keepAlive;
        int beat;
    };

    private:
};
#endif