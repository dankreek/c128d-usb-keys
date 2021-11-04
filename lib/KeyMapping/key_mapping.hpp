#ifndef KEY_MAPPING_HPP
#define KEY_MAPPING_HPP

#include <stdint.h>
#include "key_defs.hpp"

// Keys which are sent to LOW by themselves (not part of the matrix)
struct SpecialKeys {
    bool restore;    
    bool forty_eighty;
    bool caps_lock;
};

// One row of pins in the keybaord matrix
struct PinsRow {
    bool cols[8]; 
    bool k0, k1, k2;         
};

// Represents the state of all output pins, in the keyboar matrix. 
struct PinsState {
    PinsRow rows[8];       
    SpecialKeys special;
};

// An instance of PinsState for the key mapping 
extern PinsState output_pins_state;

// Describes a mapping of a USB keyboard key to the keyboard matrix on a C128D.
struct KeyInfo {
    bool is_sent;
    bool *pins;    // Pointer to the rows[].cols[] where the key crosses
};

/**
 * Every entry in the array refers to a USB key code. Any key code which isn't
 * directly sent to the C128D will have is_sent set to false, otherwise is_sent
 * will be set to true, and row_pin and col_pin will point to the KeyInfo 
 * which indicates which Teensy output pins are set to OUTPUT LOW.
 */
extern KeyInfo usb_key_mapping[MAX_USB_KEY_CODE + 1]; 

// Mappings for the keypad when numlock is off
extern KeyInfo numlock_off_kp_2;
extern KeyInfo numlock_off_kp_4;
extern KeyInfo numlock_off_kp_6;
extern KeyInfo numlock_off_kp_8;


#endif