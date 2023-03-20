#if BUILD_ENV_NAME==teensy40 or BUILD_ENV_NAME==teensy36
#include <teensy_usb_c128d.hpp>
#endif


void setup() {
	implementation.begin();
}


void loop() {
	implementation.task();
}
