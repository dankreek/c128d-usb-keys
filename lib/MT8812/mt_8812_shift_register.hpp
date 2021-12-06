#ifndef MT_8812_SHIFT_REGISTER
#define MT_8812_SHIFT_REGISTER

#include "mt_8812.hpp"
#include <shift_register.hpp>
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
         * @param shift_register reference to an initialized shift register connected directly to the uC
         * @param strobe_pin Arduino pin number which is connected to the MT8812 STROBE pin
         */
        MT8812ShiftRegister(OutputShiftRegister* shift_register, uint8_t strobe_pin) {
            this->_strobe_pin = strobe_pin;
            this->_shift_register = shift_register;
        }

        // TODO: explain why this is hacky
        void reset() {
            for (int col_i=0; col_i < 12; col_i++) {
                for (int row_i=0; row_i < 8; row_i++) {
                    set_switch(col_i, row_i, false);
                }
            }
        }

    protected:
        void set_switch_pins(uint8_t column, uint8_t row, bool is_closed) {
            uint8_t output = (is_closed << 7) | ((row & 0x07) << 4) | (column & 0x0f);
            _shift_register->set_output_pins(output);
        }

        void set_strobe_pin(bool is_enabled) {
            digitalWrite(_strobe_pin, is_enabled);
        }

    private:
        uint8_t _strobe_pin;
        OutputShiftRegister* _shift_register;
};

#endif