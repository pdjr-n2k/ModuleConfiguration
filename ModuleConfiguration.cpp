#include <ModuleConfiguration.h>

tModuleConfiguration::tModuleConfiguration(unsigned char *configuration, unsigned int size, unsigned int eepromAddress, bool (*validator)(unsigned int, unsigned char)) {
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

bool tModuleConfiguration::setByte(unsigned int index, unsigned char value) {
  if ((index < this->size) && ((!this->validator) || (this->validator(index, value)))) {
    this->configuration[index] = value;
    this->saveByte(index);
    return(true);
  }
  return(false);
}

unsigned char tModuleConfiguration::getByte(unsigned int index) {
  if (index < this->size) {
    return(this->configuration[index]);
  }
  return(0xff);
}

bool tModuleConfiguration::validateAddress(unsigned char index) {
  return(index < this->size);
}

bool tModuleConfiguration::processValue(unsigned char index, unsigned char value) {
  return(this->setByte(index, value));
}

void tModuleConfiguration::saveByte(unsigned int index) {
  EEPROM.update(this->eepromAddress + index, this->configuration[index]);
}

void tModuleConfiguration::save() {
  EEPROM.put(this->eepromAddress, this->configuration);
}

void tModuleConfiguration::load() {
  EEPROM.get(this->eepromAddress, this->configuration);
}

void tModuleConfiguration::erase() {
  for (unsigned int i = 0; i < this->size; i++) {
    EEPROM.update((this->eepromAddress + i), 0xff);
  }
}
