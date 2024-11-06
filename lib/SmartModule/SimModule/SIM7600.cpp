#include "SIM7600.h"

// Constructor
SIM7600::SIM7600(HardwareSerial& serial) : simSerial(serial) {}

void SIM7600::begin() {
  simSerial.begin(SIM7600_BAUD_RATE, SERIAL_8N1, SIM7600_RX_PIN, SIM7600_TX_PIN);
}

String SIM7600::sendCommandWithResponse(const char* command, int timeout) {
  Serial.print("Enviando comando => ");
  Serial.println(command);

  String formattedCommand = String(command).substring(3);
  int posEqual = formattedCommand.indexOf("=");
  if (posEqual != -1) {
    formattedCommand = formattedCommand.substring(0, posEqual);
  }

  if (!isAllowedCommand(formattedCommand)) {
    Serial.println("Comando no permitido.");
    return "INVALID COMMAND";
  }

  simSerial.println(command);  // Enviar comando
  String response = "";
  long startTime = millis();
  while ((millis() - startTime) < timeout) {
    if (simSerial.available()) {
      char c = simSerial.read();
      response += c;
    }
  }

  Serial.println("Respuesta completa: ");
  Serial.println(response);

  return processResponse(formattedCommand, response);
}

String SIM7600::processResponse(const String& command, const String& response) {
  String processedResponse = response;
  processedResponse.replace("AT+" + command, "");
  processedResponse.replace("+" + command + ": ", "");
  processedResponse.replace("\n", "");  // Eliminar saltos de línea
  processedResponse.replace("\r", "");  // Eliminar retornos de carro
  processedResponse.trim();

  if (processedResponse.endsWith("OK")) {
    processedResponse.remove(processedResponse.length() - 2);
    Serial.println("Estado del comando: OK");
  } else if (processedResponse.endsWith("ERROR")) {
    processedResponse.remove(processedResponse.length() - 5);
    Serial.println("Estado del comando: ERROR");
    return "ERROR COMMAND";
  }

  Serial.print("Respuesta procesada: ");
  Serial.println(processedResponse);
  return processedResponse;
}

bool SIM7600::isAllowedCommand(const String& command) {
  for (const char* cmd : allowedCommands) {
    if (command == cmd) return true;
  }
  return false;
}

int SIM7600::commandType(const String& command) {
  if (command.endsWith("=?")) return TEST;
  else if (command.endsWith("?") && command.indexOf('=') == -1) return READ;
  else if (command.indexOf('=') != -1 && !command.endsWith("?")) return WRITE;
  else if (command.startsWith("AT+") && command.indexOf('=') == -1) return EXECUTE;
  else return UNKNOWN;
}
