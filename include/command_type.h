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

#endif
