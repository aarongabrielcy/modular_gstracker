//CONFIGURACIÓN INICIAL 
/*
1. FECHA DEL MODULO
2. AUTO CONEXION DE RED CELULAR

*/

#ifndef CONFIG_H
#define CONFIG_H

// Pines para el módulo SIM7600
#define SIM7600_RX_PIN 18      // Pin RX del módulo SIM7600 (conectar al TX del ESP32)
#define SIM7600_TX_PIN 17      // Pin TX del módulo SIM7600 (conectar al RX del ESP32)

// Tiempo de espera predeterminado para comandos AT (en milisegundos)
#define AT_COMMAND_TIMEOUT 5000
#define TEST_SIM_TIMEOUT 20000
#define UPDATE_DATA_TIMEOUT 3000
#define SEND_DATA_TIMEOUT 8000
// Velocidad de transmisión predeterminada (baud rate) para la comunicación serial con el módulo
#define SIM7600_BAUD_RATE 115200

// Configuración de serial del monitor para depuración
#define SERIAL_BAUD_RATE 115200

// Pines de encendido y reseteo del módulo SIM7600 (si aplica)
#define SIM7600_PWRKEY_PIN 4   // Pin de encendido (PWRKEY) del SIM7600
#define SIM7600_RST_PIN 5      // Pin de reinicio (RESET) del SIM7600

#define INPUTS "{\"ign\": 10, \"in1\": 11, \"in2\": 12, \"in3\": 16, \"in4\": 0, \"in5\": 0, \"in6\": 0, \"in7\": 0}"

#define PIN_GPIO_BKB      (8)
#define PIN_GPIO_BAT      (9)

#define OUTPUTS "{\"out1\": 13, \"out2\": 14, \"out3\": 0, \"out4\": 0, \"out5\": 0, \"rsv1\": 0, \"rsv2\": 0, \"rsv3\": 0}"

//REPORT FORMAR MARKS
#define COMMA ","
#define SMCLN ";" //semicolon
// Opcional: Define el APN y otros parámetros de red si aplica
#define PDPTYPE "IP"
#define DEFAULT_APN_TELCEL "internet.itelcel.com"
#define DEFAULT_APN_ATT "ott.iot.attmex.mx"
#define DEFAULT_SVR "34.196.135.179"
#define DEFAULT_SRV_DEV "201.99.112.78"
#define DEFAULT_PORT_DEV 7004
#define DEFAULT_PORT 5200

//Configuración ESP WIFI AP
#define DEFAULT_IP "192.168.101.1"
#define DEFAULT_PORT_ES 8080
#define DEFAULT_SSID "GST AP V1"
#define DEFAULT_PASS "gstrackerv1."
#define DEFAULT_WEB_SVR 80

//Static values
#define HEADER "STT"
#define REPORT_MAP "3FFFFF"
#define MODEL_DEVICE 32
#define SW_VER "1.0.0"
#define MSG_TYPE 1

// Configuraciones de depuración
#define DEBUG true
#define GPS_DEBUG "2056.004981,N,08942.826095,W,141124,040824.0,-14.7,0.0,0"
#define GNSS_DEBUG "2,06,02,10,2056.008970,N,08942.816465,W,141124,040641.0,12.7,70.3,255.0,2.0,1.7,1.0"
#define INPUTS_ACTIVE "00000111"
#define OUTPUTS_ACTIVE "00000011"
//#define GNSS_DEBUG "2,09,05,00,3113.330650,N,12121.262554,E,131124,091918.0,32.9,0.0,255.0,1.1,0.8,0.7"
#endif

