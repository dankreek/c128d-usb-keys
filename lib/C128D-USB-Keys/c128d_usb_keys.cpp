#include <Arduino.h>
#include "USBHost_t36.h"
#include "c128d_usb_keys.hpp"
#include "key_mapping.hpp"


#define C128_KEY_CAPSLOCK_TOGGLE USB_KEY_F4
#define C128_KEY_4080_TOGGLE     USB_KEY_F7

USBKeyBuffer key_buffer;

LockKey c128d_caps_lock(0, USB_KEY_F4);
LockKey c128d_40_80(0, USB_KEY_F7);


void initialize_lock_key_state() {
    // TODO: load state from EEPROM and set accordingly
    c128d_caps_lock.set_is_on(false);
    c128d_40_80.set_is_on(false);
}

/**
 * Initialize the pin state structs and the output pins  to the INPUT state 
 * which the C128D sees as "no connection." 
 */
void initialize_keyboard_output_pins() {
    for (int i=0; i < OUTPUT_PINS_COUNT; i++) {
        pins_state[i]->is_set = false;

        // Set output to digital low to be safe. This probably isn't necessary.
        pinMode(pins_state[i]->pin_num, OUTPUT);
        digitalWrite(pins_state[i]->pin_num, LOW);

        // Set pin to input mode to make it appear disconnected to the C128d
        pinMode(pins_state[i]->pin_num, INPUT);
    }

}


void _set_output_key(KeyInfo key_info) {
    if (key_info.is_sent) {
        key_info.col_pin->is_set = true;
        key_info.row_pin->is_set = true;
    }
}


void _reset_keyboard_output_pins_state() {
    for (int i=0; i < OUTPUT_PINS_COUNT; i++) {
        pins_state[i]->is_set = false;
    }
}


void _apply_keyboard_output_pins_state() {
    for (int i=0; i < OUTPUT_PINS_COUNT; i++) {
        if (pins_state[i]->is_set) {
            pinMode(pins_state[i]->pin_num, OUTPUT);
            digitalWrite(pins_state[i]->pin_num, LOW);
        } 
        else {
            pinMode(pins_state[i]->pin_num, INPUT);
        }
    }
}


bool _is_keypad_key(uint8_t key_code) {
    return ((key_code >= USB_KEY_KP_MINUS) && (key_code <= USB_KEY_KP_DOT));
}


/**
 * 1. Reset the buffered output pin states.
 * 2. Recalculate the output pin states based on the contents of the keyboard
 *    buffer and the current state of the USB caps-lock key (which emulates
 *    the C128D's shift-lock key), and the C128D caps-lock and 40/80 lock keys.
 * 3. Once the buffered state has been set, apply the state to the actual 
 *    output pins. 
 */
void update_output_pins(KeyboardController keyboard_controller) {
    _reset_keyboard_output_pins_state();

    if (c128d_caps_lock.is_on()) {
        _set_output_key(usb_key_mapping[c128d_caps_lock.usb_key_code()]);
    }

    if (c128d_40_80.is_on()) {
        _set_output_key(usb_key_mapping[c128d_40_80.usb_key_code()]);
    }

    // Set output pins state for every key in the buffer
    for (int i=0; i < USBKeyBuffer::KEY_BUFFER_SIZE; i++) {
        uint8_t usb_key_code = key_buffer.key_buffer[i];

        // If this is a pressed key, and is within the accepted range 
        if ((usb_key_code > 0) && (usb_key_code <= MAX_USB_KEY_CODE)) {
            // If caps lock is on, set left-shift (emulates shift-lock on C128D keyboard)
            if (keyboard_controller.capsLock()) {
                _set_output_key(usb_key_mapping[USB_KEY_LSHIFT]);
            }

            // If up-arrow or left-arrow is pressed, set right-shift
            // this emulates C128d cursor directions, the cursors keys are sent below
            if ((usb_key_code == USB_KEY_LEFT) || (usb_key_code == USB_KEY_UP)) {
                _set_output_key(usb_key_mapping[USB_KEY_RSHIFT]);
            }

            // Handle keypad digits depending on numlock state
            if (_is_keypad_key(usb_key_code)) {
                // If numlock is on send the key as normal (c128 number pad)
                if (keyboard_controller.numLock()) {
                    _set_output_key(usb_key_mapping[usb_key_code]);
                } 
                // If numlock is off and one of the "arrow" digits is pressed, 
                // send the C128 top-row cursor key
                else {
                    switch (usb_key_code) {
                        case USB_KEY_KP_2:
                            _set_output_key(numlock_off_kp_2);
                            break;
                        case USB_KEY_KP_4:
                            _set_output_key(numlock_off_kp_4);
                            break;
                        case USB_KEY_KP_6:
                            _set_output_key(numlock_off_kp_6);
                            break;
                        case USB_KEY_KP_8:
                            _set_output_key(numlock_off_kp_8);
                            break;
                    }
                }
            }
            // This is not a lock key or keypad digit so send it as is
            else {
                _set_output_key(usb_key_mapping[usb_key_code]);
            }
        }
    }

    _apply_keyboard_output_pins_state();
}

