#ifndef MODULE_CONFIGURATION_H
#define MODULE_CONFIGURATION_H

#include <Arduino.h>
#include <EEPROM.h>

class ModuleConfiguration {

  public:
    ModuleConfiguration(unsigned char* (*initialiser)(int&, unsigned int), bool (*changeHandler)(unsigned int, unsigned char), unsigned int eepromAddress = 0);

    void setByte(unsigned int index, unsigned char value);
    unsigned char getByte(unsigned int index);

    int interact(int value = 0xffff, bool longPress = false);

    void saveByte(unsigned int index);
    void save();
    void load();

  private:
    unsigned int eepromAddress;
    unsigned long interactionTimeout;
    bool (*changeHandler)(unsigned int address, unsigned char value);
  
    unsigned char *configuration;
    unsigned char size = 0;
    
};

#endif