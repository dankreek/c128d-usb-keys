#include <unity.h>
#include <cstring>
#include <key_mapping.hpp>
#include <usb_c128d.hpp>

PinsRow unset_columns = {
    {false, false, false, false, false, false, false, false},
    false, false, false
};

SpecialKeys unset_sepcial_keys = { false, false, false };


class MockUSB_C128D : public USB_C128D {
    public:
        bool mock_usb_capslock_set = false;
        bool mock_usb_numlock_set = false;
        SelectedRow mock_selected_row = SelectedRow::none;

        PinsRow cols_that_were_set;
        SpecialKeys specials_that_were_set;

    void exercise_full_rows_scan(PinsState& output_pins_state) {
        mock_selected_row = USB_C128D::SelectedRow::none;
        set_output_cols();

        mock_selected_row = USB_C128D::SelectedRow::row0;
        set_output_cols();
        memcpy(&output_pins_state.rows[0], &cols_that_were_set, sizeof(PinsRow));
        memcpy(&output_pins_state.special, &specials_that_were_set, sizeof(SpecialKeys));

        mock_selected_row = USB_C128D::SelectedRow::row1;
        set_output_cols();
        memcpy(&output_pins_state.rows[1], &cols_that_were_set, sizeof(PinsRow));

        mock_selected_row = USB_C128D::SelectedRow::row2;
        set_output_cols();
        memcpy(&output_pins_state.rows[2], &cols_that_were_set, sizeof(PinsRow));

        mock_selected_row = USB_C128D::SelectedRow::row3;
        set_output_cols();
        memcpy(&output_pins_state.rows[3], &cols_that_were_set, sizeof(PinsRow));

        mock_selected_row = USB_C128D::SelectedRow::row4;
        set_output_cols();
        memcpy(&output_pins_state.rows[4], &cols_that_were_set, sizeof(PinsRow));

        mock_selected_row = USB_C128D::SelectedRow::row5;
        set_output_cols();
        memcpy(&output_pins_state.rows[5], &cols_that_were_set, sizeof(PinsRow));

        mock_selected_row = USB_C128D::SelectedRow::row6;
        set_output_cols();
        memcpy(&output_pins_state.rows[6], &cols_that_were_set, sizeof(PinsRow));

        mock_selected_row = USB_C128D::SelectedRow::row7;
        set_output_cols();
        memcpy(&output_pins_state.rows[7], &cols_that_were_set, sizeof(PinsRow));
    }

    protected:
        bool is_usb_capslock() { return mock_usb_capslock_set; }
        bool is_usb_numlock() { return mock_usb_numlock_set; }
        SelectedRow selected_row() { return mock_selected_row; }

        void set_cols(PinsRow& selected_cols) {
            memcpy(&cols_that_were_set, &selected_cols, sizeof(PinsRow));
        }

        void set_special_pins(SpecialKeys& selected_special_keys) {
            memcpy(&specials_that_were_set, &selected_special_keys, sizeof(SpecialKeys));
        }
};


PinsState expected_no_keys = {
    {
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false}, 
    },
    {false, false, false}
};

PinsState expected_a_pins = {
    {
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, true,  false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false}, 
    },
    {false, false, false}
};

PinsState expected_b_pins = {
    {
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, true, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false}, 
    },
    {false, false, false}
};

PinsState expected_left_shift = {
    {
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, false, false, false, false, false, false, false}, false, false, false},
    {{false, true, false, false, false, false, false, false}, false, false, false}, 
    },
    {false, false, false}
};


void _combine_pins(PinsState &state_a, PinsState &state_b, PinsState &output) {
    for (int row=0; row < 8; row++) {
        for (int col=0; col < 8; col++) {
            output.rows[row].cols[col] = state_a.rows[row].cols[col] | state_b.rows[row].cols[col];
        }

        output.rows[row].k0 = state_a.rows[row].k0 | state_b.rows[row].k0;
        output.rows[row].k1 = state_a.rows[row].k1 | state_b.rows[row].k1;
        output.rows[row].k2 = state_a.rows[row].k2 | state_b.rows[row].k2;
    }

    output.special.caps_lock = state_a.special.caps_lock | state_b.special.caps_lock;
    output.special.forty_eighty = state_a.special.forty_eighty | state_b.special.forty_eighty;
    output.special.restore = state_a.special.restore | state_b.special.restore;
}


void test_no_keys_pressed() {
    MockUSB_C128D usb_c128d; 
    PinsState received;

    usb_c128d.exercise_full_rows_scan(received);
    TEST_ASSERT_EQUAL_MEMORY(&expected_no_keys, &received, sizeof(PinsState));
}


void test_single_key_press() {
    MockUSB_C128D usb_c128d;
    PinsState received;

    usb_c128d.usb_key_down(USB_KEY_A);
    usb_c128d.exercise_full_rows_scan(received);

    TEST_ASSERT_EQUAL_MEMORY(&expected_a_pins, &received, sizeof(PinsState));
}


void test_key_down_and_key_up() {
    MockUSB_C128D usb_c128d;
    PinsState received;

    usb_c128d.usb_key_down(USB_KEY_A);
    usb_c128d.exercise_full_rows_scan(received);
    TEST_ASSERT_EQUAL_MEMORY(&expected_a_pins, &received, sizeof(PinsState));

    usb_c128d.usb_key_up(USB_KEY_A);
    usb_c128d.exercise_full_rows_scan(received);
    TEST_ASSERT_EQUAL_MEMORY(&expected_no_keys, &received, sizeof(PinsState));
}


void test_multiple_keys_pressed() {
    MockUSB_C128D usb_c128d;
    PinsState expected;
    PinsState received;
    _combine_pins(expected_a_pins, expected_b_pins, expected);

    usb_c128d.usb_key_down(USB_KEY_A);
    usb_c128d.usb_key_down(USB_KEY_B);
    usb_c128d.exercise_full_rows_scan(received);

    TEST_ASSERT_EQUAL_MEMORY(&expected, &received, sizeof(PinsState));
}


void test_usb_capslock_on() {
    MockUSB_C128D usb_c128d;
    PinsState received;

    usb_c128d.mock_usb_capslock_set = true;
    usb_c128d.exercise_full_rows_scan(received);

    TEST_ASSERT_EQUAL_MEMORY(&expected_left_shift, &received, sizeof(PinsState));
}


void test_usb_numlock() {
    MockUSB_C128D usb_c128d;
    PinsState received;
    PinsState expected_kp_4 = {
        {
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, true, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false}, 
        },
        {false, false, false}
    };

    PinsState expected_top_cursor_left = {
        {
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, true},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false}, 
        },
        {false, false, false}
    };

    usb_c128d.mock_usb_numlock_set = true;
    usb_c128d.usb_key_down(USB_KEY_KP_4);
    usb_c128d.exercise_full_rows_scan(received);
    TEST_ASSERT_EQUAL_MEMORY(&expected_kp_4, &received, sizeof(PinsState));

    usb_c128d.mock_usb_numlock_set = false;
    usb_c128d.exercise_full_rows_scan(received);
    TEST_ASSERT_EQUAL_MEMORY(&expected_top_cursor_left, &received, sizeof(PinsState));
}

void test_4080_lock_key() {
    MockUSB_C128D usb_c128d;
    PinsState received;
    PinsState expected_4080_lock = {
        {
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false}, 
        },
        {false, true, false}
    };

    usb_c128d.usb_key_down(usb_c128d.c128_4080_lock_key.usb_key_code());
    usb_c128d.exercise_full_rows_scan(received);

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_4080_lock, &received, sizeof(PinsState),
        "After F7 key down 40/80 should be locked"
    );

    TEST_ASSERT_TRUE_MESSAGE(
        usb_c128d.c128_4080_lock_key.is_on(),
        "The 40/80 LockKey toggle is set to true"
    )

    usb_c128d.usb_key_up(usb_c128d.c128_4080_lock_key.usb_key_code());
    usb_c128d.exercise_full_rows_scan(received);

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_4080_lock, &received, sizeof(PinsState),
        "After F7 key up 40/80 should still be locked"
    );

    usb_c128d.usb_key_down(usb_c128d.c128_4080_lock_key.usb_key_code());
    usb_c128d.exercise_full_rows_scan(received);

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_no_keys, &received, sizeof(PinsState),
        "After second F7 key down 40/80 should no longer be locked"
    );

    TEST_ASSERT_FALSE_MESSAGE(
        usb_c128d.c128_4080_lock_key.is_on(),
        "The 40/80 LockKey toggle is set to false"
    )

    usb_c128d.usb_key_up(usb_c128d.c128_4080_lock_key.usb_key_code());
    usb_c128d.exercise_full_rows_scan(received);

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_no_keys, &received, sizeof(PinsState),
        "After second F7 key up 40/80 should still no longer be locked"
    );
}

void test_cursor_keys() {
    MockUSB_C128D usb_c128d;
    PinsState received;
    PinsState expected_cursor_right = {
        {
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{true, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false}, 
        },
        {false, false, false}
    };

    PinsState expected_cursor_down = {
        {
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{true, false, false, false, false, false, false, false}, false, false, false}, 
        },
        {false, false, false}
    };

    PinsState expected_right_shift = {
        {
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, true, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false},
        {{false, false, false, false, false, false, false, false}, false, false, false}, 
        },
        {false, false, false}
    };

    usb_c128d.usb_key_down(USB_KEY_RIGHT);
    usb_c128d.exercise_full_rows_scan(received);
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_cursor_right, &received, sizeof(PinsState),
        "USB right cursor should send right cursor"
    );
    usb_c128d.usb_key_up(USB_KEY_RIGHT);

    PinsState expected_shift_cursor;
    _combine_pins(expected_cursor_right, expected_right_shift, expected_shift_cursor);

    usb_c128d.usb_key_down(USB_KEY_LEFT);
    usb_c128d.exercise_full_rows_scan(received);
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_shift_cursor, &received, sizeof(PinsState),
        "USB left cursor should send shift-right cursor"
    );
    usb_c128d.usb_key_up(USB_KEY_LEFT);

    usb_c128d.usb_key_down(USB_KEY_DOWN);
    usb_c128d.exercise_full_rows_scan(received);
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_cursor_down, &received, sizeof(PinsState),
        "USB down cursor should send down cursor"
    );
    usb_c128d.usb_key_up(USB_KEY_DOWN);

    _combine_pins(expected_cursor_down, expected_right_shift, expected_shift_cursor);
    usb_c128d.usb_key_down(USB_KEY_UP);
    usb_c128d.exercise_full_rows_scan(received);
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_shift_cursor, &received, sizeof(PinsState),
        "USB up cursor should send shift-down cursor"
    );
    usb_c128d.usb_key_up(USB_KEY_UP);
}


/** Ensure that the output does not update until the C128D starts a new scan of the matrix */
void test_keys_not_updated_during_scan() {
    MockUSB_C128D usb_c128d;
    PinsState received;

    usb_c128d.mock_selected_row = USB_C128D::SelectedRow::none;
    usb_c128d.set_output_cols();
    usb_c128d.mock_selected_row = USB_C128D::SelectedRow::row0;
    usb_c128d.set_output_cols();

    // Press a key in the middle of a keyboard scan
    usb_c128d.usb_key_down(USB_KEY_LSHIFT);

    usb_c128d.mock_selected_row = USB_C128D::SelectedRow::row1;
    usb_c128d.set_output_cols();
    usb_c128d.mock_selected_row = USB_C128D::SelectedRow::row2;
    usb_c128d.set_output_cols();
    usb_c128d.mock_selected_row = USB_C128D::SelectedRow::row3;
    usb_c128d.set_output_cols();
    usb_c128d.mock_selected_row = USB_C128D::SelectedRow::row4;
    usb_c128d.set_output_cols();
    usb_c128d.mock_selected_row = USB_C128D::SelectedRow::row5;
    usb_c128d.set_output_cols();
    usb_c128d.mock_selected_row = USB_C128D::SelectedRow::row6;
    usb_c128d.set_output_cols();
    usb_c128d.mock_selected_row = USB_C128D::SelectedRow::row7;
    usb_c128d.set_output_cols();

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &unset_columns, &usb_c128d.cols_that_were_set, sizeof(PinsRow),
        "Nothing should be set on row7, where left-shift would be set"
    );

    usb_c128d.exercise_full_rows_scan(received);

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &expected_left_shift, &received, sizeof(PinsState),
        "On the next scan, left-shift should be set"
    );
}

void test_nothing_sent_while_not_scanning() {
    MockUSB_C128D usb_c128d;
    PinsState received;

    usb_c128d.usb_key_down(USB_KEY_3);
    usb_c128d.usb_key_down(USB_KEY_W);
    usb_c128d.usb_key_down(USB_KEY_A);
    usb_c128d.usb_key_down(USB_KEY_4);
    usb_c128d.usb_key_down(USB_KEY_Z);
    usb_c128d.usb_key_down(USB_KEY_S);
    usb_c128d.usb_key_down(USB_KEY_E);
    usb_c128d.usb_key_down(USB_KEY_LSHIFT);
    usb_c128d.exercise_full_rows_scan(received);

    usb_c128d.mock_selected_row = USB_C128D::SelectedRow::none;
    usb_c128d.set_output_cols();

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(
        &unset_columns, &usb_c128d.cols_that_were_set, sizeof(PinsRow),
        "When no row is selected, no columns should be set"
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
    RUN_TEST(test_keys_not_updated_during_scan);
    RUN_TEST(test_nothing_sent_while_not_scanning);
}
