#include <Arduino.h>
#include "SimModule/SIM7600.h"
#include "config.h"
#include "Network/NetworkManager.h"

SIM7600 simModule(Serial1);

void handleSerialInput();

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  simModule.begin();
  configureAPN(DEFAULT_APN);
}

void loop() {
  handleSerialInput();
}

void handleSerialInput() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    String response = simModule.sendCommandWithResponse(command.c_str());
    Serial.print("Respuesta limpia: ");
    Serial.println(response);
  }
}
