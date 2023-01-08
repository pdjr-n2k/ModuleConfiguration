#include <cstdlib>
#include <Arduino.h>
#include <EEPROM.h>
#include "ModuleConfiguration.h"

ModuleConfiguration::ModuleConfiguration(unsigned int size, unsigned int eepromAddress, void (*changeHandler)(unsigned int, unsigned char)) {
  this->size = size;
  this->eepromAddress = eepromAddress;
  this->changeHandler = changeHandler;
  this->interactionTimeout = 30000UL;

  this->configuration = new unsigned char [this->size];
  for (unsigned int i = 0; i < this->size; i++) this->configuration[i] = 0xff;
}

void ModuleConfiguration::setInitialiser(bool (*initialiser)()) {
  this->initialiser = initialiser;
}

void ModuleConfiguration::setByte(unsigned int index, unsigned char value) {
  if (index < this->size) {
    this->configuration[index] = value;
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
          if (this->changeHandler) this->changeHandler(address, (unsigned char) value);
          address = -1;
          retval = false;
        }
        break;
    }
  }
  return(retval);
}

void ModuleConfiguration::initialise() {
  if (this->initialiser) this->initialiser();
}

void ModuleConfiguration::save() {
  EEPROM.put(this->eepromAddress, this->configuration);
}

void ModuleConfiguration::load() {
  EEPROM.get(this->eepromAddress, this->configuration);
}
