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
#include "SleepMode/Sleep.h"

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
Sleep mySleep(simModule);

unsigned long current_time = 0;
unsigned long previous_time_send = 0;
unsigned long previous_time_ign_off = 0;
const unsigned long sendDataTimeout = SEND_DATA_TIMEOUT;
const unsigned long sendDataIgnOff = SEND_DATA_ING_OFF;
bool ledState = LOW;    
String message;
bool ignState;
bool LaststateIgnition = HIGH;
bool fix;
String datetime;
float previousCourse = -1.0;
int trackingCourse = 0;

void handleSerialInput();
void updateData();
void simInfo();
void gnssInfo();
void gpsInfo(Connection::GPSData gpsData);
void ignition_event(Connection::GPSData gpsData);
void event_generated(Connection::GPSData gpsData, int event);
//void enterDeepSleep();
void connectionServices();

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  powerOut.powerModule();
  powerOut.powerKey();
  configStorage.begin();
  /*mySleep.InitSleepPin(SIM_DTR_PIN);
  mySleep.DeactivateSleep(SIM_DTR_PIN);*/
  simModule.begin();
  do{Serial.println("Inicializando Modulo...");}while(!networkManager.initializeModule());
  //stateSIM = networkManager.readSIMInsert();
  //connection.activeModuleSat(1);// crear una funcion para auto activar AT+CGPS=1
  String ssid = SSID_NAME+modInfo.getDevID();
  wifiManager.startAP(ssid.c_str(), AP_PASSWORD);//AGREGAR EL IMEI AL NOMBRE PARA IDENTIDICARLOS EJ: GST{imei}
  // Iniciar servidor web
  webServerHandler.begin();
  networkManager.basicConfigCDMs();
  do{Serial.println("Activando GNSS...");}while(!connection.stateGPS() );
  connectionServices();
  generated.initializePinsFromJson(INPUTS, INPUTS_ACTIVE);
  generated.initOutput(GNSS_LED_PIN);
  generated.initInput(10);
}

void loop() {
  static bool stCfgTcp = false;
  fix  = connection.ReadDataGNSS();
  Connection::GPSData gpsData = connection.getLastGPSData();
  handleSerialInput();
  dynInfo.getCPSI(); // Validar que no se imprima hasta que tenga los datos "NO" vacíos
  generated.readPinsFromJson(INPUTS_ACTIVE);

  fix ?  generated.GnssLED(GNSS_LED_PIN, true) : generated.GnssLedBlink(GNSS_LED_PIN, ledState); 
  generated.readInput() ? ignState = 0 : ignState = 1;
  ignition_event(gpsData);
  //bool inSleepMode = mySleep.StateSleep(SIM_DTR_PIN);
  current_time = millis();
  webServerHandler.handleClient();

  if(!fix ){
    datetime = dateTimeMS.getDateTime(); // La hora a veces no tiene sentido año 2080
  }else{
    datetime = gpsData.date+SMCLN+gpsData.utc_time;
  }

  message = String(Headers::STT)+SMCLN+modInfo.getDevID()+SMCLN+REPORT_MAP+SMCLN+MODEL_DEVICE+SMCLN+SW_VER+SMCLN+MSG_TYPE+SMCLN
        +datetime+SMCLN+dynInfo.getCellID()+SMCLN+dynInfo.getMCC()+SMCLN+dynInfo.getMNC()+SMCLN+dynInfo.getLAC()+SMCLN+dynInfo.getRxLev()+SMCLN
        +gpsData.latitude+SMCLN+gpsData.longitude+SMCLN+gpsData.speed+SMCLN+gpsData.course+SMCLN+gpsData.gps_svs+SMCLN+fix+SMCLN
        +trackingCourse+generated.ioState.in6+generated.ioState.in5+generated.ioState.in4+generated.ioState.in3+generated.ioState.in2
        +generated.ioState.in1+ignState+SMCLN+generated.ioState.rsv3+generated.ioState.rsv2+generated.ioState.rsv1+generated.ioState.ou5
        +generated.ioState.ou4+generated.ioState.ou3+generated.ioState.ou2+generated.ioState.ou1+SMCLN+CADENA_FALTANTE;


  if (previousCourse != -1.0) {  // Verificar que no sea la primera lectura
    float difference = abs(gpsData.course - previousCourse);  
    if (difference >= SEND_DATA_ANGLE) {
      trackingCourse = 1;
      Serial.print("Cambio significativo detectado en course: ");
      Serial.println(difference);
      
      if (!sendDataToServes.sendData(message)) {
        connectionServices();
      }
      // Volver al inicio del loop sin ejecutar el resto del código
      previousCourse = gpsData.course; // Actualizar antes de retornar
      return;
    }
  }
  trackingCourse = 0;
  if(current_time - previous_time_send >= sendDataTimeout && ignState == 1) {

    previous_time_send = current_time;  // Actualizar el tiempo anterior
        
    if(!sendDataToServes.sendData(message)) {
      connectionServices();
    }
  }
  if(current_time - previous_time_ign_off >= sendDataIgnOff && ignState == 0) {
      //gpsInfo( gpsData);
    previous_time_ign_off = current_time;  // Actualizar el tiempo anterior
    Serial.println("tiempo transcurrido con motor apagado => "); 
    if(!sendDataToServes.sendData(message)) {
      connectionServices();
    }
  }
}

void handleSerialInput() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    String response = simModule.sendCommandWithResponse(command.c_str(), AT_COMMAND_TIMEOUT);
    Serial.print("Respuesta limpia: ");
    Serial.println(response);
  }
}
void ignition_event(Connection::GPSData gpsData) {
  int StateIgnition = generated.readInput();
  if (StateIgnition == LOW && LaststateIgnition == HIGH) {
    Serial.println("*** ¡ignition ON! **** ");

    event_generated(gpsData, IGN_ON);
    
  }else if(StateIgnition == HIGH && LaststateIgnition == LOW) {
    Serial.println("**** ¡ignition OFF! ***** ");

    event_generated(gpsData, IGN_OFF);
  }
  LaststateIgnition = StateIgnition;
}
void event_generated(Connection::GPSData gpsData, int event){

  String data_event = "";
  
  data_event = String(Headers::ALT)+SMCLN+modInfo.getDevID()+SMCLN+REPORT_MAP+SMCLN+MODEL_DEVICE+SMCLN+SW_VER+SMCLN+MSG_TYPE+SMCLN
      +datetime+SMCLN+dynInfo.getCellID()+SMCLN+dynInfo.getMCC()+SMCLN+dynInfo.getMNC()+SMCLN+dynInfo.getLAC()+SMCLN+dynInfo.getRxLev()+SMCLN
      +gpsData.latitude+SMCLN+gpsData.longitude+SMCLN+gpsData.speed+SMCLN+gpsData.course+SMCLN+gpsData.gps_svs+SMCLN+fix+SMCLN
      +trackingCourse+generated.ioState.in6+generated.ioState.in5+generated.ioState.in4+generated.ioState.in3+generated.ioState.in2
      +generated.ioState.in1+ignState+SMCLN+generated.ioState.rsv3+generated.ioState.rsv2+generated.ioState.rsv1+generated.ioState.ou5
      +generated.ioState.ou4+generated.ioState.ou3+generated.ioState.ou2+generated.ioState.ou1+SMCLN+event+";;";
 
  Serial.println("EVENT => "+data_event); 
  sendDataToServes.sendData(data_event);
}

void connectionServices(){
  do{Serial.println("Conectando a la RED Celular...");}while(!networkManager.configurePDP(TELCEL, 1) );
  do{Serial.println("Activando servicio TCP...");}while(!sendDataToServes.validTcpNet() );
  do{Serial.println("Leyendo IP y Puerto en memoria ... ");}while(!webServerHandler.handleRoot());
  do{Serial.println("Conectando al servidor TCP...");}while (!sendDataToServes.configureTCP(webServerHandler.getServerIP(), webServerHandler.getServerPort() ));

}

