#ifndef SIM7600_H
#define SIM7600_H

#include <Arduino.h>
#include "command_type.h"
#include "config.h"

class SIM7600 {
public:
  SIM7600(HardwareSerial& serial);
  void begin();
  String sendCommandWithResponse(const char* command, int timeout = AT_COMMAND_TIMEOUT);

private:
  HardwareSerial& simSerial;
  bool isAllowedCommand(const String& command);
  String processResponse(const String& command, const String& response);
  int commandType(const String& command);

  static const char* allowedCommands[7];  // Declaración de miembro estático
};

#endif
