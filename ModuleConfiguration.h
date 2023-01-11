#ifndef MODULE_CONFIGURATION_H
#define MODULE_CONFIGURATION_H

#include <Arduino.h>
#include <EEPROM.h>

class ModuleConfiguration {

  public:
    ModuleConfiguration(unsigned char* (*initialiser)(int&, unsigned int), bool (*changeHandler)(unsigned int, unsigned char), unsigned int eepromAddress = 0);

    void setup();

    bool setByte(unsigned int index, unsigned char value);
    unsigned char getByte(unsigned int index);

    int interact(int value = 0xffff, bool longPress = false);

    void saveByte(unsigned int index);
    void save();
    void load();

  private:
    unsigned char* (*initialiser)(int&, unsigned int);
    bool (*validator)(unsigned int, unsigned char);
    unsigned int eepromAddress;

    unsigned long interactionTimeout = 30000UL;
    unsigned char *configuration = 0;
    unsigned int size = 0;
    
};

#endif