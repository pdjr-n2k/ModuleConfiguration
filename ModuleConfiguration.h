#ifndef MODULE_CONFIGURATION_H
#define MODULE_CONFIGURATION_H

#include <Arduino.h>
#include <EEPROM.h>

class ModuleConfiguration {

  public:
    ModuleConfiguration(unsigned int eepromAddress = 0, void (*changeHandler)(unsigned int, unsigned char) = 0);

    void setByte(unsigned int index, unsigned char value);
    unsigned char getByte(unsigned int index);

    bool interact(int value = 0xffff, bool longPress = false);
    void initialise(unsigned char* (*initialiser)(int& size, unsigned int eepromAddress));

    void save();
    void load();

  private:
    unsigned int eepromAddress;
    unsigned long interactionTimeout;
    void (*changeHandler)(unsigned int address, unsigned char value);
  
    unsigned char *configuration;
    unsigned char size = 0;
    
};

#endif