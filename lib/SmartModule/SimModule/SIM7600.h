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
  String processResponse(const String& command, const String& fcommand, const String& response);

private:
  HardwareSerial& simSerial;
  bool isAllowedCommand(const String& command);
  int commandType(const String& command);

  static const char* allowedCommands[21];  // Declaración de miembro estático
};

#endif
