#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include "cpsi_type.h"

String trimResponse(const String& response);
String getValueCPSI(String command,int type);
String getPositionCPSI(String data,int position);
#endif
