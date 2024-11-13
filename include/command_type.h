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

#endif
