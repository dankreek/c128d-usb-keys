#include <Arduino.h>
#include <teensy36_usb_c128d.hpp>
#include <shift_register.hpp>
#include <mt_8812_shift_register.hpp>


// Addressed in EEPROM where the state of the C128d's Capslock and 40/80 keys
// const int capslock_state_eeprom_addr = 0;
// const int forty_eighty_lock_state_eeprom_addr = 1;

// Read the previous states of the soft-lock keys from EEPROM and set them
void restore_lock_key_states() {
	// bool capslock_state = read_eeprom_bool(capslock_state_eeprom_addr);
	// usb_c128d.c128_capslock_lock_key.set_is_on(capslock_state);

	// bool forty_eighty_state = read_eeprom_bool(forty_eighty_lock_state_eeprom_addr);
	// usb_c128d.c128_4080_lock_key.set_is_on(forty_eighty_state);
}


void setup() {
	// Setup debugging output
	Serial.begin(115200);
	implementation.begin();
}


void loop() {
	implementation.task();
}
