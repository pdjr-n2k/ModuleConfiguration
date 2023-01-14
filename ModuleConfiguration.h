#ifndef MODULE_CONFIGURATION_H
#define MODULE_CONFIGURATION_H

#include <Arduino.h>
#include <EEPROM.h>

/**
 * @brief Representation of a byte array that can be used as a
 *        basis for firmware configuration.
 */
class ModuleConfiguration {

  public:
    /******************************************************************
     * @brief Construct a new Module Configuration object.
     * 
     * @param initialiser   - callback function that will be used to
     *                        load / default the module configuration.
     * @param validator     - callback function that will be used to
     *                        validate and approve every byte written
     *                        to the module configuration.
     * @param eepromAddress - address in EEPROM where the configuration
     *                        will be persisted.
     */    
    ModuleConfiguration(unsigned char* (*initialiser)(int&, unsigned int), bool (*validator)(unsigned int, unsigned char), unsigned int eepromAddress = 0);

    /******************************************************************
     * @brief Initialise the configuration by executing the initialiser
     *        callback.
     */
    void setup();

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

    /******************************************************************
     * @brief Process an interaction protocol event.
     * 
     * @param value     - the registered value being offered for
     *                    processing. 
     * @param longPress - whether value resulted from a long button
     *                    press or not.
     * @return int      - Result code:
     *                    1  a long button press supplied an index value
     *                       which was valid. The next invocation with
     *                       longPress equal to false should specify a
     *                       value to be stored at the supplied index in
     *                       configuration.
     *                    2  a short button press supplied a value which
     *                       has been saved at the previously specified
     *                       index.
     *                    0  a short button press supplied a value but no
     *                       index had been previously specified.
     *                    -1   
     */
    int interact(int value = 0xffff, bool longPress = false);

    /**
     * @brief Save a configuration byte to EEPROM using EEPROM.update().
     * 
     * @param index - the index of the configuration byte to be saved. 
     */
    void saveByte(unsigned int index);

    /**
     * @brief Save the entire configuration array to EEPROM.
     */
    void save();

    /**
     * @brief Load the entire configuration array from EEPROM.
     */
    void load();

  private:
    unsigned char* (*initialiser)(int&, unsigned int);
    bool (*validator)(unsigned int, unsigned char);
    unsigned int eepromAddress;

    unsigned long interactionTimeout = 30000UL;
    unsigned char *configuration = 0;
    unsigned int size = 0;
    
};

#endif