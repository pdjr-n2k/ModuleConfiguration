# ModuleConfiguration

Class implementing an array of bytes intended for use as a repository
for firmware configuration data.
Methods allow initialisation, interactive updates and use of persistent
storage.
Typically, a module configuration instance will be created and
initialised (see the initialise() method) before use. 

## CONSTRUCTORS

### ModuleConfiguration()

Create a new ModuleConfiguration instance with an EEPROM storage
address of 0.

### ModuleConfiguration(*eepromAddress*)

Create a new ModuleConfiguration instance with an EEPROM storage
address of *eepromAddress*.

### ModuleConfiguration(*eepromAddress*, *changeHandler*)

Create a new ModuleConfiguration instance with an EEPROM storage
address of *eepromAddress*.
*changeHandler* is a callback function which will be invoked each time
the value of a configuration byte changes with the index of the changed
byte and its new value as arguments.
This allows the host application to respond to a configuration change
as it sees fit.
```
#define CONFIGURATION_EEPROM_ADDRESS 0

void myChangeHandler(unsigned int index, unsigned char value) {
    EEPROM.put(CONFIGURATION_EEPROM_ADDRESS + index, value);
}

ModuleConfiguration myModuleConfiguration(CONFIGURATION_EEPROM_ADDRESS, myChangeHandler);
```

## METHODS

### setByte(*address*, *value*)

Set the configuration byte at *address* to *value*.

### getByte(*address*)

Return the configuration byte at *address*.

### interact(*value*, *longPress*)

Process an interaction event that has generated *value* as the result
of a long or short button press as indicated by the boolean value
*longPress*.

The method returns one of the following integer values:

0 says no action taken and none required.

1 says a long press has been processed and the supplied value stored
  as the configuration address to which a subsequent short press value
  will be saved.

2 says a short press has been processed and the supplied value stored
  at the previously supplied configuration address.

3 says a short press has been processed in the absence of a prior
  storage address.

This return value can be used by the host application to prompt or
otherwise signify to a user the state of an interaction protocol.

In the case that an interaction has completed and a change handler was
registered at instantiation then the change handler is invoked with the
affected address and its new value as arguments.

### initialise(*initialiser*)

Initialise the configuration with the value returned from the host
application function *initialiser* which must return an initialised
configuration array and its size.


The initialise() method must be called after a ModuleConfiguration
has been instantiated and before further use.

In an application which persists its configuration the initialiser
function will typically:

1. Create a static, dynamic, array big enough to hold the application's
   configuration data.
2. Load data into the array from the coniguration EEPROM address.
3. If the loaded data has not been set up correctly then initialise
   the configuration to some default state and save it.
4. Return the prepared array and its size.

```
#define CONFIG_SIZE 20

unsigned char *initialiser(int &size, unsigned int eepromAddress) {
    static unsigned char *buffer;
    buffer = new unsigned char[size = CONFIG_SIZE];
    EEPROM.get(eepromAddress, buffer);
    if (buffer[0] = 0xff) {
      for (unsigned int i = 0; i < CONFIG_SIZE; i++) buffer[i] = 0x00;
    }
    return(buffer);
}

myModuleConfig.initialise(initialiser);
```

### save()

Save the configuration to EEPROM, or, if no EEPROM address has been
configured, silently do nothing.

### load()

Load the configuration from EEPROM, or, if no EEPROM address has been
configured, silently do nothing.
