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
    String lastData = (position != -1)? getPositionData(data, position) : getPositionData(data, 2);
    /*Serial.print("Last data: ");
    Serial.println(lastData);*/
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
String getFormatUTC(String dt){
    String date = getPositionData(dt ,DATE);
    date.replace("\"", "");
    String timeWithTZ = getPositionData(dt, TIME);
    timeWithTZ.replace("\"", "");
    //Serial.println("dateTime UTC-6 => "+ date +";"+timeWithTZ);
     // Extraer fecha (formato recibido: "yy/MM/dd")
    int year = 2000 + date.substring(0, 2).toInt(); // Convertir "yy" en "yyyy"
    int month = date.substring(3, 5).toInt();
    int day = date.substring(6, 8).toInt();

    Serial.println("YEAR: "+ year);
    Serial.println("MONTH: "+ month);
    Serial.println("DAY: "+ day);

    // Extraer hora (formato recibido: "hh:mm:ss±zz")
    int hour = timeWithTZ.substring(0, 2).toInt();
    int minute = timeWithTZ.substring(3, 5).toInt();
    int second = timeWithTZ.substring(6, 8).toInt();

    // Extraer y convertir zona horaria (en cuartos de hora)
    int timezoneOffset = timeWithTZ.substring(9).toInt() * 15; // ±zz en minutos
    int offsetHours = timezoneOffset / 60;
    int offsetMinutes = timezoneOffset % 60;

    // Ajuste a UTC sumando la diferencia horaria
    hour += offsetHours;
    minute += offsetMinutes;
    
    // Ajuste de minutos y horas si se supera el límite de 60 o 24
    if (minute >= 60) {
        hour += minute / 60;
        minute %= 60;
    } else if (minute < 0) {
        hour -= 1;
        minute += 60;
    }

    if (hour >= 24) {
        hour %= 24;
        day += 1;  // Incrementa el día si la hora supera las 24 horas
    } else if (hour < 0) {
        hour += 24;
        day -= 1;  // Ajuste si la hora es negativa
    }

    // Formatear la fecha y la hora en el estilo esperado
    String formattedDate = String(year) + (month < 10 ? "0" : "") + String(month) + (day < 10 ? "0" : "") + String(day);
    String formattedTime = (hour < 10 ? "0" : "") + String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + ":" + (second < 10 ? "0" : "") + String(second);
    // Retorna la fecha y hora en formato UTC
    return formattedDate + ";" + formattedTime;
}
String getPositionData(String data,int position){
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