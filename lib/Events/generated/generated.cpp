#include "Generated.h"

Generated::Generated() {}

void Generated::initInput(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}

bool Generated::readInput() {
    return digitalRead(pin);
    
}

void Generated::initOutput(int pinOut) {
    this->pinOut = pinOut;
    pinMode(pinOut, OUTPUT);
    digitalWrite(pinOut, LOW);
}

void Generated::turnOn() {
  digitalWrite(pinOut, HIGH);
}

void Generated::turnOff() {
  digitalWrite(pinOut, LOW);
}

