#ifndef TEENSY40_USB_C128D_HPP
#define TEENSY40_USB_C128D_HPP

#include <Arduino.h>
#include <usb_c128d.hpp>
#include <mt_8812.hpp>


class Teensy40_USB_C128D : public USB_C128D {
    public:
        Teensy40_USB_C128D(MT8812* mt8812) { this->_mt8812 = mt8812; }

        void begin();
        void task();

        static const uint8_t c128_caps_lock_led_pin = 23;
        static const uint8_t c128_4080_lock_led_pin = 22;

        static const uint8_t shift_register_data = 10;
        static const uint8_t shift_register_latch = 11;
        static const uint8_t shift_register_clock = 12;

        static const uint8_t restore_key_pin = 14;
        static const uint8_t forty_eighty_key_pin = 15;
        static const uint8_t capslock_key_pin = 16;

        static const uint8_t mt8812_strobe_pin = 9;
        static const uint8_t mt8812_reset_pin = 8;

    protected:
        bool is_usb_capslock(); 
        bool is_usb_numlock();

        void set_switch(uint8_t row, uint8_t column, bool is_closed) {
            _mt8812->set_switch(column, row, is_closed);
        }

        void set_special_key(SpecialKey key, bool is_closed) {
            uint8_t pin = 0;

            switch (key) {
                case restore:
                    pin = restore_key_pin;
                    break;
                case caps_lock:
                    pin = capslock_key_pin;
                    break;
                case forty_eighty:
                    pin = forty_eighty_key_pin;
                    break;
            }

            if (is_closed) {
                // On the original hardware, special keys were directly sent to 
                // ground when closed. For this implementation we will just set 
                // them to LOW, which is effectively the same thing.
                pinMode(pin, OUTPUT);
                digitalWrite(pin, LOW);
            } else {
                // If key not pressed set to INPUT indicating "no connection"
                pinMode(pin, INPUT);
            }
        };

        void reset_output_matrix() { 
            _mt8812->reset(); 
        }
    
    private:
        MT8812* _mt8812;

};

extern Teensy40_USB_C128D implementation;

#endif