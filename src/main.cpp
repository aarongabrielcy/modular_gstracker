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

SIM7600 simModule(Serial1);  // Instancia de SIM7600 creada en main
ModuleInfo modInfo(simModule); // Inyección de simModule en ModuleInfo
DynamicInfo dynInfo(simModule);
DateTime dateTimeMS(simModule);
// Crear instancias de las clases
ConfigStorage configStorage;
WiFiAPManager wifiManager;
WebServerHandler webServerHandler(configStorage);
NetworkManager networkManager(simModule);

  static bool stateSIM = false;
  static bool stateCfgPdp = false;


void handleSerialInput();
void updateData();
void simInfo();

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  simModule.begin();
  do{Serial.println("Inicializando Modulo...");}while(!networkManager.initializeModule());
  stateSIM = networkManager.readSIMInsert();
  networkManager.basicConfigCDMs();
  configStorage.begin();
  
  // Iniciar modo Access Point
  wifiManager.startAP("GST32 AP", "12345678.");

  // Iniciar servidor web
  webServerHandler.begin();
}

void loop() {
  updateData();
  handleSerialInput();
  // Manejar clientes HTTP
  webServerHandler.handleClient();
  if(!stateSIM){
    stateSIM = networkManager.readSIMInsert();
    if(stateSIM){
      //reincia el módulo sim
    }
  }
  if(!stateCfgPdp && stateSIM){
    stateCfgPdp = networkManager.configurePDP(TELCEL, 1);
  }
  simInfo();
  delay(3000);
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
void updateData(){
  dynInfo.getCPSI(); // Validar que no se imprima hasta que tenga los datos "NO" vacíos
  dateTimeMS.getDateTime(); // La hora a veces no tiene sentido año 2080  
}
