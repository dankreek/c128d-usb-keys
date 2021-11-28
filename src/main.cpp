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

#define S0 33
#define S1 34
#define S2 35
#define Z 36
#define READ_DELAY_US 1

#define COL0 32
#define COL1 31
#define COL2 30
#define COL3 29
#define COL4 28
#define COL5 27
#define COL6 26
#define COL7 25


elapsedMillis since_measure;
int select_count[8];
int multiples_count;
uint8_t last_selected;
uint8_t last_multi_pattern;
uint8_t full_count;
uint8_t empty_count;


void clear_measurements() {
	for (int i=0; i < 8; i++) {
		select_count[i] = 0;
	}

	since_measure = 0;
	multiples_count = 0;
	last_selected = 0;
	full_count = 0;
	empty_count = 0;
}



void setup() {
	// Setup debugging output
	Serial.begin(115200);

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

	pinMode(S0, OUTPUT);
	pinMode(S1, OUTPUT);
	pinMode(S2, OUTPUT);
	pinMode(Z, INPUT);

	pinMode(COL0, INPUT);
	pinMode(COL1, INPUT);
	pinMode(COL2, INPUT);
	pinMode(COL3, INPUT);
	pinMode(COL4, INPUT);
	pinMode(COL5, INPUT);
	pinMode(COL6, INPUT);
	pinMode(COL7, INPUT);

	clear_measurements();
}

uint8_t selected_cols_mux() {
	uint8_t retval = 0;

	digitalWriteFast(S0, HIGH);
	digitalWriteFast(S1, HIGH);
	digitalWriteFast(S2, HIGH);
	delayMicroseconds(READ_DELAY_US);
	if (!(digitalReadFast(Z) && digitalReadFast(Z))) retval++;
	retval <<= 1;

	digitalWriteFast(S0, LOW);
	delayMicroseconds(READ_DELAY_US);
	if (!(digitalReadFast(Z) && digitalReadFast(Z))) retval++;
	retval <<= 1;

	digitalWriteFast(S0, HIGH);
	digitalWriteFast(S1, LOW);
	delayMicroseconds(READ_DELAY_US);
	if (!(digitalReadFast(Z) && digitalReadFast(Z))) retval++;
	retval <<= 1;

	digitalWriteFast(S0, LOW);
	delayMicroseconds(READ_DELAY_US);
	if (!(digitalReadFast(Z) && digitalReadFast(Z))) retval++;
	retval <<= 1;

	digitalWriteFast(S0, HIGH);
	digitalWriteFast(S1, HIGH);
	digitalWriteFast(S2, LOW);
	delayMicroseconds(READ_DELAY_US);
	if (!(digitalReadFast(Z) && digitalReadFast(Z))) retval++;
	retval <<= 1;

	digitalWriteFast(S0, LOW);
	delayMicroseconds(READ_DELAY_US);
	if (!(digitalReadFast(Z) && digitalReadFast(Z))) retval++;
	retval <<= 1;

	digitalWriteFast(S0, HIGH);
	digitalWriteFast(S1, LOW);
	delayMicroseconds(READ_DELAY_US);
	if (!(digitalReadFast(Z) && digitalReadFast(Z))) retval++;
	retval <<= 1;

	digitalWriteFast(S0, LOW);
	delayMicroseconds(READ_DELAY_US);
	if (!(digitalReadFast(Z) && digitalReadFast(Z))) retval++;

	return retval;
}


uint8_t selected_cols() {
	uint8_t retval = 0;

	if (!digitalReadFast(COL7)) retval++;
	retval <<= 1;

	if (!digitalReadFast(COL6)) retval++;
	retval <<= 1;

	if (!digitalReadFast(COL5)) retval++;
	retval <<= 1;

	if (!digitalReadFast(COL4)) retval++;
	retval <<= 1;

	if (!digitalReadFast(COL3)) retval++;
	retval <<= 1;

	if (!digitalReadFast(COL2)) retval++;
	retval <<= 1;

	if (!digitalReadFast(COL1)) retval++;
	retval <<= 1;

	if (!digitalReadFast(COL0)) retval++;
	
	return retval;
}

bool is_selected(uint8_t mask, int bit_num) {
	return ((mask >> bit_num) & 0x01) == 1;
}

void loop() {
	// also store stats for when multiple pins are brought low
	uint8_t selected = selected_cols();

	if (selected != last_selected) {
		for (int i=0; i < 8; i++) {
			if (is_selected(selected, i) && !is_selected(last_selected, i)) {
				select_count[i] += 1;
			}
		}

		switch (selected) {
			case 0x01:
			case 0x02:
			case 0x04:
			case 0x08:
			case 0x10:
			case 0x20:
			case 0x40:
			case 0x80:
				break;
			case 0x00:
				empty_count++;
				break;
			case 0xff:
				full_count++;
				break;
			default:
				multiples_count++;
				last_multi_pattern = selected;
				break;
		};

		last_selected = selected;
	}

	if (since_measure >= 1000) {
		for (int i=0; i < 8; i++) {
			Serial.print(i);
			Serial.print("=");
			Serial.print(select_count[i]);
			Serial.print("hz ");
		}
		Serial.println();
		Serial.print("Empty=");
		Serial.print(empty_count);
		Serial.print(" Full=");
		Serial.print(full_count);
		Serial.print(" Multiples=");
		Serial.print(multiples_count);
		Serial.print(" Pattern=");
		Serial.println(last_multi_pattern, HEX);

		clear_measurements();
	}
}
