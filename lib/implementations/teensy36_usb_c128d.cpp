#include <Arduino.h>
#include <USBHost_t36.h>
#include "teensy36_usb_c128d.hpp"
#include <mt_8812_shift_register.hpp>

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


bool Teensy36_USB_C128D::is_usb_capslock() { return keyboard.capsLock(); }
bool Teensy36_USB_C128D::is_usb_numlock() { return keyboard.numLock(); }


void capslock_toggled(bool is_set) {
    digitalWrite(Teensy36_USB_C128D::c128_caps_lock_led_pin, is_set);
}


void forty_eighty_toggled(bool is_set) {
    digitalWrite(Teensy36_USB_C128D::c128_4080_lock_led_pin, is_set);
}


void Teensy36_USB_C128D::begin() {
    // Setup special keys
    pinMode(restore_key_pin, OUTPUT);
    pinMode(forty_eighty_key_pin, OUTPUT);
    pinMode(capslock_key_pin, OUTPUT);
    digitalWrite(restore_key_pin, HIGH);
    digitalWrite(forty_eighty_key_pin, HIGH);
    digitalWrite(capslock_key_pin, HIGH);

    pinMode(c128_caps_lock_led_pin, OUTPUT);
    digitalWrite(c128_caps_lock_led_pin, LOW);

    pinMode(c128_4080_lock_led_pin, OUTPUT);
    digitalWrite(c128_4080_lock_led_pin, LOW);

    // TODO: Add storage of lock state to EEPROM
    this->c128_capslock_lock_key.set_toggle_callback(capslock_toggled);
    this->c128_4080_lock_key.set_toggle_callback(forty_eighty_toggled);

    _mt8812->begin();
    USB_C128D::begin();

    usb_host.begin();
    keyboard.attachRawPress(raw_press);
    keyboard.attachRawRelease(raw_release);
}


// allow the USB host to read the keyboard and perform callbacks before
// recalculating output
void Teensy36_USB_C128D::task() {
    usb_host.Task();
    USB_C128D::task();
}

OutputShiftRegister shift_register = OutputShiftRegister(
    Teensy36_USB_C128D::shift_register_data,
    Teensy36_USB_C128D::shift_register_clock,
    Teensy36_USB_C128D::shift_register_latch
);

MT8812ShiftRegister mt8812 = MT8812ShiftRegister(
    &shift_register,
    Teensy36_USB_C128D::mt8812_strobe_pin
);

Teensy36_USB_C128D implementation = Teensy36_USB_C128D(&mt8812);

