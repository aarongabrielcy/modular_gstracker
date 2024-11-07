#include "Utils.h"

String trimResponse(const String& response) {
  String result = response;      // Hacemos una copia modificable de response
  result.replace("\n", "");      // Elimina saltos de línea
  result.replace("\r", "");      // Elimina retornos de carro
  result.trim();                 // Elimina espacios en blanco al inicio y al final
  return result;
}

String formatCPSI(String command,int type){
  
   switch (type){
    case MCC:

      break;
    case MNC:

      break;
    case LAC:

      break;
    case RXLEV:

      break;
    default:
      break;
   }
}



