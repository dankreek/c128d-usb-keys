#include <unity.h>
#include <cstring>
#include <key_mapping.hpp>
#include <usb_c128d.hpp>


class MockUSB_C128D : public USB_C128D {
    public:
        bool mock_usb_capslock_set = false;
        bool mock_usb_numlock_set = false;
        PinsState pins_set;

    protected:
        bool is_usb_capslock() { return mock_usb_capslock_set; }
        bool is_usb_numlock() { return mock_usb_numlock_set; }

        void set_switch(uint8_t row, uint8_t column, bool is_closed) {
            pins_set.rows[row].cols[column] = is_closed;
        }

        void set_special_key(SpecialKey key, bool is_closed) {
            switch (key) {
                case restore:
                    pins_set.special.restore = is_closed;
                    break;
                case forty_eighty:
                    pins_set.special.forty_eighty = is_closed;
                    break;
                case caps_lock:
                    pins_set.special.caps_lock = is_closed;
                    break;
            }
        }

        void reset_output_matrix() { pins_set.reset(); }
};


PinsState expected_no_keys = {
    {
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false}, 
    },
    {false, false, false}
};

PinsState expected_a_switches = {
    {
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, true,  false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false}, 
    },
    {false, false, false}
};

PinsState expected_b_switches = {
    {
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, true, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false}, 
    },
    {false, false, false}
};

PinsState expected_left_shift = {
    {
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false, false, false, false},
    {false, true, false, false, false, false, false, false, false, false, false}, 
    },
    {false, false, false}
};


void _combine_pins(PinsState &state_a, PinsState &state_b, PinsState &output) {
    for (int row=0; row < 8; row++) {
        for (int col=0; col < 11; col++) {
            output.rows[row].cols[col] = state_a.rows[row].cols[col] | state_b.rows[row].cols[col];
        }
    }

    output.special.caps_lock = state_a.special.caps_lock | state_b.special.caps_lock;
    output.special.forty_eighty = state_a.special.forty_eighty | state_b.special.forty_eighty;
    output.special.restore = state_a.special.restore | state_b.special.restore;
}


void test_no_keys_pressed() {
    MockUSB_C128D usb_c128d;  
    usb_c128d.begin();
    usb_c128d.task();

    TEST_ASSERT_EQUAL_MEMORY(&expected_no_keys, &usb_c128d.pins_set, sizeof(PinsState));
}


void test_single_key_press() {
    MockUSB_C128D usb_c128d;
    PinsState received;

    usb_c128d.begin();
    usb_c128d.usb_key_down(USB_KEY_A);
    usb_c128d.task();

    TEST_ASSERT_EQUAL_MEMORY(&expected_a_switches, &usb_c128d.pins_set, sizeof(PinsState));
}


void test_key_down_and_key_up() {
    MockUSB_C128D usb_c128d;
    usb_c128d.begin();

    usb_c128d.usb_key_down(USB_KEY_A);
    usb_c128d.task();
    TEST_ASSERT_EQUAL_MEMORY(&expected_a_switches, &usb_c128d.pins_set, sizeof(PinsState));

    usb_c128d.usb_key_up(USB_KEY_A);
    usb_c128d.task();
    TEST_ASSERT_EQUAL_MEMORY(&expected_no_keys, &usb_c128d.pins_set, sizeof(PinsState));
}


void test_multiple_keys_pressed() {
    MockUSB_C128D usb_c128d;
    PinsState expected;
    _combine_pins(expected_a_switches, expected_b_switches, expected);

    usb_c128d.begin();
    usb_c128d.usb_key_down(USB_KEY_A);
    usb_c128d.usb_key_down(USB_KEY_B);
    usb_c128d.task();

    TEST_ASSERT_EQUAL_MEMORY(&expected, &usb_c128d.pins_set, sizeof(PinsState));
}


void test_usb_capslock_on() {
    MockUSB_C128D usb_c128d;
    PinsState expected;
    _combine_pins(expected_left_shift, expected_a_switches, expected);

    usb_c128d.begin();
    usb_c128d.mock_usb_capslock_set = true;
    usb_c128d.usb_key_down(USB_KEY_A);
    usb_c128d.task();
    TEST_ASSERT_EQUAL_MEMORY(&expected, &usb_c128d.pins_set, sizeof(PinsState));
}


void test_usb_numlock() {
    MockUSB_C128D usb_c128d;
    PinsState expected_kp_4 = {
        {
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, true, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false}, 
        },
        {false, false, false}
    };

    PinsState expected_top_cursor_left = {
        {
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, true},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false}, 
        },
        {false, false, false}
    };

    usb_c128d.begin();
    usb_c128d.mock_usb_numlock_set = true;
    usb_c128d.usb_key_down(USB_KEY_KP_4);
    usb_c128d.task();
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_kp_4, &usb_c128d.pins_set, sizeof(PinsState),
        "Should send numeric key if numlock is on"
    );
    usb_c128d.usb_key_up(USB_KEY_KP_4);

    usb_c128d.mock_usb_numlock_set = false;
    usb_c128d.usb_key_down(USB_KEY_KP_4);
    usb_c128d.task();
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_top_cursor_left, &usb_c128d.pins_set, sizeof(PinsState),
        "Should send arrow key if numlock is off"
    );
}

void test_4080_lock_key() {
    MockUSB_C128D usb_c128d;
    PinsState expected_4080_lock = {
        {
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false}, 
        },
        {false, true, false}
    };

    usb_c128d.begin();
    usb_c128d.usb_key_down(usb_c128d.c128_4080_lock_key.usb_key_code());
    usb_c128d.task();

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_4080_lock, &usb_c128d.pins_set, sizeof(PinsState),
        "After F7 key down 40/80 should be locked"
    );

    TEST_ASSERT_TRUE_MESSAGE(
        usb_c128d.c128_4080_lock_key.is_on(),
        "The 40/80 LockKey toggle is set to true"
    );

    usb_c128d.usb_key_up(usb_c128d.c128_4080_lock_key.usb_key_code());
    usb_c128d.task();

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_4080_lock, &usb_c128d.pins_set, sizeof(PinsState),
        "After F7 key up 40/80 should still be locked"
    );

    usb_c128d.usb_key_down(usb_c128d.c128_4080_lock_key.usb_key_code());
    usb_c128d.task();

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_no_keys, &usb_c128d.pins_set, sizeof(PinsState),
        "After second F7 key down 40/80 should no longer be locked"
    );

    TEST_ASSERT_FALSE_MESSAGE(
        usb_c128d.c128_4080_lock_key.is_on(),
        "The 40/80 LockKey toggle is set to false"
    );

    usb_c128d.usb_key_up(usb_c128d.c128_4080_lock_key.usb_key_code());
    usb_c128d.task();

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_no_keys, &usb_c128d.pins_set, sizeof(PinsState),
        "After second F7 key up 40/80 should still no longer be locked"
    );
}

void test_cursor_keys() {
    MockUSB_C128D usb_c128d;
    PinsState expected_cursor_right = {
        {
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {true, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false}, 
        },
        {false, false, false}
    };

    PinsState expected_cursor_down = {
        {
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {true, false, false, false, false, false, false, false, false, false, false}, 
        },
        {false, false, false}
    };

    PinsState expected_right_shift = {
        {
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, true, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false},
        {false, false, false, false, false, false, false, false, false, false, false}, 
        },
        {false, false, false}
    };

    usb_c128d.begin();
    usb_c128d.usb_key_down(USB_KEY_RIGHT);
    usb_c128d.task();
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_cursor_right, &usb_c128d.pins_set, sizeof(PinsState),
        "USB right cursor should send right cursor"
    );
    usb_c128d.usb_key_up(USB_KEY_RIGHT);

    PinsState expected_shift_cursor;
    _combine_pins(expected_cursor_right, expected_right_shift, expected_shift_cursor);

    usb_c128d.usb_key_down(USB_KEY_LEFT);
    usb_c128d.task();
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_shift_cursor, &usb_c128d.pins_set, sizeof(PinsState),
        "USB left cursor should send shift-right cursor"
    );
    usb_c128d.usb_key_up(USB_KEY_LEFT);

    usb_c128d.usb_key_down(USB_KEY_DOWN);
    usb_c128d.task();
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_cursor_down, &usb_c128d.pins_set, sizeof(PinsState),
        "USB down cursor should send down cursor"
    );
    usb_c128d.usb_key_up(USB_KEY_DOWN);

    _combine_pins(expected_cursor_down, expected_right_shift, expected_shift_cursor);
    usb_c128d.usb_key_down(USB_KEY_UP);
    usb_c128d.task();
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_shift_cursor, &usb_c128d.pins_set, sizeof(PinsState),
        "USB up cursor should send shift-down cursor"
    );
}


void run_usb_c128d_tests() {
    RUN_TEST(test_no_keys_pressed);
    RUN_TEST(test_single_key_press);
    RUN_TEST(test_key_down_and_key_up);
    RUN_TEST(test_multiple_keys_pressed);
    RUN_TEST(test_usb_capslock_on);
    RUN_TEST(test_usb_numlock);
    RUN_TEST(test_4080_lock_key);
    RUN_TEST(test_cursor_keys);
}
