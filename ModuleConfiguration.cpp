#include <ModuleConfiguration.h>

ModuleConfiguration::ModuleConfiguration(unsigned char *configuration, unsigned int size, unsigned int eepromAddress, bool (*validator)(unsigned int, unsigned char)) {
  this->configuration = configuration;
  this->size = size;
  this->eepromAddress = eepromAddress;
  this->validator = validator;

  if (EEPROM.read(this->eepromAddress) == 0xff) {
    EEPROM.put(this->eepromAddress, this->configuration);
  } else {
    EEPROM.get(this->eepromAddress, this->configuration);
  }
}

bool ModuleConfiguration::setByte(unsigned int index, unsigned char value) {
  if ((index < this->size) && ((!this->validator) || (this->validator(index, value)))) {
    this->configuration[index] = value;
    this->saveByte(index);
    return(true);
  }
  return(false);
}

unsigned char ModuleConfiguration::getByte(unsigned int index) {
  return((index < this->size)?this->configuration[index]:0xff);
}


bool ModuleConfiguration::validateAddress(unsigned int index) {
  return(index < this->size);
}

bool ModuleConfiguration::processValue(unsigned int index, unsigned char value) {
  return(this->setByte(index, value));
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

void ModuleConfiguration::erase() {
  for (unsigned int i = 0; i < this->size; i++) {
    EEPROM.update((this->eepromAddress + i), 0xff);
  }
}
