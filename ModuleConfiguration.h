#ifndef MODULE_CONFIGURATION
#define MODULE_CONFGURATION

class ModuleConfiguration {

  public:
    ModuleConfiguration(unsigned int size, unsigned int eepromAddress = 0, void (*changeHandler)(unsigned int, unsigned char) = 0);
    void setInitialiser(bool (*initialiser)());

    void setByte(unsigned int index, unsigned char value);
    unsigned char getByte(unsigned int index);

    bool interact(int value = 0xffff, bool longPress = false);
    void initialise();

    void save();
    void load();

  private:
    unsigned int size;
    unsigned int eepromAddress;
    unsigned long interactionTimeout;
    void (*changeHandler)(unsigned int address, unsigned char value);
    bool (*initialiser)();
  
    unsigned char *configuration;
    
};

#endif