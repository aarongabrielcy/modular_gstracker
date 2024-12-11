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
#include "Generated/Generated.h"
#include "Calculated/Calculated.h"
#include "PowerOut.h"
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
PowerOut powerOut;

bool stateSIM;
bool stateGnss;
int count_reset = 0;
int previous_time_update = 0;
int previous_time_send = 0;
int counter_10s = 0;
String message;
bool ignState;
String event;
bool LaststateIgnition = HIGH;
int event_ign;

void handleSerialInput();
void updateData();
void simInfo();
void gnssInfo();
void gpsInfo(Connection::GPSData gpsData);
void ignition_event();

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  powerOut.powerModule();
  powerOut.powerKey();

  simModule.begin();
  do{Serial.println("Inicializando Modulo...");}while(!networkManager.initializeModule());
  //stateSIM = networkManager.readSIMInsert();
  networkManager.basicConfigCDMs();
  connection.activeModuleSat(1);
  configStorage.begin();
  
  // Iniciar modo Access Point
  wifiManager.startAP("GST32 AP", "12345678.");//AGREGAR EL IMEI AL NOMBRE PARA IDENTIDICARLOS EJ: GST{imei}

  // Iniciar servidor web
  webServerHandler.begin();
  generated.initializePinsFromJson(INPUTS, INPUTS_ACTIVE);
  generated.initOutput(GNSS_LED_PIN);
  generated.initInput(10);
}

void loop() {
  event_ign = -1;
  stateSIM = false;
  stateGnss = false;
  static bool stateCfgPdp = false;
  static bool stCfgTcp = false;
  handleSerialInput();
  ignition_event();
  
  generated.readInput() ? ignState = 0 : ignState = 1;
  Connection::GPSData gpsData = connection.getLastGPSData();
  /*Serial.print("Botón presionado: ");
  Serial.println(ignState ? "Sí" : "No");*/

  unsigned long current_time = millis();
  
  if (current_time - previous_time_update >= UPDATE_DATA_TIMEOUT) {
    previous_time_update = current_time;  // Actualizar el tiempo anterior
      dynInfo.getCPSI(); // Validar que no se imprima hasta que tenga los datos "NO" vacíos
      if(!connection.ReadDataGNSS() ){
        dateTimeMS.getDateTime(); // La hora a veces no tiene sentido año 2080 
        
      }
      generated.readPinsFromJson(INPUTS_ACTIVE);
      //generated.stateIO(); 
      //calculated.stateEvent(); 
  }
  //webServerHandler.serviceToApp();
  // Manejar clientes HTTP
  webServerHandler.handleClient();
  if(!stateSIM) {
    //Serial.println("Validando estado de la SIMCARD");
    stateSIM = networkManager.readSIMInsert();
    if(!stateSIM){
      //reincio suave al módulo sim
      count_reset++;
      Serial.println("** SIM no insertada! => ");
      if(count_reset == 12 ) {
        Serial.println("Reinicio suave del modulo: "+networkManager.softReset());
        count_reset = 0;
        stateCfgPdp = false;
      }
      delay(1000);
    }else{
      //Serial.println("## SIM insertada!");
      delay(1000);
    }
  }
  if(!stateCfgPdp && stateSIM){
    Serial.println("configurando PDP");
    stateCfgPdp = networkManager.configurePDP(TELCEL, 1);
  }
  if(stateCfgPdp && stateSIM){
    if(sendDataToServes.validTcpNet() && !stCfgTcp) {
      if(webServerHandler.handleRoot() ){
       stCfgTcp = sendDataToServes.configureTCP(webServerHandler.getServerIP(), webServerHandler.getServerPort() );
      }
    }
    //simInfo();
    //gnssInfo();
    if (current_time - previous_time_send >= SEND_DATA_TIMEOUT) {
      
      //gpsInfo( gpsData);
      previous_time_send = current_time;  // Actualizar el tiempo anterior
        if(!connection.getFix() ) {
          message = String(Headers::STT)+SMCLN+modInfo.getDevID()+SMCLN+REPORT_MAP+SMCLN+MODEL_DEVICE+SMCLN+SW_VER+SMCLN+MSG_TYPE+SMCLN
          +dateTimeMS.getValueUTC()+SMCLN+dynInfo.getCellID()+SMCLN+dynInfo.getMCC()+SMCLN+dynInfo.getMNC()+SMCLN+dynInfo.getLAC()+SMCLN+dynInfo.getRxLev()+SMCLN
          +gpsData.latitude+SMCLN+gpsData.longitude+SMCLN+gpsData.speed+SMCLN+gpsData.course+SMCLN+gpsData.gps_svs+SMCLN+connection.getFix()+SMCLN
          +generated.ioState.in7+generated.ioState.in6+generated.ioState.in5+generated.ioState.in4+generated.ioState.in3+generated.ioState.in2
          +generated.ioState.in1+ignState+SMCLN+generated.ioState.rsv3+generated.ioState.rsv2+generated.ioState.rsv1+generated.ioState.ou5
          +generated.ioState.ou4+generated.ioState.ou3+generated.ioState.ou2+generated.ioState.ou1+SMCLN+CADENA_FALTANTE;
        }else {
          Serial.println("########### Trackeo con fix ############");
          //crea un modulo donde procese el tipo de reporte a mandar al servidor
          message = String(Headers::STT)+SMCLN+modInfo.getDevID()+SMCLN+REPORT_MAP+SMCLN+MODEL_DEVICE+SMCLN+SW_VER+SMCLN+MSG_TYPE+SMCLN
          +gpsData.date+SMCLN+gpsData.utc_time+SMCLN+dynInfo.getCellID()+SMCLN+dynInfo.getMCC()+SMCLN+dynInfo.getMNC()+SMCLN+dynInfo.getLAC()+SMCLN+dynInfo.getRxLev()+SMCLN
          +gpsData.latitude+SMCLN+gpsData.longitude+SMCLN+gpsData.speed+SMCLN+gpsData.course+SMCLN+gpsData.gps_svs+SMCLN+connection.getFix()+SMCLN
          +generated.ioState.in7+generated.ioState.in6+generated.ioState.in5+generated.ioState.in4+generated.ioState.in3+generated.ioState.in2
          +generated.ioState.in1+ignState+SMCLN+generated.ioState.rsv3+generated.ioState.rsv2+generated.ioState.rsv1+generated.ioState.ou5
          +generated.ioState.ou4+generated.ioState.ou3+generated.ioState.ou2+generated.ioState.ou1+SMCLN+CADENA_FALTANTE;
        }
      if(!sendDataToServes.sendData(message)) {
        stCfgTcp = false;
      }
      //Leer aquí respuesta del servidor   
      //Serial.println(sendDataToServes.sendData(message) );
    }
    
  }
  //// EVENT GENERATED
  if (ignState == 1 && event_ign == 1) {
    Serial.println("Engine ON");
    if(!connection.getFix() ) {
      event = String("ALT")+SMCLN+modInfo.getDevID()+SMCLN+REPORT_MAP+SMCLN+MODEL_DEVICE+SMCLN+SW_VER+SMCLN+MSG_TYPE+SMCLN
          +dateTimeMS.getValueUTC()+SMCLN+dynInfo.getCellID()+SMCLN+dynInfo.getMCC()+SMCLN+dynInfo.getMNC()+SMCLN+dynInfo.getLAC()+SMCLN+dynInfo.getRxLev()+SMCLN
          +gpsData.latitude+SMCLN+gpsData.longitude+SMCLN+gpsData.speed+SMCLN+gpsData.course+SMCLN+gpsData.gps_svs+SMCLN+connection.getFix()+SMCLN
          +generated.ioState.in7+generated.ioState.in6+generated.ioState.in5+generated.ioState.in4+generated.ioState.in3+generated.ioState.in2
          +generated.ioState.in1+ignState+SMCLN+generated.ioState.rsv3+generated.ioState.rsv2+generated.ioState.rsv1+generated.ioState.ou5
          +generated.ioState.ou4+generated.ioState.ou3+generated.ioState.ou2+generated.ioState.ou1+SMCLN+33+";;";
    }else{
      event = String("ALT")+SMCLN+modInfo.getDevID()+SMCLN+REPORT_MAP+SMCLN+MODEL_DEVICE+SMCLN+SW_VER+SMCLN+MSG_TYPE+SMCLN
          +gpsData.date+SMCLN+gpsData.utc_time+SMCLN+dynInfo.getCellID()+SMCLN+dynInfo.getMCC()+SMCLN+dynInfo.getMNC()+SMCLN+dynInfo.getLAC()+SMCLN+dynInfo.getRxLev()+SMCLN
          +gpsData.latitude+SMCLN+gpsData.longitude+SMCLN+gpsData.speed+SMCLN+gpsData.course+SMCLN+gpsData.gps_svs+SMCLN+connection.getFix()+SMCLN
          +generated.ioState.in7+generated.ioState.in6+generated.ioState.in5+generated.ioState.in4+generated.ioState.in3+generated.ioState.in2
          +generated.ioState.in1+ignState+SMCLN+generated.ioState.rsv3+generated.ioState.rsv2+generated.ioState.rsv1+generated.ioState.ou5
          +generated.ioState.ou4+generated.ioState.ou3+generated.ioState.ou2+generated.ioState.ou1+SMCLN+33+";;";
    }
    

    generated.turnOn();
    //Serial.println("Event => "+ event);
    sendDataToServes.sendData(event);
  } else if(ignState == 0 && event_ign == 0) {
    Serial.println("Engine Off");
    if(!connection.getFix() ) {
      event = String("ALT")+SMCLN+modInfo.getDevID()+SMCLN+REPORT_MAP+SMCLN+MODEL_DEVICE+SMCLN+SW_VER+SMCLN+MSG_TYPE+SMCLN
          +dateTimeMS.getValueUTC()+SMCLN+dynInfo.getCellID()+SMCLN+dynInfo.getMCC()+SMCLN+dynInfo.getMNC()+SMCLN+dynInfo.getLAC()+SMCLN+dynInfo.getRxLev()+SMCLN
          +gpsData.latitude+SMCLN+gpsData.longitude+SMCLN+gpsData.speed+SMCLN+gpsData.course+SMCLN+gpsData.gps_svs+SMCLN+connection.getFix()+SMCLN
          +generated.ioState.in7+generated.ioState.in6+generated.ioState.in5+generated.ioState.in4+generated.ioState.in3+generated.ioState.in2
          +generated.ioState.in1+ignState+SMCLN+generated.ioState.rsv3+generated.ioState.rsv2+generated.ioState.rsv1+generated.ioState.ou5
          +generated.ioState.ou4+generated.ioState.ou3+generated.ioState.ou2+generated.ioState.ou1+SMCLN+34+";;";
    }else {
      event = String("ALT")+SMCLN+modInfo.getDevID()+SMCLN+REPORT_MAP+SMCLN+MODEL_DEVICE+SMCLN+SW_VER+SMCLN+MSG_TYPE+SMCLN
          +gpsData.date+SMCLN+gpsData.utc_time+SMCLN+dynInfo.getCellID()+SMCLN+dynInfo.getMCC()+SMCLN+dynInfo.getMNC()+SMCLN+dynInfo.getLAC()+SMCLN+dynInfo.getRxLev()+SMCLN
          +gpsData.latitude+SMCLN+gpsData.longitude+SMCLN+gpsData.speed+SMCLN+gpsData.course+SMCLN+gpsData.gps_svs+SMCLN+connection.getFix()+SMCLN
          +generated.ioState.in7+generated.ioState.in6+generated.ioState.in5+generated.ioState.in4+generated.ioState.in3+generated.ioState.in2
          +generated.ioState.in1+ignState+SMCLN+generated.ioState.rsv3+generated.ioState.rsv2+generated.ioState.rsv1+generated.ioState.ou5
          +generated.ioState.ou4+generated.ioState.ou3+generated.ioState.ou2+generated.ioState.ou1+SMCLN+34+";;";
    }
    generated.turnOff();
    //Serial.println("Event => "+ event);
    sendDataToServes.sendData(event);

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
void ignition_event() {
  int StateIgnition = generated.readInput();
  if (StateIgnition == LOW && LaststateIgnition == HIGH) {
    Serial.println("*** ¡ignition ON! **** ");
    event_ign = 1;
  
  }else if(StateIgnition == HIGH && LaststateIgnition == LOW) {
    Serial.println("**** ¡ignition OFF! ***** ");
    event_ign = 0;
  }
  LaststateIgnition = StateIgnition;
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

