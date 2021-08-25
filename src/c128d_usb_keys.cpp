#include <Arduino.h>
#include "USBHost_t36.h"
#include "c128d_usb_keys.hpp"

#define C128_KEY_CAPSLOCK_TOGGLE USB_KEY_F4
#define C128_KEY_4080_TOGGLE     USB_KEY_F7

bool is_capslock_on;
bool is_4080_on;


void initialize_lock_key_state() {
    // TODO: load state from EEPROM and set LEDs
    is_capslock_on = false;
    is_4080_on = false;
}


void initialize_usb_key_buffer() {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        usb_key_buffer[0] = 0;
    }
}


void initialize_keyboard_output_pins() {
    for (int i=0; i < OUTPUT_PINS_COUNT; i++) {
        pins_state[i]->is_set = false;

        // Set output to digital low to be safe
        pinMode(pins_state[i]->pin_num, OUTPUT);
        digitalWrite(pins_state[i]->pin_num, LOW);

        // Set pin to input mode to make it appear disconnected to the C128d
        pinMode(pins_state[i]->pin_num, INPUT);
    }

}


void add_keycode_to_buffer(uint8_t key_code) {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        if (usb_key_buffer[i] != 0) {
            usb_key_buffer[i] = key_code;
            return;
        }
    }
}


void remove_keycode_from_buffer(uint8_t key_code) {
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        if (usb_key_buffer[i] == key_code) {
            usb_key_buffer[i] = 0;
            return;
        }
    }
}

void _set_output_key(KeyInfo key_info) {
    if (key_info.is_sent) {
        key_info.col_pin->is_set = true;
        key_info.row_pin->is_set = true;
    }
}


void _toggle_caps_lock_state() {
    // TODO: Set LED on/off and store to EEPROM
    is_capslock_on = !is_capslock_on;
    if (is_capslock_on) _set_output_key(usb_key_mapping[USB_KEY_F4]);
}


void _toggle_4080_lock_state() {
    // TODO: Set LED on/off and store to EEPROM
    is_4080_on = !is_4080_on;
    if (is_4080_on) _set_output_key(usb_key_mapping[USB_KEY_F7]);
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


void update_output_pins(KeyboardController keyboard_controller) {
    _reset_keyboard_output_pins_state();

    // Set output pins state for every key in the buffer
    for (int i=0; i < KEY_BUFFER_SIZE; i++) {
        uint8_t usb_key_code = usb_key_buffer[i];

        // If this is a pressed key, and is within the accepted range 
        if ((usb_key_code > 0) && (usb_key_code <= MAX_USB_KEY_CODE)) {
            // If caps lock is on, set left-shift 
            if (keyboard_controller.capsLock()) {
                _set_output_key(usb_key_mapping[USB_KEY_LSHIFT]);
            }

            // If up-arrow or left-arrow is pressed, set right-shift
            if ((usb_key_code == USB_KEY_LEFT) || (usb_key_code == USB_KEY_UP)) {
                _set_output_key(usb_key_mapping[USB_KEY_RSHIFT]);
            }

            // If caps-lock key (F4 on USB keyboard) is pressed toggle the C128 caps-lock key's state
            if (usb_key_code == USB_KEY_F4) {
                _toggle_caps_lock_state();
            }
            // If 40/80 key (F7 on USB keyboard) is pressed toggle the C128 key's 40/80 state
            else if (usb_key_code == USB_KEY_F7) {
                _toggle_4080_lock_state();
            }
            // Handle keypad digits depending on numlock state
            else if (_is_keypad_key(usb_key_code)) {
                // If numlock is on send the key as normal (c128 number pad)
                if (keyboard_controller.numLock()) {
                    _set_output_key(usb_key_mapping[usb_key_code]);
                } 
                // If numlock is off and one of the "arrow" digits is pressed, send the C128 top-row cursor key
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

