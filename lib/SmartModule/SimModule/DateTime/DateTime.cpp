#include "DateTime.h"
#include "SimModule/SIM7600.h"
#include "Utils/Utils.h"

DateTime::DateTime(SIM7600& sim7600) : simModule(sim7600) {}  // Inicializa la referencia

void DateTime::getDateTime(){
    String dt_cmd = "AT+CCLK?";
    String dt = simModule.sendCommandWithResponse(dt_cmd.c_str(), 4000);
    /*Serial.print("getDateTime MS: ");
    Serial.println(dt);*/
    dtUTC = getFormatUTC(dt);
}

String DateTime::getValueUTC(){ return dtUTC; }
