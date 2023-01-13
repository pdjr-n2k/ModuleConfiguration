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
address specified by unsigned int *eepromAddress*.
*initialiser* is a callback function which will be used to prepare
the configuration for use.
*validator* is a callback function which will be used to validate
data values before they are assigned to the configuration.  

## METHODS

### setByte(*index*, *value*)

Validate *value* using the *validator* function declared at
instantiation and, if *validator* returns true, save *value* to
configuration location *index*. 

### getByte(*index*)

Return the configuration byte stored at configuration location
*index*.

### interact(*value*, *longPress*)

The method supports a two-step data entry protocal that can be used to
update values in the module configuration array.
Data is assumed to derive from an input register (most likely a DIL
switch) and the interact() the method to be invoked by operation of a
push-button.
The method expects to be invoked each time the push-button is operated.
Additionally, because the method implements a timeout mechanism, the
method must be called from loop with no arguments.

The supported protocol takes a long-button press to signify entry of a
configuration array index and a short button press to signify entry of
an array value.
Together, the sequential entry of an index followed by a value supplies
the data required for an array update.

If a data value does not follow an index value withing 30 seconds then
the supplied index is discarded and the protocol reset.
If a data value is entered without a preceeding index, then the supplied
value is ignored and an exception reported.

The status of protocol processing is indicated by the method's return
value.

0  says that a short press has been processed but no prior array index
   was available and no action has been taken.
   Zero is also returned from a loop invocation when no action has been
   taken.
1  says a long press has been processed and the supplied value accepted
   as a valid configuration array index.
   Any subsequent short press value will be saved to this address.
-1 says a long press has been processed but the suppied value was not
   a valid configuration array index.
2  says a short press has been processed and the supplied value validated
   and stored at a previously supplied configuration address.
-2 says a short press has been processed but the supplied value did not
   validate and has not been stored in the configuration array.
10 says that a loop invocation has timed out an interaction sequence and
   the protocol reset. 

### saveByte(*index*)

Save the configuration byte at *index* to EEPROM using the EEPROM
library's update() method.

### save()

Save the configuration array to EEPROM using the EEPROM library's
put() method.

### load()

Load the configuration array from EEPROM using the EEPROM library's
get() method.
