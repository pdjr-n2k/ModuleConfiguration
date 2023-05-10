#ifndef MODULE_CONFIGURATION_H
#define MODULE_CONFIGURATION_H

#include <Arduino.h>
#include <EEPROM.h>
#include <ModuleOperatorInterfaceClient.h>

/**
 * @brief ADT modelling an array of bytes intended for use as as a
 * persistent repository of firmware configuration data.
 * 
 * A ModuleConfiguration instance must be initialised before use by a
 * call to the initialise() method.
 */
class ModuleConfiguration: public ModuleOperatorInterfaceClient {

  public:

    /**
     * @brief Construct a new ModuleConfiguration object.
     *
     * Create a new ModuleConfiguration instance from either an
     * existing, previously saved, configuration or, if none exists,
     * then from a supplied fallback.
     * 
     * @param configuration - byte array containing a fallback configuration.
     * @param size          - size of configuration array in bytes.
     * @param eepromAddress - address in EEPROM where the configuration has been or will be persisted (optional: defaults to 0).
     * @param validator     - callback function that can be used to validate bytes written to the module configuration (optional: defaults to none).
     */    
    ModuleConfiguration(unsigned char *configuration, unsigned int size, unsigned int eepromAddress = 0, bool (*validator)(unsigned int, unsigned char) = 0);

    /**
     * @brief Set a byte in the configuration.
     * 
     * If a /a validator function was specified when this instance was
     * created then supplied bytes must be successfully validated
     * before being saved.
     * 
     * @param index  - the index within the configuration where value should be saved.
     * @param value  - the value to be saved.
     * @return true  - value was saved successfully.
     * @return false - value was not saved because \a index was out of range or \a value failed validation.
     */
    bool setByte(unsigned int index, unsigned char value);

    /**
     * @brief Get a byte from the configuration.
     * 
     * @param index - the index within the configuration from where the requested byte should be retrieved. 
     * @return unsigned char - the retrieved byte or 0xff if \a index was out of range.
     */
    unsigned char getByte(unsigned int index);

    /**
     * @brief Check that an index value is a valid configuration address.
     * 
     * @param index - the value to be validated.
     * @return true - index is valid (in range).
     * @return false - index is invalid (out of range).
     */
    bool validateAddress(unsigned int index);

    /**
     * @brief Alias for setByte().
     */
    bool processValue(unsigned int index, unsigned char value);

    /******************************************************************
     * @brief Save a configuration byte to persistent storage.
     * 
     * @param index - the index of the configuration byte to be saved. 
     */
    void saveByte(unsigned int index);

    /******************************************************************
     * @brief Save the entire configuration to persistent storage.
     */
    void save();

    /******************************************************************
     * @brief Load the entire configuration from persistent storage.
     */
    void load();

    /******************************************************************
     * @brief Erase the configuration from persistent storage.
     * 
     * The value 0xff will be writtem to every configuration byte.
     */
    void erase();

  private:
    unsigned char *configuration = 0;
    unsigned int size = 0;
    unsigned int eepromAddress;
    bool (*validator)(unsigned int, unsigned char);
    
};

#endif