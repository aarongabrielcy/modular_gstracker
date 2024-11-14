#include "NetworkManager.h"
#include "SimModule/SIM7600.h"

NetworkManager::NetworkManager(SIM7600& sim7600) : simModule(sim7600) {}

bool NetworkManager::initializeModule(){
  String at_cmd = "AT+AT";
  String at = simModule.sendCommandWithResponse(at_cmd.c_str(), 4000);
  if(at == "AT"){
    return true;
  }
  Serial.println("Error initializing module!");
  return false;
}
bool NetworkManager::readSIMInsert(){
    String cpin_cmd = "AT+CPIN?";
  String cpin = simModule.sendCommandWithResponse(cpin_cmd.c_str(), 4000);
  if(cpin == "READY"){
    return true;
  }
  Serial.println("NO SIM INSERT!");
  return false;
}
void NetworkManager::basicConfigCDMs(){
  String cfun_cmd = "AT+CFUN=1";
  String cfun = simModule.sendCommandWithResponse(cfun_cmd.c_str(), 4000);
}
void NetworkManager::configureAPN(const String& apn, int c_id, const String& pdp_type) {

  String cgdcont_cmd = "AT+CGDCONT=\""+String(c_id)+"\",\""+pdp_type+"\",\""+apn+"\"";
  String cgdcont = simModule.sendCommandWithResponse(cgdcont_cmd.c_str(), 4000);

}
bool NetworkManager::readConfiguredAPN(int company){
  String cgd_cmd = "AT+CGDCONT?";
  String cgd = simModule.sendCommandWithResponse(cgd_cmd.c_str(), 4000);

  int ipPos = cgd.indexOf("\"IP\"");  // Encontrar "IP"
    if (ipPos == -1) {
      Serial.println("IP not found");
      apn_1 = "";
      return false;  // Si no se encuentra "IP"
    }

    int start = cgd.indexOf("\"", ipPos + 4) + 1;  // Buscar la comilla después de "IP"
    int end = cgd.indexOf("\"", start);  // Encontrar la siguiente comilla que cierra el APN

    if (start != -1 && end != -1) {
        apn_1 = cgd.substring(start, end); //asigna ip a variable
        return true;  // Extraer y retornar el APN
    } else {
        Serial.println("APN format error");
        return false;  // Si no se encuentra correctamente
    }
  return false;
}
//hay que esperar para que el comando logre la conexion valida el while que se quede intentando conectar AT+CGACT=1,1

int NetworkManager::readCompanySIM() {
  String cops_cmd = "AT+COPS?";
  String cops = simModule.sendCommandWithResponse(cops_cmd.c_str(), 4000);
  if (cops.indexOf("AT&T") != -1) {
    return ATT;
  } 
  // Verifica si la respuesta contiene "TELCEL"
  else if (cops.indexOf("TELCEL") != -1) {
    return TELCEL;
  }
  return 0;
}

bool NetworkManager::configurePDP(){
  if(NetworkManager::readConfiguredAPN() || !NetworkManager::validatePDP()){
    String cgAct_cmd = "AT+CGACT=1,1";
    String cgAct = simModule.sendCommandWithResponse(cgAct_cmd.c_str(), 5000);
    //Encontrar el error el la respuesta "cgAct" para reinciar la funcion hasta que retorne verdadero
    if(cgAct == "OK"){//<--------validar el error aquí
      NetworkManager::validatePDP();
      Serial.println("CGACT Configurado correctamente");
      return true;
    }
  }
  return false;
}
//esta funcion no está bien validada
bool NetworkManager::validatePDP(){
  String cga_cmd = "AT+CGACT?";
  String cgpaddr_cmd = "AT+CGPADDR=1";
  String cga = simModule.sendCommandWithResponse(cga_cmd.c_str(), 4000);
  if(cga == "1,02,03,0"){
      return false;
  }else if("1,12,03,0"){
   String cgpaddr =  simModule.sendCommandWithResponse(cgpaddr_cmd.c_str(), 5000);
   public_ip_1 = cgpaddr;
    return true;
  }
  return false;
}

void NetworkManager::configureTCP(const String& server, int port){
  String cgd_cmd = "AT+NETOPEN";
  String cip_cmd = "AT+CIPOPEN=0,\"TCP\",\"" + server + "\"," +port;
  String cgd = simModule.sendCommandWithResponse(cgd_cmd.c_str(), 4000); 
  if(cgd == "OK0"){
      String cip = simModule.sendCommandWithResponse(cip_cmd.c_str(), 4000); 
  }
}
bool NetworkManager::validTCP(){
  String netopen_cmd = "AT+NETOPEN?";
  String netopen = simModule.sendCommandWithResponse(netopen_cmd.c_str(), 4000);
  if(netopen == "OK"){
    Serial.println("Conectado al servidor correctamente!");
    return true;
  }
  return false;
}

String NetworkManager::getApn1() { return apn_1; }
String NetworkManager::getApn2() { return apn_2; }

String NetworkManager::getPublicIp1() { return public_ip_1; }
String NetworkManager::getPublicIp2() { return public_ip_2; }


