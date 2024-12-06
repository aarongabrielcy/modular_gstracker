#ifndef COMMAND_TYPE_H
#define COMMAND_TYPE_H

enum cmdResponse {
  ROK = 0,
  ERROR = 1,
  DATA = 2
};

enum cmdType {
  TEST = 0,
  READ = 1,
  WRITE = 2,
  EXECUTE = 3,
  SEND = 4,
  UNKNOWN = -1
};
//clasificar los comandos por lectura, escritura, ejecución y test
enum commands7600 {
  CQS = 1,
  CFUN    ,
  CCID    ,
  CCLK    ,
  COPS    ,
  CPSI    ,
  CGPS    ,
  SIMEI   ,
  CGDCONT ,
  CIPOPEN ,
  NETOPEN ,
  CGPSINFO
};

enum CMEErrorCodes {
  PhoneFailure,
  NoConnectionToPhone,
  PhoneAdaptorLinkReserved,
  OperationNotAllowed,
  SIMFailure = 13,
  MemoryFull = 20,
  NoNetworkService = 30,
  NetworkTimeout = 31,
  Unknown = 100,
  //final 260+
};
#endif
