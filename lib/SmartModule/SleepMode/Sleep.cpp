#include "Sleep.h"

Sleep::Sleep(SIM7600& sim7600) : simModule(sim7600) {}

void Sleep::InitSleepPin(int pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);

}
void Sleep::ActiveSleep(int pin) {
   Serial.println("Entrando en modo sleep...");
  digitalWrite(pin, HIGH); // DTR en HIGH para activar el modo sleep
  //inSleepMode = true;
}
bool Sleep::StateSleep(int pin) {
  return digitalRead(pin);
}

void Sleep::DeactivateSleep(int pin) {
  Serial.println("Saliendo del modo sleep...");
  String cmd_basic = "AT+AT";
  digitalWrite(pin, LOW);  // DTR en LOW para salir del modo sleep
   //inSleepMode = false;

  // Espera a que el módulo se reactive
  delay(500);
  String response = simModule.sendCommandWithResponse(cmd_basic.c_str(), 1000);
}
