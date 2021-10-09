#include "lock_key.hpp"

LockKey::LockKey(uint8_t led_pin, uint8_t usb_key_code) {
    this->_led_pin = led_pin;
    this->_usb_key_code = usb_key_code;
    this->_is_on = false;
}

bool LockKey::is_on() {
    return this->_is_on;
}

void LockKey::set_is_on(bool is_on) {
    this->_is_on = is_on;
    this->_set_led_state();
}

void LockKey::toggle_on_state() {
    this->_is_on = !this->_is_on;
    this->_set_led_state();
}

void LockKey::_set_led_state() {
    // TODO: set the LED on/off, save to EEPROM and rename deeze shits
}

uint8_t LockKey::usb_key_code() {
    return this->_usb_key_code;
}