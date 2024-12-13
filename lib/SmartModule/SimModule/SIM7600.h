#ifndef SIM7600_H
#define SIM7600_H

#include <Arduino.h>
#include "command_type.h"
#include "config.h"
#include "Utils/Utils.h"

class SIM7600 {
public:
  SIM7600(HardwareSerial& serial);
  void begin();
  String sendCommandWithResponse(const char* command, int timeout = AT_COMMAND_TIMEOUT);
  String processResponse(const String& command, const String& fcommand, const String& response);
  String sendReadDataToServer(const String& fcommand, const String& message, int timeout);

private:
  HardwareSerial& simSerial;
  bool isAllowedCommand(const String& command);
  int commandType(const String& command);
  String processDataToServer(String Response);

  static const char* allowedCommands[34];  // Declaración de miembro estático
};

#endif
