#ifndef USB_KEY_BUFFER_HPP
#define USB_KEY_BUFFER_HPP

#include <stdint.h>


class USBKeyBuffer {
    public:
        // The number of keys that can be stored in the buffer at once
        int static const KEY_BUFFER_SIZE = 16;

        /**
         * Default constructor which initializes the internal key_buffer
         */
        USBKeyBuffer();
    
        /**
         * @param key_code - The USB keycode to add to the bufer
         */
        void add(uint8_t key_code);

        /**
         * @param key_code - The USB keycode to remove from the buffer
         */
        void remove(uint8_t key_code);

        // Sparse array containing all currently pressed keys. 
        // (A value of 0x00 indicates an unused index.)
        // TODO: Make this private and a friend to system container class
        uint8_t key_buffer[KEY_BUFFER_SIZE];
};

#endif