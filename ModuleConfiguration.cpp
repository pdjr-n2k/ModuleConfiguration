#include "ModuleConfiguration.h"

ModuleConfiguration::ModuleConfiguration(unsigned char* (*initialiser)(int&, unsigned int), bool (*changeHandler)(unsigned int, unsigned char), unsigned int eepromAddress) {
  unsigned int size;

  this->eepromAddress = eepromAddress;
  this->changeHandler = changeHandler;
  this->interactionTimeout = 30000UL;

  this->configuration = initialise(size, eepromAddress);
  this->size = size;
}

void ModuleConfiguration::setByte(unsigned int index, unsigned char value) {
  if ((index < this->size) && (this->changeHandler(index, value))) {
    this->configuration[index] = value;
    this->saveByte(index);
  }
}

unsigned char ModuleConfiguration::getByte(unsigned int index) {
  if (index < this->size) {
    return(this->configuration[index]);
  }
  return(0xff);
}

/**********************************************************************
 * 1   a long press interaction specified a valid address and interact
 *     is waiting for short press value to be supplied
 * 2   a short press interaction specified a value for a previously
 *     supplied address and the value has been saved to the
 *     configuration.
 * 0   Interaction not dealt with. A short press was presented without
 *     a previous address. No action taken
 * -1  a long press interaction specified an out-of-range configuration
 *     address.
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
        } else {
          retval = -1;
        }
        break;
      case false:
        if (address != -1) {
          retval = (this->setByte(address, (unsigned char) value))?2:-2;
          address = -1;
        }
        break;
    }
  }
  return(retval);
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
