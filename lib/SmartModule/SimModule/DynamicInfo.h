#ifndef DYNAMICINFO_H
#define DYNAMICINFO_H

#include <Arduino.h>
#include "SimModule/SIM7600.h"  // Incluye el encabezado para SIM7600

class DynamicInfo{
public:
    DynamicInfo(SIM7600& sim7600);  // Constructor que recibe una referencia a SIM7600
    void getCPSI();
    String getMCC();
    String getMNC();
    String getLAC();
    String getCellID();
    String getRxLev();  

private:
    SIM7600& simModule;  // Referencia al módulo SIM7600
    String mcc, mnc, lac, cellID, rxLev;

};
#endif
