#include <unity.h>

#include "usb_key_buffer.hpp"
#include "key_mapping.hpp"


USBKeyBuffer* key_buffer;


void test_add_key() {
    key_buffer->add(USB_KEY_F);

    key_buffer->for_each([](uint8_t key_code) {
        TEST_ASSERT_EQUAL(USB_KEY_F, key_code);
    });

    TEST_ASSERT_EQUAL(1, key_buffer->count());
}


void test_remove_key() {
    key_buffer->remove(USB_KEY_F);
    TEST_ASSERT_EQUAL(0, key_buffer->count());
}


void test_add_multiple_keys() {
    key_buffer->add(USB_KEY_A);
    key_buffer->add(USB_KEY_B);
    key_buffer->add(USB_KEY_C);

    key_buffer->for_each([](uint8_t key_code) {
        TEST_ASSERT_TRUE(
            (key_code == USB_KEY_A) || 
            (key_code == USB_KEY_B) ||
            (key_code == USB_KEY_C)
        );
    });

    TEST_ASSERT_EQUAL(3, key_buffer->count());
}


void test_remove_single_key_from_middle_of_buffer() {
    key_buffer->remove(USB_KEY_B);

    key_buffer->for_each([](uint8_t key_code) {
        TEST_ASSERT_TRUE(
            (key_code == USB_KEY_A) || 
            (key_code == USB_KEY_C)
        );
    });

    TEST_ASSERT_EQUAL(2, key_buffer->count());
}


void test_remove_nonexistent_key_still_works() {
    key_buffer->remove(USB_KEY_Z);

    key_buffer->for_each([](uint8_t key_code) {
        TEST_ASSERT_TRUE(
            (key_code == USB_KEY_A) || 
            (key_code == USB_KEY_C)
        );
    });

    TEST_ASSERT_EQUAL(2, key_buffer->count());
}


void test_overflowing_buffer_drops_keys() {
    const uint8_t top_key_code = USB_KEY_A + USBKeyBuffer::KEY_BUFFER_SIZE;
    for (int key_code=USB_KEY_A; key_code < top_key_code; key_code++) {
        key_buffer->add(key_code);
    }
    TEST_ASSERT_EQUAL_MESSAGE(
        USBKeyBuffer::KEY_BUFFER_SIZE, key_buffer->count(),
        "Buffer size should be maxed"
    );

    key_buffer->add(top_key_code + 3);
    TEST_ASSERT_EQUAL_MESSAGE(
        USBKeyBuffer::KEY_BUFFER_SIZE, key_buffer->count(),
        "Buffer size should still be at max"
    );

    key_buffer->for_each([top_key_code](uint8_t key_code) {
        TEST_ASSERT_MESSAGE(
            (key_code >= USB_KEY_A) && (key_code <= top_key_code),
            "Overflow characters should be dropped"
        );
    });
}


void run_usb_keyboard_buffer_tests() {
    key_buffer = new USBKeyBuffer;
    RUN_TEST(test_add_key);
    RUN_TEST(test_remove_key);
    RUN_TEST(test_add_multiple_keys);
    RUN_TEST(test_remove_single_key_from_middle_of_buffer);
    RUN_TEST(test_remove_nonexistent_key_still_works);
    delete key_buffer;

    key_buffer = new USBKeyBuffer;
    RUN_TEST(test_overflowing_buffer_drops_keys);
    delete key_buffer;
}
