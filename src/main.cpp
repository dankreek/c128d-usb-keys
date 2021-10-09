#include <Arduino.h>
#include "USBHost_t36.h"
#include "c128d_usb_keys.hpp"


USBHost usb_host;
USBHub hub1(usb_host);
USBHub hub2(usb_host);
KeyboardController keyboard(usb_host);


void OnRawPress(uint8_t keycode) {
	Serial.print("raw key press: 0x");
	Serial.println((int)keycode, HEX);

	if (keycode == c128d_caps_lock.usb_key_code()) {
		c128d_caps_lock.toggle_on_state();
	}
	else if (keycode == c128d_40_80.usb_key_code()) {
		c128d_40_80.toggle_on_state();
	}
	else {
		add_keycode_to_buffer(keycode);
	}
}


void OnRawRelease(uint8_t keycode) {
	Serial.print("raw key release: 0x");
	Serial.println((int)keycode, HEX);

	if ((keycode != c128d_caps_lock.usb_key_code()) && (keycode != c128d_40_80.usb_key_code())) {
		remove_keycode_from_buffer(keycode);
	}
}


void setup() {
	// Setup debugging output
	while (!Serial) ; 
	Serial.begin(115200);

	// Initialize data structures
	initialize_usb_key_buffer();
	initialize_lock_key_state();
	initialize_keyboard_output_pins();

	// Setup USB Host and listen to the first keyboard found
	usb_host.begin();
	keyboard.attachRawPress(OnRawPress);
	keyboard.attachRawRelease(OnRawRelease);
}


void loop() {
	// Poll the USB keyboard and update the USB key buffer
	usb_host.Task();

	// Once the USB keyboard buffer has been updated, derive output state from it
	update_output_pins(keyboard);
}

