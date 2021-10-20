#include "usb_key_buffer.hpp"


uint8_t usb_key_buffer[KEY_BUFFER_SIZE];


/**
 * Clear out the USB keyboard buffer, which is a sparse array that contains
 * all keys currently pressed on the keyboard. A value of 0 indicates that no
 * key is pressed.
 */
void initialize_usb_key_buffer() {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        usb_key_buffer[0] = 0;
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
