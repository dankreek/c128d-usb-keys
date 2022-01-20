#ifndef SHIFT_REGISTER_HPP
#define SHIFT_REGISTER_HPP

#include <Arduino.h>

class OutputShiftRegister {
    public:
        OutputShiftRegister(uint8_t data_pin, uint8_t clock_pin, uint8_t latch_pin) {
            _data_pin = data_pin;
            _clock_pin = clock_pin;
            _latch_pin = latch_pin;
        }

        void begin() {
            pinMode(_data_pin, OUTPUT);
            pinMode(_clock_pin, OUTPUT);
            pinMode(_latch_pin, OUTPUT);
            _reset_pins();
            set_output_pins(0);
        }

        void set_output_pins(uint8_t pins_state) {
            digitalWrite(_latch_pin, LOW);
            shiftOut(_data_pin, _clock_pin, MSBFIRST, pins_state);
            digitalWrite(_latch_pin, HIGH);
            digitalWrite(_latch_pin, LOW);
        }

    private:
        uint8_t _data_pin, _clock_pin, _latch_pin;

        void _reset_pins() {
            digitalWrite(_latch_pin, LOW);
            digitalWrite(_clock_pin, LOW);
            digitalWrite(_data_pin, LOW);
        }
};

#endif
