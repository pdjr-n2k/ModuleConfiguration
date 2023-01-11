# ModuleConfiguration

Class implementing an array of bytes intended for use as a repository
for firmware configuration data.
Methods allow initialisation, interactive updates and use of persistent
storage.

A ModuleConfiguration instance must be initialised before use by a call
to the initialise() method.

## Callback functions

### unsigned char *initialiser*(unsigned int *size*&, unsigned int *eepromAddress*)

The *initialiser* function is called from the class constructor must
both create and prepare an array of ```unsigned char`` which will be
used to hold module configuration data.
The function must return a pointer to the array and must also pass the
size of the array through the *size* reference.

Configuration data is often persisted to EEPROM and hence it will be
desiarable to restore persisted values to the array as part of the
initialisation process.

The following example function creates a configuration array, recovers
data from EEPROM and checks if the recovered data hs actually been saved
and, if not, proceeds to initialise all array items to 0x00.
```
#define CONFIGURATION_SIZE 20

unsigned char *initialiser(unsigned int size&, unsigned int eepromAddress) {
  static unsigned char buffer[] = new unsigned char(size = CONFIGURATION_SIZE);
  EEPROM.get(eepromAddress, buffer);
  if (buffer[0] == 0xff) {
    for (insigned int i = 0; i < CONFIGURATION_SIZE; i++) {
      buffer[i] = 0x00;
    }
  }
  return(buffer);
}

### bool *validator*(unsigned int *index*, unsigned char *value*)

The *validator* function is called from the setByte() method to
allow the host application to validate *value* before it is assigned
to configuration location *address*.

If *validator* returns true, *value* will be saved, otherwise *value*
will be discarded.

The following example function with accept all data values which are
even numbers.
```
bool validator(unsigned int index, unsigned char value) {
  return(!(value & 0x01));
}
```

## CONSTRUCTORS

### ModuleConfiguration(*initialiser*, *validator*)

Create a new ModuleConfiguration instance with an EEPROM storage
address of 0.
*initialiser* is a callback function which will be used to prepare
the configuration for use.
*validator* is a callback function which will be used to validate
data values before they are assigned to the configuration.  

### ModuleConfiguration(*initialiser*, *validator*, *eepromAddress*)

Create a new ModuleConfiguration instance with an EEPROM storage
address of *eepromAddress*.
*initialiser* is a callback function which will be used to prepare
the configuration for use.
*validator* is a callback function which will be used to validate
data values before they are assigned to the configuration.  

## METHODS

### setByte(*address*, *value*)

Validate *value* using the *validator* function declared at
instantiation and, if *validator* returns true, save *value* to
configuration location *index*. 

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

### saveByte(*index*)

Save the configuration byte at *index* to EEPROM using the EEPROM
library's update() method.

### save()

Save the configuration array to EEPROM using the EEPROM library's
put() method.

### load()

Load the configuration array from EEPROM using the EEPROM library's
get() method.
