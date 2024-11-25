#ifndef GENERATED_H
#define GENERATED_H

#include <Arduino.h>

class Generated {
    public:
        Generated();
        void stateIO();
        void initInput(int pin);
        bool readInput();
        void initOutput(int pinOut); // Método para inicializar con el pin
        void turnOn();
        void turnOff();
        struct ioGen {
            int ign;
            int in1;
            int in2;
            int ou1;
            int ou2;
            float bckVolt;
            float pwrVolt;
            float com1;
            float com2; 
        };
    private:
        int pin, pinOut; // Pin asignado al botón

};
#endif
