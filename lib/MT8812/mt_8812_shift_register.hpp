#ifndef MT_8812_SHIFT_REGISTER
#define MT_8812_SHIFT_REGISTER

#include "mt_8812.hpp"
#include "../ShiftRegister/shift_register.hpp"
#include <Arduino.h>


/**
 * @brief Implementation for controlling the MT812 via a shift register for AX,AY, and DATA and an individual strobe pin
 * 
 * TODO: Document connections between uC, shift register and MT8812
 */
class MT8812ShiftRegister: public MT8812 {
    public:

        /**
         * @brief Construct new object for manipulating the MT8812 with a shift register
         * 
         * @param shift_register reference to a shift register connected directly to the uC
         * @param strobe_pin Arduino pin number connected to the MT8812 STROBE pin
         * @param reset_pin Arduino pin number connected to the MT8812 RESET pin
         */
        MT8812ShiftRegister(OutputShiftRegister* shift_register, uint8_t strobe_pin, uint8_t reset_pin) {
            this->_strobe_pin = strobe_pin;
            this->_shift_register = shift_register;
            this->_reset_pin = reset_pin;
        }

        void begin() {
            pinMode(_strobe_pin, OUTPUT);
            pinMode(_reset_pin, OUTPUT);
            // RESET is active HIGH, so this is "non-reset" state
            digitalWrite(_reset_pin, LOW);

            set_strobe_pin(false);
            _shift_register->begin();
        }

        void reset() {
            digitalWrite(_reset_pin, HIGH);
            // The calls to digitalWrite are most likely plenty long enough
            // but adding the 40ns delay just in case. 
            delayNanoseconds(40);
            digitalWrite(_reset_pin, LOW);
        }

    protected:
        void set_switch_pins(uint8_t column, uint8_t row, bool is_closed) {
            uint8_t shifted_col = (column <= 5) ? column : (column + 2);

            uint8_t output = (is_closed << 7) | ((row & 0x07) << 4) | (shifted_col & 0x0f);
            _shift_register->set_output_pins(output);
        }

        void set_strobe_pin(bool is_enabled) {
            digitalWrite(_strobe_pin, is_enabled);
        }

    private:
        uint8_t _strobe_pin;
        uint8_t _reset_pin;
        OutputShiftRegister* _shift_register;
};

#endif