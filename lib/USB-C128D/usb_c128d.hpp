#ifndef USB_C128D_HPP
#define USB_C128D_HPP

#include <stdint.h>
#include "usb_key_buffer.hpp"
#include "lock_key.hpp"
#include "key_mapping.hpp"

class USB_C128D {
    public:
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
         * Return a pointer to a PinState struct which inidcates which output
         * pins are being sent to ground based on which keys are currently
         * being pressed.
         * 
         * @param is_usb_capslock - Is the USB keyboard's capslock on?
         * @param is_usb_numlock - Is the USB keyboard's numlock on?
         */
        PinsState* get_output_pins(bool is_usb_capslock, bool is_usb_numlock);

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

    private:
        /**
         * Buffer holding all the currently pressed keys on the USB keyboard.
         * This buffer does not hold the F4 or F7 keys since those are only
         * triggered on key-down events.
         */
        USBKeyBuffer usb_key_buffer;

        static void _reset_pins_state();
        static bool _is_keypad_key(uint8_t key_code);
        static void _set_output_key(KeyInfo key_info);
};

#endif