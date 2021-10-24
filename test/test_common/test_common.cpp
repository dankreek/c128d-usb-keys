#include "test_common.hpp"


#ifdef ARDUINO
#include <Arduino.h>

// I found the solution to this issue at https://forum.pjrc.com/threads/29177-Teensy-3-1-signalr-c-(-text-_kill_r-0xe)-undefined-reference-to-_kill-error
extern "C"{
  int _write(){return -1;}
}


void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    // TODO: See if this actually needs to happen on Teensy
    delay(2000);
    run_usb_keyboard_buffer_tests();
    run_lock_key_tests();
    run_usb_c128d_tests();
}

void loop() {
    // Flash LED to indicate tests are done
	pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    run_usb_keyboard_buffer_tests();
    run_lock_key_tests();
    run_usb_c128d_tests();
    return 0;
}

#endif