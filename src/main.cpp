#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial SIM7600(1);  // Serial1 en pines 17 (TX) y 18 (RX)

String imei, ccid, datetime_module;

enum cmdResponse {
  ROK = 0,
  ERROR = 1,
  DATA = 2
};

enum cmdType {
  TEST = 0,
  READ = 1,
  WRITE = 2,
  EXECUTE = 3,
  UNKNOWN = -1
};

const char *commandAT[] = {
    "AT",
    "CFUN",
    "CGDCONT",
    "COPS",
    "NETOPEN",
    "CIPOPEN",
    "CGACT"
};

String sendCommandWithResponse(const char* command, int timeout = 5000);
int commandType(const String& command); 
void handleSerialInput();
void setup() {
  Serial.begin(115200);      // Monitor serial
  SIM7600.begin(115200, SERIAL_8N1, 18, 17); // Comunicación con SIM7600
}

void loop() {
  handleSerialInput();
}

void handleSerialInput() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    String response = sendCommandWithResponse(command.c_str());  // Envía comando AT al módulo
    Serial.print("Respuesta limpia: ");
    Serial.println(response);
  }
}

String sendCommandWithResponse(const char* command, int timeout) {
  Serial.print("Enviando comando => ");
  Serial.println(command);

  // Determinar tipo de comando y almacenar en variable
  int type = commandType(command);
  
  // Limpiar el prefijo "AT+" del comando
  String formattedCommand = String(command).substring(3); // Quitar "AT+"
  if(type == READ){
    //quitar cualquier sufijo "?"
    if (formattedCommand.endsWith("?") ) {
      formattedCommand.remove(formattedCommand.length() - 1); // Remover sufijo
    }
  }else if(type == WRITE){
    // Buscar si hay un '=' en el comando, y eliminar todo lo que esté después
    int posEqual = formattedCommand.indexOf("=");
    if (posEqual != -1) {
      formattedCommand = formattedCommand.substring(0, posEqual);  // Mantener solo hasta antes del '='
    }
  }
  
  Serial.print("Comando formateado: ");
  Serial.println(formattedCommand);

  String response = "";

  for (const char* cmd : commandAT) {
    if (formattedCommand == cmd) {
      SIM7600.println(command); // Envía el comando al módulo SIM
      long int time = millis();
      while ((millis() - time) < timeout) {
        if (SIM7600.available()) {
          char c = SIM7600.read();
          response += c;
        }
      }
    }
  }
  if(response == ""){
    return "INVALID COMMAND";
  }  
  Serial.println("Respuesta completa: ");
  Serial.println(response);


  // Limpiar la respuesta de retorno para dejar solo los datos
  String processedResponse = response;
  
  // Eliminar el comando completo enviado (como "AT+CGPADDR=1")
  processedResponse.replace(String(command), "");

  // Eliminar el prefijo de respuesta que contiene el comando (como "+CGPADDR: ")
  processedResponse.replace("+" + formattedCommand + ": ", "");

  // Limpiar saltos de línea, retornos de carro y espacios extra
  processedResponse.replace("\n", "");
  processedResponse.replace("\r", "");
  processedResponse.trim();

  // Verificar si la respuesta contiene "OK" o "ERROR" al final y removerlos
  if (processedResponse.endsWith("OK")) {
    processedResponse.remove(processedResponse.length() - 2); // Elimina "OK"
    Serial.println("Estado del comando: OK");
  } else if (processedResponse.endsWith("ERROR") || processedResponse.indexOf('ERROR') != -1) {
    processedResponse.remove(processedResponse.length() - 5); // Elimina "ERROR"
    Serial.println("Estado del comando: ERROR");
    return "ERROR COMMAND";
  }

  Serial.print("Respuesta procesada: ");
  Serial.println(processedResponse);

  return processedResponse;
}

int commandType(const String& command) {
  Serial.print("Analizando tipo de comando: ");
  Serial.println(command);

  // Comando de prueba (AT+<x>=?)
  if (command.endsWith("=?")) {
    Serial.println("Es un comando de prueba (TEST).");
    return TEST;
  }
  // Comando de lectura (AT+<x>?)
  else if (command.endsWith("?") && command.indexOf('=') == -1) {
    Serial.println("Es un comando de lectura (READ).");
    return READ;
  }
  // Comando de escritura (AT+<x>=<...>)
  else if (command.indexOf('=') != -1 && !command.endsWith("?")) {
    Serial.println("Es un comando de escritura (WRITE).");
    return WRITE;
  }
  // Comando de ejecución (AT+<x>)
  else if (command.startsWith("AT+") && command.indexOf('=') == -1 && !command.endsWith("?")) {
    Serial.println("Es un comando de ejecución (EXECUTE).");
    return EXECUTE;
  } 
  // Si no coincide con ninguno de los anteriores, es desconocido
  else {
    Serial.println("Tipo de comando desconocido.");
    return UNKNOWN;
  }
}