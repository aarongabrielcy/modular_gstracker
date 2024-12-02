#include "SendDataToServes.h"
#include "SimModule/SIM7600.h"

SendDataToServes::SendDataToServes(SIM7600& sim7600) : simModule(sim7600) {}

String SendDataToServes::sendData(String message) {
    Serial.print("DATA => ");
    Serial.println(message);
    return "OK";    
}

String SendDataToServes::readData(String data, int timeout) {
  Serial.println("Read data");
  return "OK";
}

bool SendDataToServes::configureTCP(const String& server, int port) {
  bool stNetOpen;
  String cip_cmd = "AT+CIPOPEN=0,\"TCP\",\"" + server + "\"," +port;
  Serial.println("cmd CIP => " + cip_cmd);
  String cip = simModule.sendCommandWithResponse(cip_cmd.c_str(), 4000);
  Serial.println("Response CIP => " + cip);
  if(cip != "OK") {
    Serial.println("## NO Conectado al servidor TCP!");
    return false;
  }
  //calbiar la funcion a boil segun la respuesta de "cip" para validar que se configuró y se conecto al servidor tcp
  Serial.println("resp server tcp => "+ cip);
  return true;
}
bool SendDataToServes::validTCP() {
  String netopen_cmd = "AT+NETOPEN?";
  String netopen = simModule.sendCommandWithResponse(netopen_cmd.c_str(), 4000);
  Serial.println("Valid TCP => " + netopen);
  if(netopen == "0OK1" || netopen == "1"){
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
