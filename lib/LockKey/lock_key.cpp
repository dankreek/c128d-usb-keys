#include "lock_key.hpp"

LockKey::LockKey(uint8_t usb_key_code) {
    _usb_key_code = usb_key_code;
    _toggle_callback = nullptr;
    _is_on = false;
}

bool LockKey::is_on() {
    return _is_on;
}

void LockKey::set_is_on(bool is_on) {
    bool should_call_cb = false;
    if (_is_on != is_on) should_call_cb = true;

    _is_on = is_on;

    if (should_call_cb) _call_callback();
}

void LockKey::toggle_on_state() {
    this->_is_on = !this->_is_on;
    _call_callback();
}

uint8_t LockKey::usb_key_code() {
    return this->_usb_key_code;
}

void LockKey::set_toggle_callback(void (*toggle_callback)(bool)) {
    _toggle_callback = toggle_callback;
    _call_callback();
}

void LockKey::_call_callback() {
    if (_toggle_callback != nullptr) _toggle_callback(_is_on);
}