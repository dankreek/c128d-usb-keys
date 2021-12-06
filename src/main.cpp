#include <Arduino.h>
#include <USBHost_t36.h>
#include <eeprom_funcs.hpp>
#include <usb_c128d.hpp>
#include <shift_register.hpp>
#include "output_pins.hpp"


// How "bright" a lock key's indicator is. 
const uint8_t led_brightness = 120;

// Addressed in EEPROM where the state of the C128d's Capslock and 40/80 keys
const int capslock_state_eeprom_addr = 0;
const int forty_eighty_lock_state_eeprom_addr = 1;


// Add a USB host, support two hubs and use the first keyboard found
// USBHost usb_host;
// USBHub hub1(usb_host);
// USBHub hub2(usb_host);
// KeyboardController keyboard(usb_host);

// An array of all keyboard output pins, used for convenienty initialization
// uint8_t all_keyboard_pins[] = {
// 	ROW0_PIN, ROW1_PIN, ROW2_PIN, ROW3_PIN, ROW4_PIN, ROW5_PIN, ROW6_PIN, ROW7_PIN,
// 	RESTORE_PIN, FORTY_EIGHTY_PIN, CAPS_LOCK_PIN,
// 	COL0_PIN, COL1_PIN, COL2_PIN, COL3_PIN, COL4_PIN, COL5_PIN, COL6_PIN, COL7_PIN,
// 	K0_PIN, K1_PIN, K2_PIN
// };


// KeyboardController callback to handle key pressed
void on_raw_press(uint8_t key_code) {
	Serial.print("raw key press: 0x");
	Serial.println((int)key_code, HEX);
	// usb_c128d.usb_key_down(key_code);
}


// KeyboardController callback to handle key releases
void on_raw_release(uint8_t key_code) {
	Serial.print("raw key release: 0x");
	Serial.println((int)key_code, HEX);
	// usb_c128d.usb_key_up(key_code);
}


// Set an LED connected to a PWM pin to be either off or set to a defined brightness
void set_led(uint8_t led_pin, bool is_lit) {
	uint8_t value = is_lit ? led_brightness : 0;
	analogWrite(led_pin, value);
}


void capslock_lock_key_cb(bool is_locked) {
	Serial.print("C128D Capslock ");
	if (is_locked) Serial.println("ON"); else Serial.println("OFF");

	set_led(CAPSLOCK_LOCK_LED, is_locked);

	// store lock key state in EEPROM to be restored on reset
	write_eeprom_bool(capslock_state_eeprom_addr, is_locked);
}


void forty_eighty_lock_key_cb(bool is_locked) {
	Serial.print("C128D 40/80 lock ");
	if (is_locked) Serial.println("ON"); else Serial.println("OFF");

	set_led(FORTY_EIGHTY_LOCK_LED, is_locked);

	// store lock key state in EEPROM to be restored on reset
	write_eeprom_bool(forty_eighty_lock_state_eeprom_addr, is_locked);
}


// Read the previous states of the soft-lock keys from EEPROM and set them
void restore_lock_key_states() {
	bool capslock_state = read_eeprom_bool(capslock_state_eeprom_addr);
	// usb_c128d.c128_capslock_lock_key.set_is_on(capslock_state);

	bool forty_eighty_state = read_eeprom_bool(forty_eighty_lock_state_eeprom_addr);
	// usb_c128d.c128_4080_lock_key.set_is_on(forty_eighty_state);
}

OutputShiftRegister shift_register(30, 32, 31);
uint8_t counter;

void setup() {
	// Setup debugging output
	Serial.begin(115200);
	counter = 63;

	// Setup all output pins 
	// for (int i=0; i < OUTPUT_PINS_COUNT; i++) {
	// 	// INPUT signals there is no connection
	// 	pinMode(all_keyboard_pins[i], INPUT);
    // }

	// Setup LED pins
	// pinMode(FORTY_EIGHTY_LOCK_LED, OUTPUT);
	// pinMode(CAPSLOCK_LOCK_LED, OUTPUT);

	// restore lock key states from EEPROM
	// restore_lock_key_states();

	// usb_c128d.c128_capslock_lock_key.set_toggle_callback(capslock_lock_key_cb);
	// usb_c128d.c128_4080_lock_key.set_toggle_callback(forty_eighty_lock_key_cb);

	// Setup USB Host and listen to the first keyboard found
	// usb_host.begin();
	// keyboard.attachRawPress(on_raw_press);
	// keyboard.attachRawRelease(on_raw_release);
	shift_register.begin();
}

uint8_t rotl(uint8_t input) {
	bool has_hi_bit = input & 0x80;
	return (input << 1) | (has_hi_bit ? 0x01 : 0x00);
}

void loop() {
    shift_register.set_output_pins(counter);
	counter += 1;
	delay(500);
}
