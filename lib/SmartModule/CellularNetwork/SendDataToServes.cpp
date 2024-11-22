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

void SendDataToServes::configureTCP(const String& server, int port){
  String cgd_cmd = "AT+NETOPEN";
  String cip_cmd = "AT+CIPOPEN=0,\"TCP\",\"" + server + "\"," +port;
  String cgd = simModule.sendCommandWithResponse(cgd_cmd.c_str(), 4000); 
  if(cgd == "OK0"){
      String cip = simModule.sendCommandWithResponse(cip_cmd.c_str(), 4000); 
  }
}
bool SendDataToServes::validTCP(){
  String netopen_cmd = "AT+NETOPEN?";
  String netopen = simModule.sendCommandWithResponse(netopen_cmd.c_str(), 4000);
  if(netopen == "OK"){
    Serial.println("Conectado al servidor correctamente!");
    return true;
  }
  return false;
}