#ifndef MODULE_CONFIGURATION_H
#define MODULE_CONFIGURATION_H

#include <Arduino.h>
#include <EEPROM.h>
#include <ModuleOperatorInterfaceClient.h>

/**
 * @brief Representation of a byte array that can be used as a
 * basis for firmware configuration.
 */
class ModuleConfiguration: public ModuleOperatorInterfaceClient {

  public:
    /******************************************************************
     * @brief Construct a new ModuleConfiguration object.
     *
     * Creates a new ModuleConfiguration object by reading size bytes
     * of data into configuration from EEPROM location eepromAddress.
     * 
     * If the read data appears to have been unconfigured, then the
     * value of configuration is used.
     * 
     * @param configuration - byte array containing default configuration.
     * @param size          - size of configuration in bytes.
     * @param eepromAddress - address in EEPROM where the configuration
     *                        will be persisted.
     * @param validator     - callback function that will be used to
     *                        validate and approve every byte written
     *                        to the module configuration.
     */    
    ModuleConfiguration(unsigned char *configuration, unsigned int size, unsigned int eepromAddress = 0, bool (*validator)(unsigned int, unsigned char) = 0);

    /******************************************************************
     * @brief Set a byte in the configuration from a value subject to
     *        approval by the validator callback.
     * 
     * @param index  - the index within the configuration where value
     *                 should be saved.
     * @param value  - the value to be saved - this will be passed to
     *                 the validator callback for validation before
     *                 being committed.
     * @return true  - value was saved successfully.
     * @return false - value was not saved because index was out of
     *                 range or validation checks failed.
     */
    bool setByte(unsigned int index, unsigned char value);

    /******************************************************************
     * @brief Get a byte from the configuration.
     * 
     * @param                - index within the configuration of the
     *                         required value. 
     * @return unsigned char - the retrieved byte or 0xff if index was
     *                         out of range.
     */
    unsigned char getByte(unsigned int index);

    bool validateAddress(unsigned char index);
    bool processValue(unsigned char address, unsigned char value);

    /******************************************************************
     * @brief Save a configuration byte to EEPROM using EEPROM.update().
     * 
     * @param index - the index of the configuration byte to be saved. 
     */
    void saveByte(unsigned int index);

    /******************************************************************
     * @brief Save the entire configuration array to EEPROM.
     */
    void save();

    /******************************************************************
     * @brief Load the entire configuration array from EEPROM.
     */
    void load();

    void erase();

  private:
    unsigned char* (*initialiser)(int&, unsigned int);
    bool (*validator)(unsigned int, unsigned char);
    unsigned int eepromAddress;

    unsigned long interactionTimeout = 30000UL;
    unsigned char *configuration = 0;
    unsigned int size = 0;
    
};

#endif