#include "test_common.hpp"


#ifdef ARDUINO
#include <Arduino.h>

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    // TODO: See if this actually needs to happen on Teensy
    delay(2000);
    run_usb_keyboard_buffer_tests();
    run_lock_key_tests();
}

void loop() {
    // Flash LED to indicate tests are done
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    run_usb_keyboard_buffer_tests();
    run_lock_key_tests();
    return 0;
}

#endif