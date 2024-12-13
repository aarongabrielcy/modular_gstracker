#include "PowerOut.h"

void PowerOut::powerModule(){
  pinMode(POWER_SIM_PIN, OUTPUT);
  digitalWrite(POWER_SIM_PIN, HIGH);  
}

void PowerOut::powerKey(){
  pinMode(POWER_KEY_PIN, OUTPUT);
  digitalWrite(POWER_KEY_PIN, LOW); 
  delay(1000);
  digitalWrite(POWER_KEY_PIN, HIGH);
  delay(3000);
}

void PowerOut::engineCutOn() {
    pinMode(GNSS_LED_PIN, OUTPUT);
    digitalWrite(GNSS_LED_PIN, HIGH);
    delay(3000);
}

void PowerOut::engineCutOff() {
    pinMode(GNSS_LED_PIN, OUTPUT);
    digitalWrite(GNSS_LED_PIN, LOW); 
    delay(3000);
}