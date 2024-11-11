#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <Arduino.h>
void configureAPN(const String& apn);
bool validConfigAPN();
void configurePDP();
bool validatePDP();
#endif
