#include "connection.h"
#include "SimModule/SIM7600.h"

Connection::Connection(SIM7600& sim7600) : simModule(sim7600) {}


bool Connection::activeModuleSat(int state){
    String cgps_cmd = "AT+CGPS="+String(state);
    //consulta antes de activar de nuevo
    String cgps = simModule.sendCommandWithResponse(cgps_cmd.c_str(), 4000);
    if(cgps == "OK"){
        return true;
    }else{
        Serial.println("Resp CGPS state => "+cgps);
        return false;
    }
}
int activeConstellation(){ 
    return 0;
}
void Connection::ReadDataGNSS(){
    String cgpsinfo_cmd = "AT+CGPSINFO";
    String cgpsinfo = simModule.sendCommandWithResponse(cgpsinfo_cmd.c_str(), 4000);
    if(cgpsinfo == ",,,,,,,,"){
        DEBUG == true? fix = 1 : fix = 0;
        Serial.println("GNSS DATA DEBUG => "+ String(GNSS_DEBUG) );
        GPSData parsedData = ParseData(GNSS_DEBUG);
        //Connection::printGPSData(parsedData);
    }else{
        fix = 1;
        Serial.println("GNSS DATA => "+ cgpsinfo);
        GPSData parsedData = ParseData(cgpsinfo);
    }
}
Connection::GPSData Connection::ParseData(const String &data){
    GPSData gpsData;
    int index = 0;
    String tokens[16]; // Array para almacenar las partes de la cadena

    // Dividir la cadena en partes usando ',' como delimitador
    for (int i = 0; i < data.length(); i++) {
        if (data[i] == ',' || i == data.length() - 1) {
            if (i == data.length() - 1) tokens[index] += data[i];
            index++;
        } else {
            tokens[index] += data[i];
        }
    }

    // Asignar los valores a la estructura
    gpsData.mode = tokens[0].toInt();
    gpsData.gps_svs = tokens[1].toInt();
    gpsData.glonass_svs = tokens[2].toInt();
    gpsData.beidou_svs = tokens[3].toInt();
    gpsData.latitude = !tokens[4].isEmpty() ? formatCoordinates(tokens[4], tokens[5][0]) : "0.0";
    gpsData.ns_indicator = tokens[5][0];
    gpsData.longitude = !tokens[6].isEmpty() ? formatCoordinates(tokens[6], tokens[7][0]) : "0.0";
    gpsData.ew_indicator = tokens[7][0];
    gpsData.date = formatDate(tokens[8]);      // Formatear la fecha
    gpsData.utc_time = formatTime(tokens[9]);  // Formatear la hora
    gpsData.altitude = tokens[10].toFloat();
    gpsData.speed = tokens[11].toFloat();
    gpsData.course = tokens[12].toFloat();
    gpsData.pdop = tokens[13].toFloat();
    gpsData.hdop = tokens[14].toFloat();
    gpsData.vdop = tokens[15].toFloat();

    //datos a mandar al servidor
    /*vlat = gpsData.latitude;
    vlon = gpsData.longitude;
    vspeed = gpsData.speed;
    gpsData.course = vcourse;
    gpsData.gps_svs = num_satt;*/ //este valor debe ser dinámico e indicarle desde la app de configuración que consteleción va a mandar este dato (GPS, GALILEO, GLONASS)
    //serializar "gpsData" y mandar en la app para ver los datos en tiempo real de GPS
    // Guardar el último gpsData procesado
    lastGPSData = gpsData;
    return gpsData;
}

/*float Connection::formatCoordinates(const String &coord, char direction) {
    int degreesLength = (coord.indexOf('.') > 4) ? 3 : 2; // Verificar si son 2 o 3 dígitos para grados
    int degrees = coord.substring(0, degreesLength).toInt();
    float minutes = coord.substring(degreesLength).toFloat();
    float decimalDegrees = degrees + (minutes / 60.0);
    return (direction == 'S' || direction == 'W') ? -decimalDegrees : decimalDegrees;
}*/

String Connection::formatCoordinates(const String &coord, char direction) {
    int degreesLength = (coord.indexOf('.') > 4) ? 3 : 2; // Verificar si son 2 o 3 dígitos para grados
    int degrees = coord.substring(0, degreesLength).toInt();
    float minutes = coord.substring(degreesLength).toFloat();
    float decimalDegrees = degrees + (minutes / 60.0);

    // Aplicar signo dependiendo de la dirección
    if (direction == 'S' || direction == 'W') {
        decimalDegrees = -decimalDegrees;
    }

    // Formatear el resultado como string con signo explícito
    char buffer[12]; // Tamaño suficiente para contener coordenadas con precisión
    snprintf(buffer, sizeof(buffer), "%+.6f", decimalDegrees); // "%+" añade el signo explícito
    return String(buffer);
}

String dataGlonass(){
    return "";
}

String dataGalileo(){
    return "";
}
String dataGps(){
    return "";
}
String Connection::formatDate(const String &date) {
    // Formato de entrada: DDMMYY (e.g., 141124)
    // Formato de salida: YYYYMMDD (e.g., 20241121)
    String year = "20" + date.substring(4, 6); // Asume que el año es 20XX
    String month = date.substring(2, 4);
    String day = date.substring(0, 2);
    return year + month + day;
}

String Connection::formatTime(const String &utcTime) {
    // Formato de entrada: HHMMSS.s (e.g., 040641.0)
    // Formato de salida: HH:MM:SS (e.g., 04:06:41)
    String hours = utcTime.substring(0, 2);
    String minutes = utcTime.substring(2, 4);
    String seconds = utcTime.substring(4, 6);
    return hours + ":" + minutes + ":" + seconds;
}

// Método para imprimir los datos GPS
void Connection::printGPSData(const GPSData &data) {
    Serial.println("===== Datos GPS Parseados =====");
    Serial.print("Modo de Fix: ");
    Serial.println(data.mode);
    Serial.print("Satélites GPS: ");
    Serial.println(data.gps_svs);
    Serial.print("Satélites GLONASS: ");
    Serial.println(data.glonass_svs);
    Serial.print("Satélites BEIDOU: ");
    Serial.println(data.beidou_svs);
    Serial.print("Latitud: ");
    Serial.print(data.latitude);
    Serial.println();
    Serial.print("Longitud: ");
    Serial.print(data.longitude);
    Serial.println();
    Serial.print("Fecha (DDMMYY): ");
    Serial.println(data.date);
    Serial.print("Hora UTC (HHMMSS.s): ");
    Serial.println(data.utc_time);
    Serial.print("Altitud (m): ");
    Serial.println(data.altitude, 2);
    Serial.print("Velocidad (nudos): ");
    Serial.println(data.speed, 2);
    Serial.print("Curso (°): ");
    Serial.println(data.course, 2);
    Serial.print("PDOP: ");
    Serial.println(data.pdop, 2);
    Serial.print("HDOP: ");
    Serial.println(data.hdop, 2);
    Serial.print("VDOP: ");
    Serial.println(data.vdop, 2);
    Serial.println("===============================");
}

int Connection::getFix() { return fix; }
/*float Connection::getLat() { return vlat; }
float Connection::getLon() { return vlon; }
float Connection::getSpeed() { return vspeed; }
float Connection::getCourse() { return vcourse; }
int Connection::getSatt() { return num_satt; }*/
Connection::GPSData Connection::getLastGPSData() {return lastGPSData;}


