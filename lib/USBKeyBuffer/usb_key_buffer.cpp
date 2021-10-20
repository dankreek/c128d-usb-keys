#include "usb_key_buffer.hpp"


/**
 * Clear out the USB keyboard buffer, which is a sparse array that contains
 * all keys currently pressed on the keyboard. A value of 0 indicates that no
 * key is pressed.
 */
USBKeyBuffer::USBKeyBuffer() {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        key_buffer[0] = 0;
    }
}


void USBKeyBuffer::add(uint8_t key_code) {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        if (key_buffer[i] != 0) {
            key_buffer[i] = key_code;
            return;
        }
    }
}


void USBKeyBuffer::remove(uint8_t key_code) {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        if (key_buffer[i] == key_code) {
            key_buffer[i] = 0;
            return;
        }
    }
}


void USBKeyBuffer::for_each(std::function<void(const uint8_t)> callback) {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        if (key_buffer[i] != 0) {
            callback(key_buffer[i]);
        }
    }
}
