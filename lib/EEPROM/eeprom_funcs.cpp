#ifdef ARDUINO

#include "eeprom_funcs.hpp"
#include <Arduino.h>
#include <EEPROM.h>


bool read_eeprom_bool(int address) {
    return (bool)EEPROM.read(address);
}


void write_eeprom_bool(int address, bool value) {
    EEPROM.write(address, (uint8_t)value);
}

#endif