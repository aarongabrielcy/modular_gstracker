#ifndef LITTLES_ENUMS_H
#define LITTLES_ENUMS_H

#include "config.h"

enum dateTimeType {
    DATE,
    TIME 
};
enum CellularCompany {
    ATT = 1,
    TELCEL
};
enum TypeofReport {
    ResCMDreq, //manda una cadena al servidor como respuesta de un comando mandado
    Time, // manda una cadena al servidor cuando vence el tiempo programdo por ejemplo cada 10s
    Distance, // manda una cadena al servidor cuando avanza una distancia configurada en el dispositivo ej. cada 10 km
    AngleHeading // manda una cadena al servidor cuando cambia de un angulo especifico el dispositivo por ej. cuando gira 15 grados.
};

enum ModeDevice {
    Parking,
    Driving,
    Idle,
    Speeding,
    Towing,
    DeactivateZone,
    Emergency
};

enum InState {
    ign,
    in1,
    in2,
    in3,
    in4,
    in5,
    in6,
    in7
};
enum OutActive {
    out1,
    out2,
    out3, 
    out4,
    out5,
    rsv6,
    rsv7,
    rsv8
};
#endif


