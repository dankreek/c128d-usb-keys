#include <Arduino.h>
#include "c128d_usb_keys.hpp"


void initialize_usb_key_buffer() {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        usb_key_buffer[0] = 0;
    }
}


void initialize_pins_state() {
    for (int i=0; i < OUTPUT_PINS_COUNT; i++) {
        pins_state[i]->is_set = false;
    }
}


void add_keycode_to_buffer(uint8_t key_code) {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        if (usb_key_buffer[i] != 0) {
            usb_key_buffer[i] = key_code;
            return;
        }
    }
}


void remove_keycode_from_buffer(uint8_t key_code) {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        if (usb_key_buffer[i] == key_code) {
            usb_key_buffer[i] = 0;
            return;
        }
    }
}

