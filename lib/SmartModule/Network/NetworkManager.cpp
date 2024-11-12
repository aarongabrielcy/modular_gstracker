#include "NetworkManager.h"
#include "SimModule/SIM7600.h"

NetworkManager::NetworkManager(SIM7600& sim7600) : simModule(sim7600) {}

void NetworkManager::configureAPN(const String& apn) {
  String cgdcont_cmd = "AT+CGDCONT=1,\"IP\",\""+apn+"\"";
  String cgdcont = simModule.sendCommandWithResponse(cgdcont_cmd.c_str(), 4000);

}
bool NetworkManager::validationAPN(const String& static_apn){
  String cgd_cmd = "AT+CGDCONT?";
  String cgd = simModule.sendCommandWithResponse(cgd_cmd.c_str(), 4000);

  int ipPos = cgd.indexOf("\"IP\"");  // Encontrar "IP"
    if (ipPos == -1) {
      Serial.println("IP not found");
      apn = "";
      return false;  // Si no se encuentra "IP"
    }

    int start = cgd.indexOf("\"", ipPos + 4) + 1;  // Buscar la comilla después de "IP"
    int end = cgd.indexOf("\"", start);  // Encontrar la siguiente comilla que cierra el APN

    if (start != -1 && end != -1) {
        apn = cgd.substring(start, end);
        if(apn == static_apn){
          Serial.println("APN valido y configurado correctmente!");
        }
        Serial.print("Apn S/F: ");
        Serial.println(apn);
        return true;  // Extraer y retornar el APN
    } else {
        Serial.println("APN format error");
        return false;  // Si no se encuentra correctamente
    }
  return false;
}
void NetworkManager::configurePDP(){
  if(NetworkManager::validationAPN || NetworkManager::validatePDP()){
    String cgAct_cmd = "AT+CGACT=1,1";
    String cgAct = simModule.sendCommandWithResponse(cgAct_cmd.c_str(), 4000);
  }
}

bool NetworkManager::validatePDP(){
  String cga_cmd = "AT+CGACT?";
  String cga = simModule.sendCommandWithResponse(cga_cmd.c_str(), 4000);
  return false;
}

void configureTCP(){

}

bool validTCP(){
  return false;
}

String NetworkManager::getAPN() { return apn; }

