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
    digitalWrite(pinOut, LOW);
}

void Generated::turnOn() {
  digitalWrite(pinOut, HIGH);
}

void Generated::turnOff() {
  digitalWrite(pinOut, LOW);
}

Generated::IOGen Generated::stateInputs(const String &inputs){
  
}

void Generated::initIO(char* activeIn, char* activeOut){
  /*InState inState;
  OutState outState;

  int inActive = pinsActive(activeIn); 
  int outActive = pinsActive(activeOut);*/


 }



 int pinsActive(const char* pins){
  uint8_t byteValue = strtol(pins, NULL, 2); // Convierte el string binario a un entero
  int count = 0;

  while (byteValue) {
    count += (byteValue & 1); // Suma 1 si el bit menos significativo está activo
    byteValue >>= 1; // Desplaza un bit a la derecha
  }

  return count;
 }

 void Generated::initializePinsFromJson(const char* json, const char* bitmask){

  DeserializationError error = deserializeJson(doc, json);

  // Manejo de errores de JSON
  if (error) {
    Serial.print("Error al procesar JSON: ");
    Serial.println(error.f_str());
    return;
  }

  int bitIndex = 0; // Para iterar sobre la máscara de bits
    for (JsonPair keyValue : doc.as<JsonObject>()) {
      int pin = keyValue.value().as<int>(); // Extrae el valor del pin
      if (bitmask[bitIndex] == '1' && pin > 0) {
        pinMode(pin, INPUT);
        Serial.print("Pin inicializado desde JSON: ");
        Serial.print(keyValue.key().c_str());
        Serial.print(" -> ");
        Serial.println(pin);
      }
      bitIndex++;
    }
 }

 void Generated::readPinsFromJson(const char* bitmask) {
    IOGen ioGen; //corregir la funcion!
  int bitIndex = 0; // Para iterar sobre la máscara de bits
  for (JsonPair keyValue : doc.as<JsonObject>()) {
    int pin = keyValue.value().as<int>();
    if (bitmask[bitIndex] == '1' && pin > 0) {
      int value = digitalRead(pin);
      Serial.print("Lectura del pin ");
      Serial.print(keyValue.key().c_str());
      Serial.print(" (");
      Serial.print(pin);
      Serial.print("): ");
      Serial.println(value);
      if(keyValue.key().c_str() == "ign"){
        ioGen.ign = value;
      }else if(keyValue.key().c_str() == "in1") {
        ioGen.in1 = value;
      }else if(keyValue.key().c_str() == "in2"){
        ioGen.in2 = value;
      }
    }
    bitIndex++;
  }
}