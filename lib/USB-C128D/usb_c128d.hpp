#ifndef USB_C128D_HPP
#define USB_C128D_HPP

#include <stdint.h>
#include "usb_key_buffer.hpp"
#include "lock_key.hpp"
#include "key_mapping.hpp"

// cppcheck-suppress noConstructor
class USB_C128D {
    public:
        /** A special key that should be set or unset */
        enum SpecialKey { restore, caps_lock, forty_eighty };

        /** 
         * @brief Initialize internal data structures and dependencies
         */
        void begin();

        /**
         * @brief Poll method to update the output keyboard matrix
         */
        void task();

        /**
         * Signal that a key from the USB keyboard is currently being pressed
         * 
         * @param usb_key_code - One byte USB key code
         */
        void usb_key_down(uint8_t usb_key_code);

        /**
         * Signal that a key from the USB keyboard is no longer being pressed
         * 
         * @param usb_key_code - USB key code
         */
        void usb_key_up(uint8_t usb_key_code);

        /**
         * Object which holds the state of the C128d's CapsLock key, which is
         * emulated on the USB keyboard as F4. 
         */
        LockKey c128_capslock_lock_key = LockKey(USB_KEY_F4);

        /**
         * Holds the state of the C128D's 40/80 lock key which is emulated
         * on the USB keyboard as F7. 
         */
        LockKey c128_4080_lock_key = LockKey(USB_KEY_F7);

    protected:
        /** @return is the USB keyboard's capslock set? */
        virtual bool is_usb_capslock() = 0;

        /** @return is the USB keyboard's numlock set? */
        virtual bool is_usb_numlock() = 0;

        /**
         * @brief Set one of the keyboard matrix switches to the on of off state
         * 
         * @param row       Row number to set
         * @param column    Column number to set 
         * @param is_closed Is the switch closed or open? 
         */
        virtual void set_switch(uint8_t row, uint8_t column, bool is_closed) = 0;

        /**
         * @brief Set the state of one of the special keys.
         * 
         * These are the keys that are directly connected to ground when they 
         * are activated. 
         * 
         * @param key       Special key to set the output state of
         * @param is_closed Is this key connected to ground or lifted?
         */
        virtual void set_special_key(SpecialKey key, bool is_closed) = 0;

        /**
         * @brief Close all switches in the output keyboard matrix
         */
        virtual void reset_output_matrix() = 0;

    private:
        // Stores the current state of the output key matrix
        PinsState current_matrix_state;

        /**
         * Buffer holding all the currently pressed keys on the USB keyboard.
         * This buffer does not hold the F4 or F7 keys since those are only
         * triggered on key-down events.
         */
        USBKeyBuffer usb_key_buffer;

        void _calculate_new_matrix_state();
        void _update_cur_matrix_state();
        static bool _is_keypad_key(uint8_t key_code);
        static void _set_output_key(KeyInfo key_info);
};

#endif