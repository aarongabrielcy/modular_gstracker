#include "Generated.h"
// Espacio para el JSON deserializado
StaticJsonDocument<256> doc;

Generated::Generated() {}

void Generated::initInput(int pin) {
    this->pin = pin;
    pinMode(pin, INPUT);
}
//leer todos los inputs y devolverlos al mein en un objeto
bool Generated::readInput() {
    return digitalRead(pin);
    
}

void Generated::initOutput(int pinOut) {
    this->pinOut = pinOut;
    pinMode(pinOut, OUTPUT);
    pinMode(13, OUTPUT);
}
void Generated::GnssLED(int pin, bool state){
  digitalWrite(pin, !digitalRead(pin));
}
void Generated::turnOn() {
  digitalWrite(pinOut, HIGH);
  digitalWrite(13, HIGH);
  Serial.println("Salida Activada!!");
}

void Generated::turnOff() {
  digitalWrite(pinOut, LOW);
  digitalWrite(13, LOW);
  Serial.println("Salida Apagada!!");
}

/*Generated::InOutState Generated::stateInputs(const String &inputs){
  
}*/
void Generated::initializePinsFromJson(const char* json, const char* bitmask) {
  /*Serial.print("Máscara de bits recibida: ");
  Serial.println(bitmask);
  Serial.print("JSON recibido: ");
  Serial.println(json);*/

  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print("Error al procesar JSON: ");
    Serial.println(error.f_str());
    return;
  }
  /*Serial.println("Contenido del JSON deserializado:");
  serializeJsonPretty(doc, Serial);
  Serial.println();*/
  int bitIndex = strlen(bitmask) - 1; // Empieza desde el último bit
  for (JsonPair keyValue : doc.as<JsonObject>()) {
    int pin = keyValue.value().as<int>(); // Extrae el valor del pin
    /*Serial.print("Iterando: ");
    Serial.print(keyValue.key().c_str());
    Serial.print(" con pin ");
    Serial.print(pin);
    Serial.print(" y bitmask[");
    Serial.print(bitIndex);
    Serial.print("] = ");
    Serial.println(bitmask[bitIndex]);*/

    // Comprueba si el bit está activo y si el pin es válido
    if (bitmask[bitIndex] == '1' && pin > 0) {
      /*Serial.print("Pin para activar: ");
      Serial.println(pin);*/
      pinMode(pin, INPUT);
      /*Serial.print("Pin inicializado desde JSON: ");
      Serial.print(keyValue.key().c_str());
      Serial.print(" -> ");
      Serial.println(pin);*/
    } else {
      Serial.println("Condición no cumplida.");
    }
    bitIndex--; // Decrementa para recorrer el bitmask en orden inverso
  }
}
 void Generated::readPinsFromJson(const char* bitmask) {
  
  int bitIndex = strlen(bitmask) - 1; // Para iterar sobre la máscara de bits
  /*Serial.println("Validando el contenido del JSON:");
  serializeJsonPretty(doc, Serial);
  Serial.println();*/
  for (JsonPair keyValue : doc.as<JsonObject>()) {
    int pin = keyValue.value().as<int>();
    if (bitmask[bitIndex] == '1' && pin > 0) {
      int value = digitalRead(pin);
      String key = keyValue.key().c_str();

      // Asigna valores al struct basado en la clave
      if (key == "ign") ioState.ign = value;
      else if (key == "in1") ioState.in1 = !value;
      else if (key == "in2") ioState.in2 = !value;
      else if (key == "in3") ioState.in3 = !value;
      /*Serial.print("Lectura del pin ");
      Serial.print(keyValue.key().c_str());
      Serial.print(" (");
      Serial.print(pin);
      Serial.print("): ");
      Serial.println(value);*/
    }
    bitIndex--;
  }
}
