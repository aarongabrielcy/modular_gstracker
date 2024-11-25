#ifndef CONNECTION_H
#define CONNECTION_H

#include <Arduino.h>
#include "SimModule/SIM7600.h"

class Connection {
    public:
        Connection(SIM7600& sim7600);
        bool activeModuleSat(int state);
        bool ReadDataGNSS();
        int getFix();
        float getLat();
        float getLon();
        float getSpeed();
        float getCourse();
        int getSatt();

        // Estructura para almacenar datos parseados
        struct GPSData {
            int mode;
            int gps_svs;
            int glonass_svs;
            int beidou_svs;
            String latitude;
            String longitude;
            char ns_indicator;
            char ew_indicator;
            String date;
            String utc_time;
            float altitude;
            float speed;
            float course;
            float pdop;
            float hdop;
            float vdop;
        };
        GPSData ParseData(const String &data);
        void printGPSData(const GPSData &data);
        GPSData getLastGPSData(); // Método para obtener el último GPSData
    private:
        SIM7600& simModule;
        String formatCoordinates(const String &coord, char direction);
        String formatDate(const String &date);    // Nueva función para formatear fecha
        String formatTime(const String &utcTime); // Nueva función para formatear hora
        GPSData lastGPSData; // Almacena el último GPSData procesado

        int fix, num_satt;
        float vlat, vlon, vspeed, vcourse;
};
#endif
