#include <Arduino.h>
#include "SimModule/SIM7600.h"
#include "config.h"
#include "Network/NetworkManager.h"
#include "SimModule/ModuleInfo.h"
#include "SimModule/DynamicInfo.h"
#include "SimModule/DateTime/DateTime.h"
#include "Config/ConfigStorage.h"
#include "Config/WiFiAPManager.h"
#include "WebServer/WebServerHandler.h"
#include "Network/SendDataToServes.h"
#include "Constellations/Connection.h"

SIM7600 simModule(Serial1);  // Instancia de SIM7600 creada en main
ModuleInfo modInfo(simModule); // Inyección de simModule en ModuleInfo
DynamicInfo dynInfo(simModule);
DateTime dateTimeMS(simModule);
// Crear instancias de las clases
ConfigStorage configStorage;
WiFiAPManager wifiManager;
WebServerHandler webServerHandler(configStorage);
NetworkManager networkManager(simModule);
SendDataToServes sendDataToServes(simModule);
Connection connection(simModule);
 
bool stateSIM;
bool stateGnss;
int count_reset = 0;
int previous_time_update = 0;
int previous_time_send = 0;
int counter_10s = 0;

void handleSerialInput();
void updateData();
void simInfo();

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  simModule.begin();
  do{Serial.println("Inicializando Modulo...");}while(!networkManager.initializeModule());
  //stateSIM = networkManager.readSIMInsert();
  networkManager.basicConfigCDMs();
  connection.activeModuleSat(1);
  configStorage.begin();
  
  // Iniciar modo Access Point
  wifiManager.startAP("GST32 AP", "12345678.");

  // Iniciar servidor web
  webServerHandler.begin();
}

void loop() {
  stateSIM = false;
  stateGnss = false;
  static bool stateCfgPdp = false;
  handleSerialInput();

  unsigned long current_time = millis();
  
  if (current_time - previous_time_update >= UPDATE_DATA_TIMEOUT) {
    previous_time_update = current_time;  // Actualizar el tiempo anterior
      dynInfo.getCPSI(); // Validar que no se imprima hasta que tenga los datos "NO" vacíos
      dateTimeMS.getDateTime(); // La hora a veces no tiene sentido año 2080 
      connection.ReadDataGNSS(); 

  }
  
  // Manejar clientes HTTP
  webServerHandler.handleClient();
  if(!stateSIM){
    Serial.println("Validando estado de la SIMCARD");
    stateSIM = networkManager.readSIMInsert();
    if(!stateSIM){
      //reincio suave al módulo sim
      count_reset++;
      Serial.println("** SIM no insertada! => ");
      if(count_reset == 12){
        Serial.println("Reinicio suave del modulo: "+networkManager.softReset());
        count_reset = 0;
        stateCfgPdp = false;
      }
      delay(1000);
    }else{
      Serial.println("## SIM insertada!");
      delay(1000);
    }
  }
  if(!stateCfgPdp && stateSIM){
    Serial.println("configurando PDP");
    stateCfgPdp = networkManager.configurePDP(TELCEL, 1);
  }
  if(stateCfgPdp && stateSIM){
    //simInfo();
    if (current_time - previous_time_send >= SEND_DATA_TIMEOUT) {
      previous_time_send = current_time;  // Actualizar el tiempo anterior
      String message = String(HEADER)+";"+modInfo.getDevID()+";"+REPORT_MAP+";"+MODEL_DEVICE+";"+SW_VER+";"+MSG_TYPE+";"
      +dateTimeMS.getValueUTC()+";"+dynInfo.getCellID()+";"+dynInfo.getMCC()+";"+dynInfo.getLAC()+";"+dynInfo.getRxLev();
      Serial.println(sendDataToServes.sendData(message) );
    }
  }
}

void handleSerialInput() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    String response = simModule.sendCommandWithResponse(command.c_str());
    Serial.print("Respuesta limpia: ");
    Serial.println(response);
  }
}
void simInfo(){
  Serial.println("IMEI => " + modInfo.getIMEI());
  Serial.println("CCID => " + modInfo.getCCID());
  Serial.println("APN SIM INSERT => "+ networkManager.getApn1());
  Serial.println("CellID => " + dynInfo.getCellID());
  Serial.println("MCC => "+ dynInfo.getMCC());
  Serial.println("MNC => "+ dynInfo.getMNC());
  Serial.println("LAC => "+ dynInfo.getLAC());
  Serial.println("RXLVL => " + dynInfo.getRxLev());
  Serial.println("DATETIME => "+ dateTimeMS.getValueUTC());
  Serial.println("IP PUBLIC => "+ networkManager.getPublicIp1());
}

