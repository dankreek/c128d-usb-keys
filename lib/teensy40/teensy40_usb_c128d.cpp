#include <Arduino.h>
#include <EEPROM.h>
#include <USBHost_t36.h>
#include <mt_8812_shift_register.hpp>
#include <eeprom_funcs.hpp>
#include "teensy40_usb_c128d.hpp"

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


bool Teensy40_USB_C128D::is_usb_capslock() { return keyboard.capsLock(); }
bool Teensy40_USB_C128D::is_usb_numlock() { return keyboard.numLock(); }


void capslock_toggled(bool is_set) {
    digitalWrite(Teensy40_USB_C128D::c128_caps_lock_led_pin, is_set ? HIGH : LOW);
    write_eeprom_bool(Teensy40_USB_C128D::eeprom_caps_lock_key_address, is_set);
}


void forty_eighty_toggled(bool is_set) {
    digitalWrite(Teensy40_USB_C128D::c128_4080_lock_led_pin, is_set ? HIGH : LOW);
    write_eeprom_bool(Teensy40_USB_C128D::eeprom_40_80_key_address, is_set);
}


void Teensy40_USB_C128D::begin() {
    // Wait for serial output to be ready
    while (!Serial) {};

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
        Serial.println("First startup detected");

        is_capslock_set = false;
        is_4080_set = false;

        write_eeprom_bool(Teensy40_USB_C128D::eeprom_caps_lock_key_address, false);
        write_eeprom_bool(Teensy40_USB_C128D::eeprom_caps_lock_key_address, false);

        // Set EEPROM to recognize the first startup has happened
        write_eeprom_bool(Teensy40_USB_C128D::eeprom_first_startup_address, false);
        EEPROM.write(Teensy40_USB_C128D::eeprom_version_address, 0);
    } else {
        is_capslock_set = read_eeprom_bool(Teensy40_USB_C128D::eeprom_caps_lock_key_address);
        is_4080_set = read_eeprom_bool(Teensy40_USB_C128D::eeprom_40_80_key_address);
    }

    // Set output pins to C128D
    this->set_special_key(caps_lock, false);
    this->set_special_key(forty_eighty, false);

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

bool Teensy40_USB_C128D::is_first_startup() {
    return !read_eeprom_bool(Teensy40_USB_C128D::eeprom_first_startup_address);
}

// allow the USB host to read the keyboard and perform callbacks before
// recalculating output
void Teensy40_USB_C128D::task() {
    usb_host.Task();
    USB_C128D::task();
}

OutputShiftRegister shift_register = OutputShiftRegister(
    Teensy40_USB_C128D::shift_register_data,
    Teensy40_USB_C128D::shift_register_clock,
    Teensy40_USB_C128D::shift_register_latch
);

MT8812ShiftRegister mt8812 = MT8812ShiftRegister(
    &shift_register,
    Teensy40_USB_C128D::mt8812_strobe_pin,
    Teensy40_USB_C128D::mt8812_reset_pin
);

Teensy40_USB_C128D implementation = Teensy40_USB_C128D(&mt8812);

