#ifndef USB_KEY_BUFFER_HPP
#define USB_KEY_BUFFER_HPP

#include <stdint.h>

// The number concurrently pressed keys that can be stored at one time
#define KEY_BUFFER_SIZE 16

// Sparse array containing all currently pressed keys. A 0x00 indicates an unused index.
extern uint8_t usb_key_buffer[KEY_BUFFER_SIZE];


void initialize_usb_key_buffer();
void add_keycode_to_buffer(uint8_t key_code);
void remove_keycode_from_buffer(uint8_t key_code);

#endif