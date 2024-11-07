#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include "cpsi_type.h"

String trimResponse(const String& response);
String formatCPSI(String command,int type);
#endif
