#ifndef MT_8812_HPP
#define MT_8812_HPP

#include <stdint.h>

/**
 * @brief Functionality for controlling the MT8812 analog switch array
 */
class MT8812 {
    public:
        /**
         * @brief Set the switch object
         * 
         * @param column 
         * @param row 
         * @param is_closed 
         */
        void set_switch(uint8_t column, uint8_t row, bool is_closed) {
            set_strobe_pin(false);
            set_switch_pins(column, row, is_closed);
            set_strobe_pin(true);
            set_strobe_pin(false);
        }

        /**
         * @brief Close all switches on the MT8812 
         * 
         * This is most easily accomplished by pulling the RESET pin HIGH, but
         * may need to be manually accomlished depending upon how many pins
         * are available.
         */
        virtual void reset() = 0;

    protected:
        /**
         * @brief Set the AX, AY and DATA pins to either open or close a switch
         * 
         * @param column    Column number (0-11), which sets the AX pins
         * @param row       Row number (0-7), which sets the AY pins
         * @param is_closed Is the switch open or closed? DATA=LOW if open, DATA=HIGH if closed
         */
        virtual void set_switch_pins(uint8_t column, uint8_t row, bool is_closed) = 0;

        /**
         * @brief Set the strobe pin state to set register
         * 
         * Note that the implementation of this method should include the
         * proper delay for the chip to receive the set_switch_pins instruction
         * 
         * @param is_enabled If enabled, set STROBE=HIGH, otherwise STROBE=LOW
         */
        virtual void set_strobe_pin(bool is_enabled) = 0;
};

#endif
