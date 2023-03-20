#include <Arduino.h>
#include <teensy_usb_c128d.hpp>
#include <shift_register.hpp>
#include <mt_8812_shift_register.hpp>


void setup() {
	implementation.begin();
}


void loop() {
	implementation.task();
}
