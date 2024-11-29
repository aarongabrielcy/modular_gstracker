#include "SendDataToServes.h"
#include "SimModule/SIM7600.h"

SendDataToServes::SendDataToServes(SIM7600& sim7600) : simModule(sim7600) {}

String SendDataToServes::sendData(String message) {
    /*Serial.print("DATA => ");
    Serial.println(message);*/
    return "OK";    
}

String SendDataToServes::readData(String data, int timeout) {
  Serial.println("Read data");
  return "OK";
}

void SendDataToServes::configureTCP(const String& server, int port) {
  Serial.print("TCP Server => ");
  Serial.println(server+":"+port);
  bool stNetOpen;
  String cgd_cmd = "AT+NETOPEN";
  String cip_cmd = "AT+CIPOPEN=0,\"TCP\",\"" + server + "\"," +port;
  String cgd = simModule.sendCommandWithResponse(cgd_cmd.c_str(), 4000); 
  Serial.println("Cmd CIPOPEN => " + cip_cmd);
  Serial.println("netopen => " + cgd);
  cgd == "OK0" || cgd == "OK1" ? stNetOpen = true : stNetOpen = false;
  if(stNetOpen || !SendDataToServes::validTCP()) {
    String cip = simModule.sendCommandWithResponse(cip_cmd.c_str(), 4000);
    if(cip != "OK"){
      Serial.println("## NO Conectado al servidor TCP!");
    }
    //calbiar la funcion a boil segun la respuesta de "cip" para validar que se configuró y se conecto al servidor tcp
    Serial.println("resp server tcp => "+ cip);
  }
}
bool SendDataToServes::validTCP() {
  String netopen_cmd = "AT+NETOPEN?";
  String netopen = simModule.sendCommandWithResponse(netopen_cmd.c_str(), 4000);
  if(netopen == "OK"){
    Serial.println("Servicio TCP Inicializado!");
    return true;
  }
  return false;
}