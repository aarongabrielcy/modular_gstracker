#include <Arduino.h>
#include "SimModule/SIM7600.h"
#include "config.h"
#include "CellularNetwork/NetworkManager.h"
#include "SimModule/ModuleInfo.h"
#include "SimModule/DynamicInfo.h"
#include "SimModule/DateTime/DateTime.h"
#include "Config/ConfigStorage.h"
#include "WpanConfig/WiFiAPManager.h"
#include "WebServer/WebServerHandler.h"
#include "CellularNetwork/SendDataToServes.h"
#include "SatelliteCom/Connection.h"
#include "Generated/generated.h"
#include "Calculated/Calculated.h"

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
Generated generated;
//Calculated calculated;

bool stateSIM;
bool stateGnss;
int count_reset = 0;
int previous_time_update = 0;
int previous_time_send = 0;
int counter_10s = 0;
String message;

void handleSerialInput();
void updateData();
void simInfo();
void gnssInfo();
void gpsInfo(Connection::GPSData gpsData);

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
  generated.initInput(PIN_GPIO_IN);
  generated.initOutput(PIN_GPIO_OUT);
  
}

void loop() {
  stateSIM = false;
  stateGnss = false;
  static bool stateCfgPdp = false;
  handleSerialInput();
  bool ignState = generated.readInput();
  ignState ? 1 : 0;
  if (ignState) {
    generated.turnOn();
  } else {
    generated.turnOff();
  }
  /*Serial.print("Botón presionado: ");
  Serial.println(ignState ? "Sí" : "No");*/

  unsigned long current_time = millis();
  
  if (current_time - previous_time_update >= UPDATE_DATA_TIMEOUT) {
    previous_time_update = current_time;  // Actualizar el tiempo anterior
      dynInfo.getCPSI(); // Validar que no se imprima hasta que tenga los datos "NO" vacíos
      if(!connection.ReadDataGNSS() ){
        dateTimeMS.getDateTime(); // La hora a veces no tiene sentido año 2080 
      }
      //generated.stateIO(); 
      //calculated.stateEvent(); 
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
    //gnssInfo();
    if (current_time - previous_time_send >= SEND_DATA_TIMEOUT) {
      Connection::GPSData gpsData = connection.getLastGPSData();
      //gpsInfo( gpsData);
      previous_time_send = current_time;  // Actualizar el tiempo anterior
        if(!connection.getFix()){
          message = String(HEADER)+SMCLN+modInfo.getDevID()+SMCLN+REPORT_MAP+SMCLN+MODEL_DEVICE+SMCLN+SW_VER+SMCLN+MSG_TYPE+SMCLN
          +dateTimeMS.getValueUTC()+SMCLN+dynInfo.getCellID()+SMCLN+dynInfo.getMCC()+SMCLN+dynInfo.getLAC()+SMCLN+dynInfo.getRxLev()+SMCLN
          +gpsData.latitude+SMCLN+gpsData.longitude+SMCLN+gpsData.speed+SMCLN+gpsData.course+SMCLN+gpsData.gps_svs+SMCLN+connection.getFix();
        }else{
          //crea un modulo donde procese el tipo de reporte a mandar al servidor
          message = String(HEADER)+SMCLN+modInfo.getDevID()+SMCLN+REPORT_MAP+SMCLN+MODEL_DEVICE+SMCLN+SW_VER+SMCLN+MSG_TYPE+SMCLN
          +gpsData.date+SMCLN+gpsData.utc_time+SMCLN+dynInfo.getCellID()+SMCLN+dynInfo.getMCC()+SMCLN+dynInfo.getLAC()+SMCLN+dynInfo.getRxLev()+SMCLN
          +gpsData.latitude+SMCLN+gpsData.longitude+SMCLN+gpsData.speed+SMCLN+gpsData.course+SMCLN+gpsData.gps_svs+SMCLN+connection.getFix()+SMCLN+ignState;
        }
            
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
void gnssInfo(){

  Serial.println("FIX => "+ connection.getFix());
  Serial.println("LAT => "+ String(connection.getLat()) );
  Serial.println("LON => "+ String(connection.getLon()) );
  Serial.println("SPEED => "+ String(connection.getSpeed()) );
  Serial.println("COURSE => "+ String(connection.getCourse()) );
  Serial.println("SATT gps => "+ connection.getSatt());

}
void gpsInfo(Connection::GPSData gpsData){
  Serial.println("Datos GPS:");
      Serial.print("Modo de Fix: ");
    Serial.println(gpsData.mode);
    Serial.print("Satélites GPS: ");
    Serial.println(gpsData.gps_svs);
    Serial.print("Satélites GLONASS: ");
    Serial.println(gpsData.glonass_svs);
    Serial.print("Satélites BEIDOU: ");
    Serial.println(gpsData.beidou_svs);
    /*Serial.printf("Latitud: %+f\n", gpsData.latitude, 6);
    Serial.printf("Longitud: %+f\n", gpsData.longitude, 6);*/
    Serial.print("Fecha (YYYYMMDD): ");
    Serial.println(gpsData.date);
    Serial.print("Hora (HH:MM:SS): ");
    Serial.println(gpsData.utc_time);
    Serial.print("Altitud: ");
    Serial.println(gpsData.altitude);
    Serial.print("Velocidad: ");
    Serial.println(gpsData.speed);
}

void oiState(){

}

