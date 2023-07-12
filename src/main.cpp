#if BUILD_ENV_NAME==teensy40 or BUILD_ENV_NAME==teensy36
#include <teensy_usb_c128d.hpp>
#endif

// cppcheck-suppress unusedFunction 
void setup() {
	implementation.begin();
}


// cppcheck-suppress unusedFunction 
void loop() {
	implementation.task();
}
