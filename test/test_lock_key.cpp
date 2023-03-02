#include <unity.h>
#include <lock_key.hpp>
#include "key_mapping.hpp"


bool was_callback_called;
bool callback_is_on_state;

void _toggle_callback(bool is_on) {
    was_callback_called = true;
    callback_is_on_state = is_on;
}

void test_set_is_on() {
    LockKey lock_key(USB_KEY_F4);

    TEST_ASSERT_FALSE(lock_key.is_on());
    lock_key.set_is_on(true);
    TEST_ASSERT_TRUE(lock_key.is_on());
}

void test_toggle_on_state() {
    LockKey lock_key(USB_KEY_F4);

    lock_key.set_is_on(true);
    lock_key.toggle_on_state();
    TEST_ASSERT_FALSE(lock_key.is_on());
}


void test_set_toggle_callback() {
    was_callback_called = false;
    callback_is_on_state = false;

    LockKey lock_key(USB_KEY_F4);
    lock_key.set_is_on(true);
    lock_key.set_toggle_callback(_toggle_callback);

    TEST_ASSERT_TRUE(was_callback_called);
    TEST_ASSERT_TRUE_MESSAGE(callback_is_on_state, "is_on should be true");
}


void test_callback_called_on_toggle() {
    LockKey lock_key(USB_KEY_F4);
    lock_key.set_is_on(true);
    lock_key.set_toggle_callback(_toggle_callback);

    was_callback_called = false;
    callback_is_on_state = false;
    lock_key.toggle_on_state();

    TEST_ASSERT_TRUE(was_callback_called);
    TEST_ASSERT_FALSE(callback_is_on_state);
}


void test_callback_called_on_set() {
    LockKey lock_key(USB_KEY_F4);
    lock_key.set_is_on(false);
    lock_key.set_toggle_callback(_toggle_callback);

    was_callback_called = false;
    callback_is_on_state = false;
    lock_key.set_is_on(true);

    TEST_ASSERT_TRUE(was_callback_called);
    TEST_ASSERT_TRUE(callback_is_on_state);
}


void test_callback_not_called_on_same_state() {
    LockKey lock_key(USB_KEY_F4);
    lock_key.set_is_on(true);
    lock_key.set_toggle_callback(_toggle_callback);

    was_callback_called = false;
    callback_is_on_state = false;
    lock_key.set_is_on(true);

    TEST_ASSERT_FALSE(was_callback_called);
}


void run_lock_key_tests() {
    RUN_TEST(test_set_is_on);
    RUN_TEST(test_toggle_on_state);
    RUN_TEST(test_set_toggle_callback);
    RUN_TEST(test_callback_called_on_toggle);
    RUN_TEST(test_callback_called_on_set);
    RUN_TEST(test_callback_not_called_on_same_state);
}
