# ModuleConfiguration

## CONSTRUCTORS

### ModuleConfiguration(*maxsize*)

Create a new ModuleConfiguration instance with a limit of *maxsize* bytes.

### ModuleConfiguration(*size*, *eepromAddress*)

Create a new ModuleConfiguration instance with a limit of *size* bytes
and an EEPROM start address of *eepromAddress*.

### ModuleConfiguration(*size*, *eepromAddress*, *changeHandler*)

Create a new ModuleConfiguration instance with a limit of *size* bytes
and an EEPROM start address of *eepromAddress*.
*changeHandler* is a callback function which will be invoked each time
a configuration interaction results in a module configuration change.

## METHODS

### setInitialiser(*initialiser*)

Set the callback which will be invoked by the initialise() method to
the function *initialiser*.

### setByte(*address*, *value*)

Set the configuration byte at *address* to *value*.

### getByte(*address*)

Return the configuration byte at *address*.

### interact(*value*, *longPress*)

Process an interaction event that has generated *value* as the result
of a long or short button press as indicated by the boolean value
*longPress*.

The method returns boolean true if the interaction requires a
subsequent invocation of the interact() method to complete and false
if the interaction protocol is complete.
This return value can be used by the host application to prompt or
otherwise signify to a user the state of an interaction protocol.

In the case that an interaction has completed and a change handler was
registered at instantiation then the change handler is invoked with the
affected address and its new value as arguments.

### initialise()

If the host application has previously registered an initialiser
function then the initialiser is called and its return value returned
as the result of the method call.

### save()

Save the configuration to EEPROM, or, if no EEPROM address has been
configured, silently do nothing.

### load()

Load the configuration from EEPROM, or, if no EEPROM address has been
configured, silently do nothing.
