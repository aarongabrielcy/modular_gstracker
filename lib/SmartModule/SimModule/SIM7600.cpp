#include "SIM7600.h"
#include "Utils/Utils.h"
// Inicialización de la constante estática de comandos permitidos
const char* SIM7600::allowedCommands[21] = {
    "AT", "CFUN", "CGDCONT","CGDSCONT","COPS", "CSQ", "NETOPEN", "CIPOPEN", "CGACT", "SIMEI", "CCID", "CPSI", "CCLK", "CGPS", "CGPSINFO", "CGPSCOLD", "CGPSHOT", "CGNSSINFO", "CREG", "CGPADDR","CGPSNMEA" 
};

// Constructor
SIM7600::SIM7600(HardwareSerial& serial) : simSerial(serial) {}

void SIM7600::begin() {
  simSerial.begin(SIM7600_BAUD_RATE, SERIAL_8N1, SIM7600_RX_PIN, SIM7600_TX_PIN);
}

String SIM7600::sendCommandWithResponse(const char* command, int timeout) {
  /*Serial.print("Enviando comando => ");
  Serial.println(command);*/
  int type = commandType(command);

  String formattedCommand = String(command).substring(3); // Quitar "AT+"
  if(type == READ){
    //quitar cualquier sufijo "?"
   // Serial.println("Quitando sufijo... ");
    if (formattedCommand.endsWith("?") ) {
      formattedCommand.remove(formattedCommand.length() - 1); // Remover sufijo
    }
  }else if(type == WRITE){
    //Serial.println("quitando signo '='");
    // Buscar si hay un '=' en el comando, y eliminar todo lo que esté después
    int posEqual = formattedCommand.indexOf("=");
    if (posEqual != -1) {
      formattedCommand = formattedCommand.substring(0, posEqual);  // Mantener solo hasta antes del '='
    }
  }else if(type == TEST){
    if (formattedCommand.endsWith("=?") ) {
      formattedCommand.remove(formattedCommand.length() - 2); // Remover sufijo
    }
  }
  Serial.print("Comando formateado: ");
  Serial.println(formattedCommand);
  
  if (!isAllowedCommand(formattedCommand)) {
    Serial.println("Comando no permitido.");
    return "INVALID COMMAND";
  }
  if(formattedCommand == "AT"){
      simSerial.println(formattedCommand);  // Enviar comando
  }else{
    simSerial.println(command);  // Enviar comando    
  }
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
  return processResponse(command, formattedCommand, response);
}

String SIM7600::processResponse(const String& command,  const String& fcommand, const String& response) {
  String state_command = "";
  String processedResponse = response;
  processedResponse.replace(String(command), "");
  //processedResponse.replace("AT+" + fcommand, "");
  processedResponse.replace("+" + fcommand + ": ", "");
  /*Serial.print("Procesando respuesta... ");
  Serial.println(processedResponse);*/

  processedResponse = trimResponse(processedResponse);
  
  if (processedResponse.endsWith("OK")) {
    processedResponse.remove(processedResponse.length() - 2);
    state_command = "OK";
    //Serial.println("Estado del comando: "+state_command);
  } else if (processedResponse.endsWith("ERROR")) {
    processedResponse.remove(processedResponse.length() - 5);
    state_command = "ERROR";
    //Serial.println("Estado del comando: "+state_command);
    return "ERROR COMMAND";
  }

  /*Serial.print("Respuesta procesada: ");
  Serial.println(processedResponse);*/
  if(processedResponse.length() == 0){
    return state_command;
  }
  return processedResponse;
}

bool SIM7600::isAllowedCommand(const String& command) {
  for (const char* cmd : allowedCommands) {
    if (command == cmd) {
      return true;
    }
  }
  return false;
}

int SIM7600::commandType(const String& command) {
  if (command.endsWith("=?")){
    Serial.println("Es un comando de prueba (TEST).");
    return TEST;
  }else if (command.endsWith("?") && command.indexOf('=') == -1){
    Serial.println("Es un comando de lectura (READ).");
    return READ;
  }else if (command.indexOf('=') != -1 && !command.endsWith("?")){ 
    Serial.println("Es un comando de escritura (WRITE).");
    return WRITE;
  }else if (command.startsWith("AT+") && command.indexOf('=') == -1){
   Serial.println("Es un comando de ejecución (EXECUTE).");
   return EXECUTE;
   }else{ Serial.println("Tipo de comando desconocido.");return UNKNOWN;}
}
