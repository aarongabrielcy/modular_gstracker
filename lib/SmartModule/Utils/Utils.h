#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include "cpsi_type.h"
#include "littles_emus.h"

String trimResponse(const String& response);
String getValueCPSI(String command,int type);
String getPositionData(String data,int position);
String getFormatUTC(String datetime);
#endif
