#include "SendDataToServes.h"

SendDataToServes::SendDataToServes(SIM7600& sim7600) : simModule(sim7600) {}

bool SendDataToServes::sendData(String message) {
  Serial.print("DATA => ");
  Serial.println(message);
  String respServer = "";

  String cmd = "AT+CIPSEND=0," + String(message.length());
  String response = simModule.sendCommandWithResponse(cmd.c_str(), 500);
  
  if (response.indexOf(">") != -1) {
    String respServer = simModule.sendReadDataToServer("CIPSEND", message, 3000); // Envía el mensaje   
    Serial.println("Respuesta procesada => " + respServer);
    String respCMD = readData(respServer, 1000);
    Serial.println("Comando recibido =>"+respCMD);
  }else {
    Serial.println("Error al enviar mensaje TCP. Intentando reconexión...");
    return false;
  }
    return true;
}

String SendDataToServes::readData(String data, int timeout) {
  int startIndex = data.indexOf("CMD");
  if (startIndex == -1) {
    // Si no encuentra "CMD", devuelve una cadena vacía
    return "";
  }
  if(data.substring(startIndex) == "CMD;02049830910;04;01"){
    Serial.println("Activa salida ===========>");
    genereted.turnOn();
    
  }else if(data.substring(startIndex) == "CMD;02049830910;04;02"){
  
    Serial.println("Desactiva salida ===========>");
    genereted.turnOff();
  }

  // Extrae desde "CMD" hasta el final de la cadena
  return data.substring(startIndex);  
}

bool SendDataToServes::configureTCP(const String& server, int port) {
  bool stNetOpen;
  String cip_cmd = "AT+CIPOPEN=0,\"TCP\",\"" + server + "\"," +port;
  Serial.println("cmd CIP => " + cip_cmd);
  if(!SendDataToServes::getPositionServer() ){
    String cip = simModule.sendCommandWithResponse(cip_cmd.c_str(), 4000);
    Serial.println("Response CIP => " + cip);
    if(cip != "OK0,0") {// el ,0 indica el indice del servidor
      Serial.println("## NO Conectado al servidor TCP!");
      return false;
    }
  }
  //calbiar la funcion a boil segun la respuesta de "cip" para validar que se configuró y se conecto al servidor tcp
  Serial.println("server tcp connected => ");
  return true;
}

bool SendDataToServes::validTcpNet() {
  String netopen_cmd = "AT+NETOPEN?";
  String netopen = simModule.sendCommandWithResponse(netopen_cmd.c_str(), 4000);
  Serial.println("Valid TCP => " + netopen);
  if(netopen == "0OK1" || netopen == "1") {
    Serial.println("Servicio TCP Inicializado!");
    return true;
  }else if(netopen == "0"){
    SendDataToServes::activeTCP();
  }
  return false;
}
void SendDataToServes::activeTCP() {
    String netactive_cmd = "AT+NETOPEN";
    String netactive = simModule.sendCommandWithResponse(netactive_cmd.c_str(), 4000);
    Serial.println("Active TCP => " + netactive);
}

bool SendDataToServes::getPositionServer() {
  //"0,\"TCP","34.196.135.179\",5200,-1123456789"; // sin configuración: 0123456789
  String cip_st_cmd = "AT+CIPOPEN?";
  String cip_state = simModule.sendCommandWithResponse(cip_st_cmd.c_str(), 4000);
  Serial.println("rsp cip_state => "+ cip_state);
  if(cip_state == "0123456789"){
    return false;
  }else if(cip_state.indexOf(",-1") != -1){
    return true;
  }
  return false;
}

String SendDataToServes::extractCMDData(String input) {
  // Encuentra la posición donde comienza "CMD;"
  int startIndex = input.indexOf("CMD");
  if (startIndex == -1) {
    // Si no encuentra "CMD;", retorna una cadena vacía
    return "";
  }

  // Extrae la subcadena desde "CMD;" hasta el final
  String result = input.substring(startIndex);
  Serial.println("comando recibido => " + result);
  return result;
}