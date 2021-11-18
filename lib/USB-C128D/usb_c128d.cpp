#include "usb_c128d.hpp"


USB_C128D::USB_C128D() {
    // Reset all output pins to unselected and force an update
    _reset_output_pins_state();
}


void USB_C128D::init() {
    set_cols(unset_cols);
    set_special_pins(unset_sepcial_keys);
}


void USB_C128D::usb_key_down(uint8_t usb_key_code) {
    // If this is one of the virtual lock keys, toggle its state
    if (usb_key_code == c128_capslock_lock_key.usb_key_code()) {
        c128_capslock_lock_key.toggle_on_state();
    } else if (usb_key_code == c128_4080_lock_key.usb_key_code()) {
        c128_4080_lock_key.toggle_on_state();
    } else {
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


/**
 * Reset all output pins for each row in the matrix back to false
 */
void USB_C128D::_reset_output_pins_state() {
    for (int row_i=0; row_i < 8; row_i++) {
        for (int col_i=0; col_i < 8; col_i++) {
            output_pins_state.rows[row_i].cols[col_i] = false;
        }

        output_pins_state.rows[row_i].k0 = false;
        output_pins_state.rows[row_i].k1 = false;
        output_pins_state.rows[row_i].k2 = false;
    }

    output_pins_state.special.restore = false;
    output_pins_state.special.caps_lock = false;
    output_pins_state.special.forty_eighty = false;
}


bool USB_C128D::_is_keypad_key(uint8_t key_code) {
    return ((key_code >= USB_KEY_KP_MINUS) && (key_code <= USB_KEY_KP_DOT));
}


void USB_C128D::_set_output_key(KeyInfo key_info) {
    if (key_info.is_sent) *key_info.pins = true;
}


void USB_C128D::set_output_cols() {
    SelectedRow new_selected_row = selected_row();

    // Only react to a state transition
    if (new_selected_row != _cur_selected_row) {
        // C128 is done reading keyboard state so clear the outputs and send them
        if (new_selected_row == SelectedRow::none) {
            set_cols(unset_cols);
        }
        else {
            // The C128 is now asking for the new keyboard state
            // so, recalculate it, and set the special pins
            if (new_selected_row == SelectedRow::row0) {
                _calculate_output_pins_state();
                set_special_pins(output_pins_state.special);
            }

            set_cols(output_pins_state.rows[new_selected_row]);
        }

        _cur_selected_row = new_selected_row;
    }
}


void USB_C128D::_calculate_output_pins_state() {
    _reset_output_pins_state();
    
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
