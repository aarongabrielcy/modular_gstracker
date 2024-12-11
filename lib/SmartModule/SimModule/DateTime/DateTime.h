#ifndef DATETIME_H
#define DATETIME_H

#include <Arduino.h>
#include "SimModule/SIM7600.h"  // Incluye el encabezado para SIM7600
class DateTime{
public:
    DateTime(SIM7600& sim7600);  // Constructor que recibe una referencia a SIM7600
    String getDateTime();
    String getValueUTC();

private:
    SIM7600& simModule;  // Referencia al módulo SIM7600
    //String dtUTC;
};
#endif
