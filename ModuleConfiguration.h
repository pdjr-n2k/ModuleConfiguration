#ifndef MODULE_CONFIGURATION_H
#define MODULE_CONFIGURATION_H

#include <Arduino.h>
#include <EEPROM.h>

/**
 * @brief Representation of a byte array that can be used as a
 *        basis for firmware configuration.
 * 
 * A module configuration is modelled as a byte array indexed from
 * zero.
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
     * This rather specialised method is designed to support a style of
     * user interaction predicated on the underlying presence of a
     * register device (like a DIL-switch) that can be used to specify
     * a value and a push-button that can be used to submit the value
     * of the register by either a short-press or a long-press.
     * 
     * The interaction protocal supported by this method is one in which
     * a long-press is used to specify an index in the configuration and
     * an immediately subsequent short-press is used to specify a value
     * which should be saved at the previously entered index.
     * 
     * @param value     - the register value being offered for
     *                    processing. 
     * @param longPress - whether value resulted from a long button
     *                    press (in which case it is an candidate index)
     *                    or from a short button press (in which case it
     *                    is a putative value).
     * @return int      - On success:
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
     * 
     *                    On error:
     *                    -1 a long button press supplied an invalid
     *                       index.
     *                    -2 a short button press supplied a value which
     *                       failed validation by the validator callback. 
     */
    int interact(int value = 0xffff, bool longPress = false);

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

  private:
    unsigned char* (*initialiser)(int&, unsigned int);
    bool (*validator)(unsigned int, unsigned char);
    unsigned int eepromAddress;

    unsigned long interactionTimeout = 30000UL;
    unsigned char *configuration = 0;
    unsigned int size = 0;
    
};

#endif