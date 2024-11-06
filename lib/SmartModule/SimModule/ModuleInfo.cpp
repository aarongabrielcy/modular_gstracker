#include "ModuleInfo.h"
#include "SimModule/SIM7600.h"
ModuleInfo::ModuleInfo(SIM7600& sim7600) : simModule(sim7600) {}  // Inicializa la referencia (// Inyección de dependencia aquí)

String ModuleInfo::getIMEI(){
    String imei_cmd = "AT+SIMEI?";
    String imei = simModule.sendCommandWithResponse(imei_cmd.c_str(), 4000);
    return imei;
}
String ModuleInfo::getCCID(){
    String ccid_cmd = "AT+CCID";
    String ccid = simModule.sendCommandWithResponse(ccid_cmd.c_str(), 4000);
    return ccid;
}


