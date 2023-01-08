# ModuleConfiguration

## CONSTRUCTORS

### ModuleConfiguration(*size*)

Create a new ModuleConfiguration instance with a limit of *size* bytes.

### ModuleConfiguration(*size*, *eepromAddress*)

Create a new ModuleConfiguration instance with a limit of *size* bytes and an EEPROM start address of *eepromAddress*.

### ModuleConfiguration(*size*, *eepromAddress*, *changeHandler*)

Create a new ModuleConfiguration instance with a limit of *size* bytes and an EEPROM start address of *eepromAddress*. *changeHandler* is a callback function which will be invoked each time a configuration interaction results in a module configuration change

## METHODS

### setInitialiser(*initialiser*)

Set the callback which will be invoked by the initialise() method to the function *initialiser*.

### setByte(*address*, *value*)

Set the configuration byte at *address* to *value*.

### getByte(*address*)

Return the configuration byte at *address*.

### interact(*value*, *longPress*)

Process an interaction event that has generated *value* as the result of a long or short button press as indicated by the boolean value *longPress*. 

### initialise()

### save()

Save the configuration to EEPROM.

### load()

Load the configuration from EEPROM.
