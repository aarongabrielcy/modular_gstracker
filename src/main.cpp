#include <Arduino.h>
#include "SimModule/SIM7600.h"
#include "config.h"
#include "Network/NetworkManager.h"
#include "SimModule/ModuleInfo.h"
#include "SimModule/DynamicInfo.h"

SIM7600 simModule(Serial1);  // Instancia de SIM7600 creada en main
ModuleInfo modInfo(simModule); // Inyección de simModule en ModuleInfo
DynamicInfo dynInfo(simModule);

void handleSerialInput();

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  simModule.begin();
  configureAPN(DEFAULT_APN);

  Serial.println("IMEI => " + modInfo.getIMEI());
  Serial.println("CCID => " + modInfo.getCCID());
  //Serial.println("CPSI => "+ dynInfo.getCPSI()); //tiene que ir en un loop 
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
