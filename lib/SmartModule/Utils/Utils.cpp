#include "Utils.h"

String trimResponse(const String& response) {
  String result = response;      // Hacemos una copia modificable de response
  result.replace("\n", "");      // Elimina saltos de línea
  result.replace("\r", "");      // Elimina retornos de carro
  result.trim();                 // Elimina espacios en blanco al inicio y al final
  return result;
}

String getValueCPSI(String data, int position) {
    // Llama a getPositionCPSI una vez y almacena el resultado en lastData
    String lastData = (position != -1)? getPositionCPSI(data, position) : getPositionCPSI(data, 2);
    Serial.print("Last data: ");
    Serial.println(lastData);
    // Verifica la posición y procesa según el caso
    switch (position) {
        case CELLID:
            // Extrae el MNC después del guión "-"
            return lastData;

        case MCC:
            // Extrae el MCC antes del guión "-"
            return lastData.substring(0, lastData.indexOf("-"));

        case MNC:
            // Extrae el MNC después del guión "-"
            return lastData.substring(lastData.indexOf("-") + 1);

        case LAC:
            // Si lastData comienza con "0x", elimina ese prefijo
            if (lastData.startsWith("0x")) {
                return lastData.substring(2);  // Devuelve sin el "0x"
            }
            return lastData;
        case RXLEV:
            return lastData;

        default:
            // Controla casos fuera de rango de manera más informativa
            return "null";
    }
}

String getPositionCPSI(String data,int position){
  int startIndex = 0;
    int endIndex = 0;
    int currentPosition = 0;
    // Recorre la cadena buscando comas para separar en posiciones
    while (currentPosition <= position) {
        endIndex = data.indexOf(',', startIndex);  // Encuentra la siguiente coma

        // Si no encuentra más comas y estamos en la última posición, ajusta endIndex
        if (endIndex == -1) endIndex = data.length();

        // Si la posición actual coincide con la buscada, extrae el valor
        if (currentPosition == position) {
            return data.substring(startIndex, endIndex);  // Devuelve el valor en la posición deseada
        }

        // Actualiza el índice de inicio para la siguiente iteración
        startIndex = endIndex + 1;
        currentPosition++;
    }

    return "";
}





