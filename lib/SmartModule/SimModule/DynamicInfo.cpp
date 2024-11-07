#include "DynamicInfo.h"
#include "SimModule/SIM7600.h"
#include "Utils/Utils.h"

DynamicInfo::DynamicInfo(SIM7600& sim7600) : simModule(sim7600) {}  // Inicializa la referencia

void DynamicInfo::getCPSI(){
    String cpsi_cmd = "AT+CPSI?";
    String cpsi = simModule.sendCommandWithResponse(cpsi_cmd.c_str(), 4000);
    mcc = formatCPSI(cpsi, MCC); //cambiar mcc por enum o algo dinamico para meterlo en un switch en la funcion que ira en formatCPSI y formatCPSI debe estar en utils.h y debe retornar un string
    mnc = formatCPSI(cpsi, MNC);
    lac = formatCPSI(cpsi, LAC);
    rxLev = formatCPSI(cpsi, RXLEV);
}

String DynamicInfo::getMCC() { return mcc; }
String DynamicInfo::getMNC() { return mnc; }
String DynamicInfo::getLAC() { return lac; }
String DynamicInfo::getCellID() { return cellID; }
String DynamicInfo::getRxLev() { return rxLev;}