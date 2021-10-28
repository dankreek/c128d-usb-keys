#ifndef EEPROM_FUNCS_HPP
#define EEPROM_FUNCS_HPP

bool read_eeprom_bool(int address);
void write_eeprom_bool(int address, bool value);

#endif