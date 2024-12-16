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
String NetworkManager::softReset(){
  String cfunr_cmd = "AT+CFUN=1,1";
  String cfunr = simModule.sendCommandWithResponse(cfunr_cmd.c_str(), 4000);
  return cfunr;

}
bool NetworkManager::readSIMInsert(){
    String cpin_cmd = "AT+CPIN?";
  String cpin = simModule.sendCommandWithResponse(cpin_cmd.c_str(), 4000);
  if(cpin == "READY"){
    return true;
  }else if(cpin == "+CME ERROR: SIM failure"){
      Serial.println("SIM CARD ERR0R!");
  return false;
  }else if(cpin == "+SIMCARD: NOT AVAILABLE+CME ERROR: SIM failure"){
  Serial.println("NO SIM INSERT!");
  return false;
  }else {
    Serial.println("Erro al intentar leer el estado de la SIM!");
    return false;
  }

}
void NetworkManager::basicConfigCDMs(){
   String commands[] = {
        "AT+CFUN=1",
        /*"AT&D2",
        "AT+CSCLK=1"*/
    };
    for (const String &cmd : commands) {
        String response = simModule.sendCommandWithResponse(cmd.c_str(), 4000);

        // Opcional: imprimir o validar la respuesta
        if (response.isEmpty()) {
            Serial.println("No response was received for: " + cmd);
        } else {
            Serial.println("RSP => " + cmd + ": " + response);
        }
    }
}
void NetworkManager::configureAPN(const String& apn) {
  //BUSCA QUE ESPACIO TIENES LIBRE 
  int c_id = readPositionAPN();
  if(c_id != 0){
    String cgdcont_cmd = "AT+CGDCONT=\""+String(c_id)+"\",\""+PDPTYPE+"\",\""+apn+"\"";
    String cgdcont = simModule.sendCommandWithResponse(cgdcont_cmd.c_str(), 4000);
  }
  
}
int NetworkManager::readPositionAPN(){
  //INDICE
  String cgd_cmd = "AT+CGDCONT?";
  String cgd = simModule.sendCommandWithResponse(cgd_cmd.c_str(), 4000);
   // Variables para verificar si los índices están ocupados
  bool indice1Ocupado = cgd.indexOf("1,\"IP\"") != -1;
  bool indice2Ocupado = cgd.indexOf("2,\"IP\"") != -1;
  bool indice3Ocupado = cgd.indexOf("3,\"IP\"") != -1;

  // Retorna el índice más bajo que esté libre
  if (!indice1Ocupado) return 1; // Si el índice 1 está libre
  if (!indice2Ocupado) return 2; // Si el índice 2 está libre
  if (!indice3Ocupado) return 3; // Si el índice 3 está libre
  return 0;
}

//manda al configurador el objeto de apn
String NetworkManager::readAPNs( ) {
  //VALIDA SI EL APN COINCIDE CON LA COMPAÑIA
  String cgd_cmd = "AT+CGDCONT?";
  String cgd = simModule.sendCommandWithResponse(cgd_cmd.c_str(), 4000);

  DynamicJsonDocument doc(1024);

  int pos = 0;
  int index = 0;

  while ((pos = cgd.indexOf("\"IP\"", pos)) != -1) {
    // Extraer ID
    int idEnd = cgd.indexOf(',', pos -2);
    int idStart = cgd.lastIndexOf(',', idEnd - 1) + 1;
    String id = cgd.substring(idStart, idEnd);

    // Extraer APN
    int apnStart = cgd.indexOf('"', pos + 4) + 1;
    int apnEnd = cgd.indexOf('"', apnStart);
    String apn = cgd.substring(apnStart, apnEnd);

    // Agregar al JSON
    JsonObject entry = doc.createNestedObject();
    entry["id"] = id.toInt();
    entry["pdptype"] = "IP";
    entry["apn"] = apn;

    pos = apnEnd + 1;
    index++;
  }

  // Convertir a JSON string
  String output;
  serializeJson(doc, output);
  return output;
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
int validateCompanyWithApn(int company, String apns){
  Serial.println("company: "+company);
  Serial.println("json apns: "+apns);
  // Tamaño del documento JSON
  const size_t capacity = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(3) + 200;
  DynamicJsonDocument doc(capacity);

  // Deserializar el JSON
  DeserializationError error = deserializeJson(doc, apns);

  if (error) {
    Serial.print("Error al deserializar JSON: ");
    Serial.println(error.c_str());
    return  0;
  }

  // Recorrer el JSON
  JsonArray array = doc.as<JsonArray>();
  for (JsonObject obj : array) {
    const int cid = obj["id"];
    const char* apn = obj["apn"];
    if(company == TELCEL){
      if (strcmp(apn, String(DEFAULT_APN_TELCEL).c_str()) == 0) {
        return cid;  // Retorna el APN encontrado
      }
    }else if(company == ATT){
      if (strcmp(apn, String(DEFAULT_APN_ATT).c_str()) == 0 ) {
        return cid;  // Retorna el APN encontrado
      }
    }
    
  }
}
//VALIDA QUE SIM TIENE INSERTATA Y MANDA EL OBJETO DE APNS para validar que se encuetre
bool NetworkManager::configurePDP( int cid, int state) {
  int company = NetworkManager::readCompanySIM();
  String apns =  NetworkManager::readAPNs();
  int validateApn  = validateCompanyWithApn(company, apns); 

  String cgAct_cmd = "AT+CGACT=" + String(state) + "," + validateApn;
  String cgAct = simModule.sendCommandWithResponse(cgAct_cmd.c_str(), 4000);
  
  if(company == ATT && cgAct == "OK"){
    apn_1 = DEFAULT_APN_ATT;
    public_ip_1 = NetworkManager::getIpActive(validateApn);
    return true;

  }else if(company == TELCEL && cgAct == "OK"){
    apn_1 = DEFAULT_APN_TELCEL;
    public_ip_1 = NetworkManager::getIpActive(validateApn);
    return true;
  }else{
    Serial.println("Configure el APN según su region => "+cgAct);
    return false;
  }
}
String  NetworkManager::getIpActive(int c_id){
  String cgpaddr_cmd = "";
  
  cgpaddr_cmd = "AT+CGPADDR="+String(c_id);
  Serial.println("Comando para obtener ip public => "+cgpaddr_cmd);
  String cgpaddr =  simModule.sendCommandWithResponse(cgpaddr_cmd.c_str(), 5000);;
   
  return cgpaddr;
}
//esta funcion no está bien validada // esta funcion consulta que contexto pdp está activo
bool NetworkManager::validateActivePDP(int c_id){
  String cga_cmd = "AT+CGACT?";
  String cgpaddr_cmd = "";
  String cga = simModule.sendCommandWithResponse(cga_cmd.c_str(), 4000);
  if(cga == "1,02,03,0"){
      return false;
  }else if("1,12,03,0"){
    return true;
  }
  return false;
}

String NetworkManager::getApn1() { return apn_1; }
String NetworkManager::getApn2() { return apn_2; }

String NetworkManager::getPublicIp1() { return public_ip_1; }
String NetworkManager::getPublicIp2() { return public_ip_2; }


