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
    if (this->changeHandler) this->changeHandler(0, index, value);
  }
}

unsigned char ModuleConfiguration::getByte(unsigned int index) {
  if (index < this->size) {
    return(this->configuration[index]);
  }
  return(0xff);
}

/**********************************************************************
 * 0  a long press interaction specified an invalid address and has
 *     been ignored.
 * 1   a long press interaction specified a valid address and interact
 *     is waiting for short press value to be supplied
 * 2   a short press interaction specified a value for a previously
 *     supplied address
 * 3   a short press interaction occured without the presence of a
 *     (previously specified) valid address.
 */

int ModuleConfiguration::interact(int value, bool longPress) {
  static long resetDeadline = 0UL;
  static int address = -1;
  long now = millis();
  int retval = 0;
   
  if (value == 0xffff) { // Perhaps cancel a timed-out protocol.
    if ((resetDeadline != 0UL) && (now > resetDeadline)) {
      address = -1;
      resetDeadline = 0UL;
    }
  } else {
    switch (longPress) {
      case true:
        if ((value >= 0) && (value < EEPROM.length())) {
          address = value;
          resetDeadline = (now + this->interactionTimeout);
          retval = 1;
        }
        break;
      case false:
        if (address != -1) {
          this->setByte(address, (unsigned char) value);
          address = -1;
          retval = 2;
        } else {
          retval = 3;
        }
        break;
    }
  }
  return(retval);
}

void ModuleConfiguration::initialise(unsigned char* (*initialiser)(int& size)) {
  int size;
  this->configuration = initialiser(this->size, this->eepromAddress);
  this->save();
}

void ModuleConfiguration::saveByte(unsigned int index) {
  EEPROM.update(this->eepromAddress + index, this->configuration[index]);
}

void ModuleConfiguration::save() {
  EEPROM.put(this->eepromAddress, this->configuration);
}

void ModuleConfiguration::load() {
  EEPROM.get(this->eepromAddress, this->configuration);
}
