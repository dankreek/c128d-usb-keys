#include <Arduino.h>
#include "USBHost_t36.h"
#include "usb_c128d.hpp"


USBHost usb_host;
USBHub hub1(usb_host);
USBHub hub2(usb_host);
KeyboardController keyboard(usb_host);


void on_raw_press(uint8_t keycode) {
	Serial.print("raw key press: 0x");
	Serial.println((int)keycode, HEX);

	// if (keycode == c128d_caps_lock.usb_key_code()) {
	// 	c128d_caps_lock.toggle_on_state();
	// }
	// else if (keycode == c128d_40_80.usb_key_code()) {
	// 	c128d_40_80.toggle_on_state();
	// }
	// else {
	// 	key_buffer.add(keycode);
	// }
}


void on_raw_release(uint8_t keycode) {
	Serial.print("raw key release: 0x");
	Serial.println((int)keycode, HEX);

	// if ((keycode != c128d_caps_lock.usb_key_code()) && (keycode != c128d_40_80.usb_key_code())) {
	// 	key_buffer.remove(keycode);
	// }
}


void setup() {
	// Setup debugging output
	// while (!Serial) ; 
	Serial.begin(115200);
	pinMode(13, OUTPUT);
	// Setup USB Host and listen to the first keyboard found
	// usb_host.begin();
	// keyboard.attachRawPress(on_raw_press);
	// keyboard.attachRawRelease(on_raw_release);
}


void loop() {
	// Poll the USB keyboard and update the USB key buffer
	// usb_host.Task();

    // Flash LED to indicate tests are done
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
	// Once the USB keyboard buffer has been updated, derive output state from it

	Serial.write("Piss and poop\n");
}

