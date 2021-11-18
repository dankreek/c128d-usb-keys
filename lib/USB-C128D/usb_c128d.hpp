#ifndef USB_C128D_HPP
#define USB_C128D_HPP

#include <stdint.h>
#include "usb_key_buffer.hpp"
#include "lock_key.hpp"
#include "key_mapping.hpp"

class USB_C128D {
    public:
        enum SelectedRow {
            row0 = 0, 
            row1 = 1, 
            row2 = 2, 
            row3 = 3, 
            row4 = 4, 
            row5 = 5, 
            row6 = 6, 
            row7 = 7, 
            none = -1
        };

        USB_C128D();

        /**
         * Initialize outputs pins to the "nothing on" state. 
         * This should be called directly after construction.
         */
        void init();

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
         *  Check to see if the input row from the C128 has changed.
         * 
         * If it has, recalculate all output pins and set them to the desired
         * state via all the virtual methods implemented by the platform.
         */
        void set_output_cols();

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
         * Scan the C128 keyboard's row pins and find if one of them is 
         * requesting that columns should be set.
         */
        virtual SelectedRow selected_row() = 0;

        /**
         * Set all of the selected columns to logic LOW, set unselected columns
         * to either logic HIGH (col0-col7), or NC (for k0, k1 and, k2).
         */
        virtual void set_cols(PinsRow& selected_cols) = 0;

        /**
         * Set all special key pins to either logic LOW or N/C
         */
        virtual void set_special_pins(SpecialKeys& selected_special_keys) = 0;

    private:
        PinsRow unset_cols = {
            {false, false, false, false, false, false, false, false},
            false, false, false
        };

        SpecialKeys unset_sepcial_keys = { false, false, false };

        /**
         * Buffer holding all the currently pressed keys on the USB keyboard.
         * This buffer does not hold the F4 or F7 keys since those are only
         * triggered on key-down events.
         */
        USBKeyBuffer usb_key_buffer;

        /**
         * The currently selected row pin. 
         * Output pins are only changed when the input row changes.
         */
        SelectedRow _cur_selected_row = SelectedRow::none;

        void _calculate_output_pins_state();
        static void _reset_output_pins_state();
        static bool _is_keypad_key(uint8_t key_code);
        static void _set_output_key(KeyInfo key_info);
};

#endif