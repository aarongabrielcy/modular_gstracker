#ifndef GENERATED_H
#define GENERATED_H

#include <Arduino.h>
#include "config.h"
#include <ArduinoJson.h>

struct InOutState {
    int ign = 0;
    int in1 = 0;
    int in2 = 0;
    int in3 = 0;
    int in4 = 0;
    int in5 = 0;
    int in6 = 0;
    int in7 = 0;
    int ou1 = 0;
    int ou2 = 0;
    int ou3 = 0;
    int ou4 = 0;
    int ou5 = 0;
    int rsv1 = 0;
    int rsv2 = 0;
    int rsv3 = 0;
    float bckVolt = 0.0;
    float pwrVolt = 0.0;
    float com1 = 0.0;
    float com2 = 0.0; 
};
class Generated {
    public:
        Generated();
        void stateIO();
        void initInput(int pin);
        bool readInput();
        void initOutput(int pinOut); // Método para inicializar con el pin
        void turnOn();
        void turnOff();
        void GnssLED(int pin, bool state);
        void GnssLedBlink(int pin, bool state);
        InOutState stateInputs( const String &inputs);
        void initIO(char* activeIn, char* activeOut);
        void initializePinsFromJson(const char* json, const char* bitmask);
        void readPinsFromJson(const char* bitmask);
        void printIOData(const InOutState &data);
        InOutState ioState;
    private:
        int pin, pinOut; // Pin asignado al botón
};
#endif
