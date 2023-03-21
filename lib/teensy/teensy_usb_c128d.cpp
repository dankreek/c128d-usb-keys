#include <Arduino.h>
#include <EEPROM.h>
#include <USBHost_t36.h>
#include <mt_8812_shift_register.hpp>
#include "teensy_usb_c128d.hpp"

// Add a USB host, support two hubs and use the first keyboard found
USBHost usb_host;
USBHub hub1(usb_host);
USBHub hub2(usb_host);
KeyboardController keyboard(usb_host);

void raw_press(uint8_t key_code) {
    implementation.usb_key_down(key_code); 
}


void raw_release(uint8_t key_code) { 
    implementation.usb_key_up(key_code); 
}


bool Teensy_USB_C128D::is_usb_capslock() { return keyboard.capsLock(); }
bool Teensy_USB_C128D::is_usb_numlock() { return keyboard.numLock(); }


void capslock_toggled(bool is_set) {
    digitalWrite(Teensy_USB_C128D::c128_caps_lock_led_pin, is_set ? HIGH : LOW);
    EEPROM.put(Teensy_USB_C128D::eeprom_caps_lock_key_address, (uint8_t)is_set);
}


void forty_eighty_toggled(bool is_set) {
    digitalWrite(Teensy_USB_C128D::c128_4080_lock_led_pin, is_set ? HIGH : LOW);
    EEPROM.put(Teensy_USB_C128D::eeprom_40_80_key_address, (uint8_t)is_set);
}


void Teensy_USB_C128D::begin() {
    // Turn on the "power" LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // Setup special keys, INPUT means "no connection"
    pinMode(restore_key_pin, INPUT);
    pinMode(forty_eighty_key_pin, INPUT);
    pinMode(capslock_key_pin, INPUT);

    // Setup lock key indicator LED's
    pinMode(c128_caps_lock_led_pin, OUTPUT);
    pinMode(c128_4080_lock_led_pin, OUTPUT);

    bool is_capslock_set;
    bool is_4080_set;

    // If this is the first startup set defaults in EEPROM
    if (this->is_first_startup())  {
        is_capslock_set = false;
        is_4080_set = false;

        EEPROM.write(Teensy_USB_C128D::eeprom_caps_lock_key_address, 0);
        EEPROM.write(Teensy_USB_C128D::eeprom_caps_lock_key_address, 0);

        // Set EEPROM to recognize the first startup has happened
        EEPROM.write(Teensy_USB_C128D::eeprom_first_startup_address, 0);
        EEPROM.write(Teensy_USB_C128D::eeprom_version_address, 0);
    } else {
        is_capslock_set = (bool)EEPROM.read(Teensy_USB_C128D::eeprom_caps_lock_key_address);
        is_4080_set = (bool)EEPROM.read(Teensy_USB_C128D::eeprom_40_80_key_address);
    }

    // Set output pins to C128D
    this->set_special_key(caps_lock, is_capslock_set);
    this->set_special_key(forty_eighty, is_4080_set);

    // Set lock key indicators
    digitalWrite(c128_caps_lock_led_pin, is_capslock_set ? HIGH : LOW);
    digitalWrite(c128_4080_lock_led_pin, is_4080_set ? HIGH : LOW);

    // Setup callbacks to write state to EEPROM and toggle LED indicators
    this->c128_capslock_lock_key.set_toggle_callback(capslock_toggled);
    this->c128_4080_lock_key.set_toggle_callback(forty_eighty_toggled);

    _mt8812->begin();
    USB_C128D::begin();

    usb_host.begin();
    keyboard.attachRawPress(raw_press);
    keyboard.attachRawRelease(raw_release);
}

bool Teensy_USB_C128D::is_first_startup() {
    return EEPROM.read(Teensy_USB_C128D::eeprom_first_startup_address);
}

// allow the USB host to read the keyboard and perform callbacks before
// recalculating output
void Teensy_USB_C128D::task() {
    usb_host.Task();
    USB_C128D::task();
}

OutputShiftRegister shift_register = OutputShiftRegister(
    Teensy_USB_C128D::shift_register_data,
    Teensy_USB_C128D::shift_register_clock,
    Teensy_USB_C128D::shift_register_latch
);

MT8812ShiftRegister mt8812 = MT8812ShiftRegister(
    &shift_register,
    Teensy_USB_C128D::mt8812_strobe_pin,
    Teensy_USB_C128D::mt8812_reset_pin
);

Teensy_USB_C128D implementation = Teensy_USB_C128D(&mt8812);

