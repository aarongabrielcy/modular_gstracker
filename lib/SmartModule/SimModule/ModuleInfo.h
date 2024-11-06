#ifndef MODULEINFO_H
#define MODULEINFO_H

#include <Arduino.h>
#include "SimModule/SIM7600.h"  // Incluye el encabezado para SIM7600

class ModuleInfo{
public:
    ModuleInfo(SIM7600& sim7600);  // Constructor que recibe una referencia a SIM7600
    String getIMEI();
    String getCCID();
private:
    SIM7600& simModule;  // Referencia al módulo SIM7600(dependencia inyectada)
};
#endif
