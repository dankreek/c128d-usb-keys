#ifndef LOCK_KEY_HPP
#define LOCK_KEY_HPP

#include <stdint.h>


class LockKey {
    public:
        /**
         * @param led_pin The Teensy 3.6 pin connected to the indicator LED
         */
        LockKey(uint8_t led_pin, uint8_t usb_key_code);

        uint8_t usb_key_code();

        /**
         * Is this lock key in the On state?
         */
        bool is_on();

        void set_is_on(bool is_on);

        void toggle_on_state();

    private:
        uint8_t _led_pin;
        uint8_t _usb_key_code;
        bool _is_on;

        void _set_led_state();
};

#endif
