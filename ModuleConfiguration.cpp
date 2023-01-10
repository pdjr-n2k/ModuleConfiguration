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

/**********************************************************************
 * 1 - the interaction event specified a valid parameter address and is
 *     being handled.
 * 2 - the interaction event specified a valid parameter value and has
 *     been handled.
 * 3 - a long press specified an address that was out of range.
 * 4 - a short press specified a value but no previous address has
 *     been specified.
 * 5 - a short press specified a value for a previously out of range
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
          retval = 1;
          resetDeadline = (now + this->interactionTimeout);
        } else {
          retval = 3;
        }
        break;
      case false:
        if (address != -1) {
          if ((address >= 0) && (address < EEPROM.length())) {
            this->setByte(address, (unsigned char) value);
            address = -1;
            retval = 2;
          } else {
            retval = 5;
          }
        } else {
          retval = 4;
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
