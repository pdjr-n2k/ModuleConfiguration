#include "ModuleConfiguration.h"

ModuleConfiguration::ModuleConfiguration(unsigned int eepromAddress, void (*changeHandler)(unsigned int, unsigned char)) {
  this->size = 0;
  this->eepromAddress = eepromAddress;
  this->changeHandler = changeHandler;
  this->interactionTimeout = 30000UL;

  this->configuration = new unsigned char [this->size];
  for (unsigned int i = 0; i < this->size; i++) this->configuration[i] = 0xff;
}

void ModuleConfiguration::setByte(unsigned int index, unsigned char value) {
  if (index < this->size) {
    EEPROM.put(index, this->configuration[index] = value);
    if (this->changeHandler) this->changeHandler(index, value);
  }
}

unsigned char ModuleConfiguration::getByte(unsigned int index) {
  if (index < this->size) {
    return(this->configuration[index]);
  }
  return(0xff);
}

bool ModuleConfiguration::interact(int value, bool longPress) {
  static long resetDeadline = 0UL;
  static int address = -1;
  long now = millis();
  bool retval = false;
   
  if (value == 0xffff) { // Perhaps cancel a timed-out protocol.
    if ((resetDeadline != 0UL) && (now > resetDeadline)) {
      address = -1;
      resetDeadline = 0UL;
      retval = false;
    }
  } else {
    switch (longPress) {
      case true:
        if ((value >= 0) && (value < EEPROM.length())) {
          address = value;
          retval = true;
          resetDeadline = (now + this->interactionTimeout);
        }
        break;
      case false:
        if (address != -1) {
          this->setByte(address, (unsigned char) value);
          address = -1;
          retval = false;
        }
        break;
    }
  }
  return(retval);
}

void ModuleConfiguration::initialise(unsigned char* (*initialiser)(int& size)) {
  int size;
  this->configuration = initialiser(size);
  this->size = size;
  this->save();
}

void ModuleConfiguration::save() {
  EEPROM.put(this->eepromAddress, this->configuration);
}

void ModuleConfiguration::load() {
  EEPROM.get(this->eepromAddress, this->configuration);
}
