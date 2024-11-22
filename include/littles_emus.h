#ifndef LITTLES_ENUMS_H
#define LITTLES_ENUMS_H

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

#endif


