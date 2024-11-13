#include "DynamicInfo.h"
#include "SimModule/SIM7600.h"
#include "Utils/Utils.h"

DynamicInfo::DynamicInfo(SIM7600& sim7600) : simModule(sim7600) {}  // Inicializa la referencia

void DynamicInfo::getCPSI(){
    String cpsi_cmd = "AT+CPSI?";
    String cpsi = simModule.sendCommandWithResponse(cpsi_cmd.c_str(), 4000);
    /*Serial.print("getCPSI: ");
    Serial.println(cpsi);*/
    cellID = getValueCPSI(cpsi, CELLID);
    mcc = getValueCPSI(cpsi, MCC);
    mnc = getValueCPSI(cpsi, MNC);
    lac = getValueCPSI(cpsi, LAC);
    rxLev = getValueCPSI(cpsi, RXLEV);
}

String DynamicInfo::getMCC() { return mcc; }
String DynamicInfo::getMNC() { return mnc; }
String DynamicInfo::getLAC() { return lac; }
String DynamicInfo::getCellID() { return cellID; }
String DynamicInfo::getRxLev() { return rxLev; }