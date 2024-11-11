#include "NetworkManager.h"

void configureAPN(const String& apn) {
  Serial.print("Configurando APN: ");
  Serial.println(apn);
}

bool validConfigAPN(){
  return false;
}

void configurePDP(){

}

bool validatePDP(){
  return false;
}

void configureTCP(){

}

bool validTCP(){
  return false;
}