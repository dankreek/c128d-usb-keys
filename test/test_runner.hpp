#ifndef TEST_RUNNER_HPP
#define TEST_RUNNER_HPP

void run_usb_keyboard_buffer_tests();
void run_lock_key_tests();
void run_usb_c128d_tests();

#ifdef ARDUINO
void run_eeprom_bool_tests();
#endif

#endif