# ModuleConfiguration

Class implementing an array of bytes intended for use as a repository
for firmware configuration data.
Methods allow initialisation, interactive updates and use of persistent
storage.

A ModuleConfiguration instance must be initialised before use by a call
to the initialise() method. 

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

*changeHandler* is an optional callback function which will be invoked
each time the value of a configuration byte is about to be updated with
the index of the changed byte and its new value as arguments.
If *changeHandler* is specified, then it should return true if the
update should proceed, or false if the update should be discarded. 
In the former case, *changeHandler* might also take the opportunity to
update any host application users of the changed configuration value.
```
#define CONFIGURATION_EEPROM_ADDRESS 0

bool myChangeHandler(unsigned int index, unsigned char value) {
  return(value < 128);
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

Invokes the *initialiser* callback function which should create,
initialise and return a pointer to a static memory buffer holding
the module configuration byte array.
*initialiser* is called with two arguments: the first is a
reference to a variable that should be used to pass back the size
of the created array and the second is the EEPROM address that was
passed to the instance constructor.

*initialiser* must create a dynamically allocated buffer of a size
sufficient to hold the module configuration.
Typically it will attempt to load any saved configuration into the
buffer.
If, for whatever reason, there is no saved configuration then the
function should prepare buffer for use in whatever way is
appropriate.

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

### saveByte(*index*)

Save the configuration byte at *index* to EEPROM using the EEPROM
library's update() method.

### save()

Save the configuration array to EEPROM using the EEPROM library's
put() method.

### load()

Load the configuration array from EEPROM using the EEPROM library's
get() method.
