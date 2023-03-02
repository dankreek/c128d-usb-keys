// #include <Arduino.h>
#include <cstring>
#include "usb_c128d.hpp"


void USB_C128D::begin() {
    // Reset all output pins to unselected and force an update
    output_pins_state.reset();
    current_matrix_state.reset();
    reset_output_matrix();
}


void USB_C128D::task() {
    _calculate_new_matrix_state();
    _update_cur_matrix_state();
}


void USB_C128D::usb_key_down(uint8_t usb_key_code) {
    // If this is one of the virtual lock keys, toggle its state
    if (usb_key_code == c128_capslock_lock_key.usb_key_code()) {
        c128_capslock_lock_key.toggle_on_state();
    } else if (usb_key_code == c128_4080_lock_key.usb_key_code()) {
        c128_4080_lock_key.toggle_on_state();
    } else {
        // Serial.write("Key down: ");
        // Serial.println(usb_key_code);
        usb_key_buffer.add(usb_key_code);
    }
}


void USB_C128D::usb_key_up(uint8_t usb_key_code) {
    // If this is one of the virtual lock keys ignore it
	if ((usb_key_code != c128_capslock_lock_key.usb_key_code()) && 
        (usb_key_code != c128_4080_lock_key.usb_key_code())) {
		usb_key_buffer.remove(usb_key_code);
	}
}


bool USB_C128D::_is_keypad_key(uint8_t key_code) {
    return ((key_code >= USB_KEY_KP_MINUS) && (key_code <= USB_KEY_KP_DOT));
}


void USB_C128D::_set_output_key(KeyInfo key_info) {
    if (key_info.is_sent) *key_info.pins = true;
}


void USB_C128D::_update_cur_matrix_state() {
    for (int row_i=0; row_i < 8; row_i++) {
        for (int col_i=0; col_i < 11; col_i++) {
            bool desired_state = output_pins_state.rows[row_i].cols[col_i];
            if (current_matrix_state.rows[row_i].cols[col_i] != desired_state) {
                set_switch(row_i, col_i, desired_state);
                current_matrix_state.rows[row_i].cols[col_i] = desired_state;
            }
        }
    }

    if (output_pins_state.special.caps_lock != current_matrix_state.special.caps_lock) {
        set_special_key(caps_lock, output_pins_state.special.caps_lock);
        current_matrix_state.special.caps_lock = output_pins_state.special.caps_lock;
    }
    
    if (output_pins_state.special.forty_eighty != current_matrix_state.special.forty_eighty) {
        set_special_key(forty_eighty, output_pins_state.special.forty_eighty);
        current_matrix_state.special.forty_eighty = output_pins_state.special.forty_eighty;
    }

    if (output_pins_state.special.restore != current_matrix_state.special.restore) {
        set_special_key(restore, output_pins_state.special.restore);
        current_matrix_state.special.restore = output_pins_state.special.restore;
    }
}


void USB_C128D::_calculate_new_matrix_state() {
    output_pins_state.reset();
    
    if (c128_capslock_lock_key.is_on()) {
        _set_output_key(usb_key_mapping[c128_capslock_lock_key.usb_key_code()]);
    }

    if (c128_4080_lock_key.is_on()) {
        _set_output_key(usb_key_mapping[c128_4080_lock_key.usb_key_code()]);
    }

    // If the USB capslock is on, set left-shift 
    // (emulates shift-lock on C128D keyboard)
    if (is_usb_capslock()) {
        _set_output_key(usb_key_mapping[USB_KEY_LSHIFT]);
    }

    // Set the output pins state for every key in the buffer
    bool usb_numlock = is_usb_numlock();
    usb_key_buffer.for_each([usb_numlock](uint8_t usb_key_code) {
        // If this key is within the accepted range
        if (usb_key_code <= MAX_USB_KEY_CODE) {
            // If up-arrow or left-arrow is pressed, set right-shift
            // (emulates C128D cursor directions, the cursor keys are sent below)
            if ((usb_key_code == USB_KEY_LEFT) || (usb_key_code == USB_KEY_UP)) {
                _set_output_key(usb_key_mapping[USB_KEY_RSHIFT]);
            }

            // Handle USB keypad keys depending on USB numlock state
            if (_is_keypad_key(usb_key_code)) {
                // If numlock is on just send the keypad digit directly
                if (usb_numlock) {
                    _set_output_key(usb_key_mapping[usb_key_code]);
                }
                // If numlock is off and one of the keypad arrow digits pressed
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
            // This is not a lock key or keypad digit so send it as-is
            else {
                _set_output_key(usb_key_mapping[usb_key_code]);
            }
        }
    });
}
