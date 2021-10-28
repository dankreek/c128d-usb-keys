#include <Arduino.h>
#include <USBHost_t36.h>
#include <eeprom_funcs.hpp>
#include <usb_c128d.hpp>
#include "output_pins.hpp"

// How "bright" a lock key's indicator is. 
const uint8_t led_brightness = 120;

// Addressed in EEPROM where the state of the C128d's Capslock and 40/80 keys
const int capslock_state_eeprom_addr = 0;
const int forty_eighty_lock_state_eeprom_addr = 1;


// Add a USB host, support two hubs and use the first keyboard found
USBHost usb_host;
USBHub hub1(usb_host);
USBHub hub2(usb_host);
KeyboardController keyboard(usb_host);

// Object which contains the "state machine" for the USB->128D keybaord adapter
USB_C128D usb_c128d;

// An array of all keyboard output pins, used for convenienty initialization
uint8_t all_keyboard_pins[] = {
	ROW0_PIN, ROW1_PIN, ROW2_PIN, ROW3_PIN, ROW4_PIN, ROW5_PIN, ROW6_PIN, ROW7_PIN,
	RESTORE_PIN, FORTY_EIGHTY_PIN, CAPS_LOCK_PIN,
	COL0_PIN, COL1_PIN, COL2_PIN, COL3_PIN, COL4_PIN, COL5_PIN, COL6_PIN, COL7_PIN,
	K0_PIN, K1_PIN, K2_PIN,
	RESTORE0_PIN
};


// KeyboardController callback to handle key pressed
void on_raw_press(uint8_t key_code) {
	Serial.print("raw key press: 0x");
	Serial.println((int)key_code, HEX);
	usb_c128d.usb_key_down(key_code);
}


// KeyboardController callback to handle key releases
void on_raw_release(uint8_t key_code) {
	Serial.print("raw key release: 0x");
	Serial.println((int)key_code, HEX);
	usb_c128d.usb_key_up(key_code);
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
	usb_c128d.c128_capslock_lock_key.set_is_on(capslock_state);

	bool forty_eighty_state = read_eeprom_bool(forty_eighty_lock_state_eeprom_addr);
	usb_c128d.c128_4080_lock_key.set_is_on(forty_eighty_state);
}


void setup() {
	// Setup debugging output
	Serial.begin(115200);

	// Setup all output pins 
	for (int i=0; i < OUTPUT_PINS_COUNT; i++) {
		// INPUT signals there is no connection
		pinMode(all_keyboard_pins[i], INPUT);
    }

	// Setup LED pins
	pinMode(FORTY_EIGHTY_LOCK_LED, OUTPUT);
	pinMode(CAPSLOCK_LOCK_LED, OUTPUT);

	// restore lock key states from EEPROM
	restore_lock_key_states();

	usb_c128d.c128_capslock_lock_key.set_toggle_callback(capslock_lock_key_cb);
	usb_c128d.c128_4080_lock_key.set_toggle_callback(forty_eighty_lock_key_cb);

	// Setup USB Host and listen to the first keyboard found
	usb_host.begin();
	keyboard.attachRawPress(on_raw_press);
	keyboard.attachRawRelease(on_raw_release);
}


/**
 * Set the C128D keyboard output pin to either GND or NC (no connection)
 * 
 * @param pin_num - Teensy3.6 output pin number 
 * @param is_gnd - Is this pin set to GND (output LOW) or NC (INPUT)?
 */
void update_output_pin(uint8_t pin_num, bool is_gnd) {
	if (is_gnd) {
		pinMode(pin_num, OUTPUT);
	} else {
		pinMode(pin_num, INPUT);
	}
}


void loop() {
	// Poll the USB keyboard and send all key up/key down events
	usb_host.Task();

	PinsState* output_pins_state = usb_c128d.get_output_pins(
		keyboard.capsLock(),
		keyboard.numLock()
	);

	update_output_pin(output_pins_state->row0, ROW0_PIN);
	update_output_pin(output_pins_state->row1, ROW1_PIN);
	update_output_pin(output_pins_state->row2, ROW2_PIN);
	update_output_pin(output_pins_state->row3, ROW3_PIN);
	update_output_pin(output_pins_state->row4, ROW4_PIN);
	update_output_pin(output_pins_state->row5, ROW5_PIN);
	update_output_pin(output_pins_state->row6, ROW6_PIN);
	update_output_pin(output_pins_state->row7, ROW7_PIN);

	update_output_pin(output_pins_state->restore, RESTORE_PIN);
	update_output_pin(output_pins_state->forty_eighty, FORTY_EIGHTY_PIN);
	update_output_pin(output_pins_state->caps_lock, CAPS_LOCK_PIN);

	update_output_pin(output_pins_state->col0, COL0_PIN);
	update_output_pin(output_pins_state->col1, COL1_PIN);
	update_output_pin(output_pins_state->col2, COL2_PIN);
	update_output_pin(output_pins_state->col3, COL3_PIN);
	update_output_pin(output_pins_state->col4, COL4_PIN);
	update_output_pin(output_pins_state->col5, COL5_PIN);
	update_output_pin(output_pins_state->col6, COL6_PIN);
	update_output_pin(output_pins_state->col7, COL7_PIN);

	update_output_pin(output_pins_state->k0, K0_PIN);
	update_output_pin(output_pins_state->k1, K1_PIN);
	update_output_pin(output_pins_state->k2, K2_PIN);

	update_output_pin(output_pins_state->restore0, RESTORE0_PIN);
}
