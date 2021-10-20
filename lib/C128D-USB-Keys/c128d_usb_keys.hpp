#ifndef C128D_USB_KEYS_H
#define C128D_USB_KEYS_H

#include <Arduino.h>
#include "USBHost_t36.h"
#include "lock_key.hpp"
#include "usb_key_buffer.hpp"

/** 
 * On the C128D hardware keyboard the Caps Lock, and 40/80 keys are physically
 * locked by hardware. This USB keyboard adapter emulates that behavior by 
 * toggling the on/off state on a single key press. 
 * 
 * The code to handle this in the update_output_pins function called by the 
 * main loop ended up being way too complicated so these data structures exist
 * to simplify that. 
 * 
 * The lock state is toggled by the USB keyboard callbacks, and the 
 * update_output_pins function will set the output pins according to their set
 * state.
 */ 
extern LockKey c128d_caps_lock;
extern LockKey c128d_40_80;

extern USBKeyBuffer key_buffer;

void initialize_keyboard_output_pins(); 
void update_output_pins(KeyboardController keyboard_controller);
void initialize_lock_key_state();

#endif
