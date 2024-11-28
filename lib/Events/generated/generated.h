#ifndef GENERATED_H
#define GENERATED_H

#include <Arduino.h>
#include "config.h"
#include <ArduinoJson.h>

struct InOutState {
    int ign;
    int in1;
    int in2;
    int in3;
    int ou1;
    int ou2;
    int out3;
    float bckVolt;
    float pwrVolt;
    float com1;
    float com2; 
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
