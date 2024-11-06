#ifndef CONFIG_H
#define CONFIG_H

// Pines para el módulo SIM7600
#define SIM7600_RX_PIN 18      // Pin RX del módulo SIM7600 (conectar al TX del ESP32)
#define SIM7600_TX_PIN 17      // Pin TX del módulo SIM7600 (conectar al RX del ESP32)

// Tiempo de espera predeterminado para comandos AT (en milisegundos)
#define AT_COMMAND_TIMEOUT 5000

// Velocidad de transmisión predeterminada (baud rate) para la comunicación serial con el módulo
#define SIM7600_BAUD_RATE 115200

// Configuración de serial del monitor para depuración
#define SERIAL_BAUD_RATE 115200

// Pines de encendido y reseteo del módulo SIM7600 (si aplica)
#define SIM7600_PWRKEY_PIN 4   // Pin de encendido (PWRKEY) del SIM7600
#define SIM7600_RST_PIN 5      // Pin de reinicio (RESET) del SIM7600

// Opcional: Define el APN y otros parámetros de red si aplica
#define DEFAULT_APN "internet.itelcel.com"

// Configuraciones de depuración
#define DEBUG true

#endif
