#ifndef USB_KEY_BUFFER_HPP
#define USB_KEY_BUFFER_HPP

#include <stdint.h>
#include <functional>

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

        /**
         * @param callback - A callback function to handle each key code 
         *                   contained in the buffer
         */
        void for_each(std::function<void(const uint8_t)> callback);


        /*
         * @returns the number of keys stored in the buffer
         */
        int count(); 

    private:
        // Sparse array containing all currently pressed keys. 
        // (A value of 0x00 indicates an unused index.)
        uint8_t key_buffer[KEY_BUFFER_SIZE];
};

#endif