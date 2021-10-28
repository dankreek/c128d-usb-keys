#ifdef ARDUINO
#include <Arduino.h>
#include <EEPROM.h>
#include <eeprom_funcs.hpp>
#include <unity.h>


void test_read_write_eeprom_bools() {
    write_eeprom_bool(0, false);
    TEST_ASSERT_FALSE(read_eeprom_bool(0));

    write_eeprom_bool(0, true);
    TEST_ASSERT_TRUE(read_eeprom_bool(0));
}


void run_eeprom_bool_tests() {
    RUN_TEST(test_read_write_eeprom_bools);

    write_eeprom_bool(0, false);
    write_eeprom_bool(1, false);
}

#endif