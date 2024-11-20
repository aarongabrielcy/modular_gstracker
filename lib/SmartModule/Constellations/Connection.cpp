#include "connection.h"
#include "SimModule/SIM7600.h"

Connection::Connection(SIM7600& sim7600) : simModule(sim7600) {}

bool Connection::activeModuleSat(int state){
    String cgps_cmd = "AT+CGPS="+String(state);
    //consulta antes de activar de nuevo
    String cgps = simModule.sendCommandWithResponse(cgps_cmd.c_str(), 4000);
    if(cgps == "OK"){
        return true;
    }else{
        Serial.println("Resp CGPS state => "+cgps);
        return false;
    }
}

String Connection::ReadDataGNSS(){
    String cgpsinfo_cmd = "AT+CGPSINFO";
    String cgpsinfo = simModule.sendCommandWithResponse(cgpsinfo_cmd.c_str(), 4000);
    if(cgpsinfo == ",,,,,,,,"){
        fix = 0;
        Serial.println("GNSS DATA DEBUG => "+ String(GNSS_DEBUG) );
        return GNSS_DEBUG;
    }else{
        Serial.println("GNSS DATA => "+ cgpsinfo);
        return cgpsinfo;
    }
}
String Connection::ReadDataGPS(){
    String cgnssinfo_cmd = "AT+CGNSSINFO";
    String cgnssinfo = simModule.sendCommandWithResponse(cgnssinfo_cmd.c_str(), 4000);
    return cgnssinfo;
}

int Connection::getFix() { return fix; }
