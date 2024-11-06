#include "ATCommands.h"

String formatATCommand(String command) {
  return "AT+" + command;
}
