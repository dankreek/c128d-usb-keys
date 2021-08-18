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

	add_keycode_to_buffer(keycode);
}


void OnRawRelease(uint8_t keycode) {
	Serial.print("raw key release: 0x");
	Serial.println((int)keycode, HEX);

	remove_keycode_from_buffer(keycode);
}


void setup() {
	// Setup debugging output
	while (!Serial) ; 
	Serial.begin(115200);

	// Initialize data structures
	initialize_pins_state();
	initialize_usb_key_buffer();

	// Setup USB Host and listen to the first keyboard found
	usb_host.begin();
	keyboard.attachRawPress(OnRawPress);
	keyboard.attachRawRelease(OnRawRelease);
}


void loop() {
	// Poll the USB keyboard and fire events for key-on or key-off events
	usb_host.Task();

	// Once the USB keyboard buffer has been updated, derive output state from it

}

