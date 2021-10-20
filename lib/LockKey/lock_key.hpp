#ifndef LOCK_KEY_HPP
#define LOCK_KEY_HPP

#include <stdint.h>


class LockKey {
    public:
        /**
         * @param usb_key_code - USB key code which this lock key is triggered by
         */
        LockKey(uint8_t usb_key_code);

        /**
         * Set a callback to be triggered any time the lock state changes. 
         * The callback will be called while this method executes.
         * 
         * @param toggle_callback - A function to be called when the lock key is
         *                          toggled. The callback parameter is set to 
         *                          the current lock state of the lock key.
         */
        void set_toggle_callback(void (*toggle_callback)(bool)); 

        /**
         * @returns The USB key code which toggles the lock state
         */
        uint8_t usb_key_code();

        /**
         * @returns Is this lock key in the On state?
         */
        bool is_on();

        /**
         * Manually set the toggle state of the lock key
         */
        void set_is_on(bool is_on);

        /**
         * Toggle the current lock state
         */
        void toggle_on_state();

    private:
        uint8_t _usb_key_code;
        void _call_callback();
        void (*_toggle_callback)(bool);
        bool _is_on;
};

#endif
