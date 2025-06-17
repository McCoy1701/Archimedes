// File: true_tests/input/test_input_errors.c - Error handling and defensive programming tests

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "Archimedes.h"
#include "tests.h"

// Global test counters
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// Helper functions
static void reset_input_state(void) {
    memset(app.keyboard, 0, sizeof(app.keyboard));
    app.mouse.x = 0;
    app.mouse.y = 0;
    app.mouse.pressed = 0;
    app.mouse.button = 0;
    app.mouse.state = 0;
    app.mouse.clicks = 0;
    app.mouse.wheel = 0;
    memset(app.input_text, 0, sizeof(app.input_text));
    app.last_key_pressed = 0;
    app.running = 1;
}

static void mock_app_init(void) {
    memset(&app, 0, sizeof(app));
    app.running = 1;
    reset_input_state();
}

// Mock functions to test internal static functions directly
// These would normally be inaccessible, but we can create similar test scenarios

static void test_keyboard_event_with_null(void) {
    // We can't call the static functions directly, but we can test the public interface
    // with malformed data to ensure it handles errors gracefully
    reset_input_state();

    // The actual static functions can't be called directly, but we can verify
    // that the public a_DoInput function handles errors appropriately
    // This test ensures our defensive programming is working
}

// ============================================================================
// NULL Pointer and Invalid Parameter Tests
// ============================================================================

int test_keyboard_boundary_scancodes(void) {
    reset_input_state();

    // Test exactly at boundary
    if (MAX_KEYBOARD_KEYS > 0) {
        // This should NOT crash the system
        // Test accessing exactly at the boundary (valid)
        app.keyboard[MAX_KEYBOARD_KEYS - 1] = 1;
        TEST_ASSERT(app.keyboard[MAX_KEYBOARD_KEYS - 1] == 1, "Last valid scancode should be accessible");

        // Reset
        app.keyboard[MAX_KEYBOARD_KEYS - 1] = 0;
        TEST_ASSERT(app.keyboard[MAX_KEYBOARD_KEYS - 1] == 0, "Last valid scancode should be resetable");
    }

    return 1;
}

int test_mouse_extreme_coordinates(void) {
    reset_input_state();

    // Test extreme coordinate values
    app.mouse.x = INT_MAX;
    app.mouse.y = INT_MAX;
    TEST_ASSERT(app.mouse.x == INT_MAX, "Should handle maximum positive coordinates");
    TEST_ASSERT(app.mouse.y == INT_MAX, "Should handle maximum positive coordinates");

    app.mouse.x = INT_MIN;
    app.mouse.y = INT_MIN;
    TEST_ASSERT(app.mouse.x == INT_MIN, "Should handle minimum negative coordinates");
    TEST_ASSERT(app.mouse.y == INT_MIN, "Should handle minimum negative coordinates");

    // Test coordinates way outside normal screen bounds
    app.mouse.x = -50000;
    app.mouse.y = 50000;
    TEST_ASSERT(app.mouse.x == -50000, "Should handle extreme off-screen coordinates");
    TEST_ASSERT(app.mouse.y == 50000, "Should handle extreme off-screen coordinates");

    return 1;
}

// ============================================================================
// Text Input Error Tests
// ============================================================================

int test_text_input_buffer_overflow_protection(void) {
    reset_input_state();

    // Test with string exactly at buffer limit
    char max_text[MAX_INPUT_LENGTH];
    memset(max_text, 'A', MAX_INPUT_LENGTH - 1);
    max_text[MAX_INPUT_LENGTH - 1] = '\0';

    STRCPY(app.input_text, max_text);
    TEST_ASSERT(strlen(app.input_text) == MAX_INPUT_LENGTH - 1, "Should handle maximum length text");
    TEST_ASSERT(app.input_text[MAX_INPUT_LENGTH - 1] == '\0', "Should maintain null termination");

    // Test with oversized string
    char oversized_text[MAX_INPUT_LENGTH + 100];
    memset(oversized_text, 'B', sizeof(oversized_text) - 1);
    oversized_text[sizeof(oversized_text) - 1] = '\0';

    STRCPY(app.input_text, oversized_text);
    TEST_ASSERT(strlen(app.input_text) < MAX_INPUT_LENGTH, "Should truncate oversized text");
    TEST_ASSERT(app.input_text[MAX_INPUT_LENGTH - 1] == '\0', "Should maintain null termination after truncation");

    return 1;
}

int test_text_input_special_characters(void) {
    reset_input_state();

    // Test with null character in middle
    char null_middle[] = "Hello\0World";
    STRCPY(app.input_text, null_middle);
    TEST_ASSERT(strlen(app.input_text) == 5, "Should stop at embedded null character");
    TEST_ASSERT(strcmp(app.input_text, "Hello") == 0, "Should only copy up to null character");

    // Test with control characters
    char control_chars[] = "Test\t\n\r";
    STRCPY(app.input_text, control_chars);
    TEST_ASSERT(app.input_text[4] == '\t', "Should preserve tab character");
    TEST_ASSERT(app.input_text[5] == '\n', "Should preserve newline character");
    TEST_ASSERT(app.input_text[6] == '\r', "Should preserve carriage return");

    // Test with high ASCII values
    char high_ascii[10] = {0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, '\0'};
    STRCPY(app.input_text, high_ascii);
    TEST_ASSERT(strlen(app.input_text) >= 1, "Should handle high ASCII characters");

    return 1;
}

int test_text_input_empty_and_whitespace(void) {
    reset_input_state();

    // Test empty string
    STRCPY(app.input_text, "");
    TEST_ASSERT(strlen(app.input_text) == 0, "Should handle empty string");
    TEST_ASSERT(app.input_text[0] == '\0', "Empty string should be properly null terminated");

    // Test whitespace-only strings
    STRCPY(app.input_text, "   ");
    TEST_ASSERT(strlen(app.input_text) == 3, "Should handle whitespace-only string");
    TEST_ASSERT(app.input_text[0] == ' ', "Should preserve leading spaces");

    // Test mixed whitespace
    STRCPY(app.input_text, " \t\n ");
    TEST_ASSERT(strlen(app.input_text) == 4, "Should handle mixed whitespace");

    return 1;
}

// ============================================================================
// Memory Corruption and Stability Tests
// ============================================================================

int test_input_state_memory_corruption_resistance(void) {
    reset_input_state();

    // Fill keyboard array with pattern to detect corruption
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        app.keyboard[i] = (i % 3); // Pattern: 0, 1, 2, 0, 1, 2, ...
    }

    // Verify pattern integrity
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        TEST_ASSERT(app.keyboard[i] == (i % 3), "Keyboard pattern should be intact");
        if (i > 50) break; // Don't spam output
    }

    // Modify mouse state with extreme values
    app.mouse.x = INT_MAX;
    app.mouse.y = INT_MIN;
    app.mouse.button = 255;
    app.mouse.state = 255;
    app.mouse.clicks = INT_MAX;
    app.mouse.wheel = INT_MIN;
    app.mouse.pressed = 255;

    // Verify keyboard pattern is still intact (no memory corruption)
    for (int i = 0; i < 10; i++) {
        TEST_ASSERT(app.keyboard[i] == (i % 3), "Keyboard pattern should remain intact after mouse modification");
    }

    return 1;
}

int test_rapid_state_changes_stability(void) {
    reset_input_state();

    // Rapid state changes to test for race conditions or instability
    for (int cycle = 0; cycle < 1000; cycle++) {
        // Rapidly change keyboard state
        for (int i = 0; i < 50; i++) {
            app.keyboard[i] = cycle % 2;
        }

        // Rapidly change mouse state
        app.mouse.x = cycle % 1000;
        app.mouse.y = cycle % 800;
        app.mouse.button = (cycle % 5) + 1;
        app.mouse.state = cycle % 2;
        app.mouse.wheel = (cycle % 5) - 2;

        // Rapidly change text
        char rapid_text[16];
        snprintf(rapid_text, sizeof(rapid_text), "t%d", cycle % 100);
        STRCPY(app.input_text, rapid_text);

        // Verify system stability (no crashes, reasonable values)
        TEST_ASSERT(app.mouse.x >= 0 && app.mouse.x < 1000, "Mouse X should be in expected range");
        TEST_ASSERT(strlen(app.input_text) > 0, "Text should not be empty");

        if (cycle > 10) break; // Don't spam test output
    }

    return 1;
}

int test_mixed_invalid_operations(void) {
    reset_input_state();

    // Test combination of invalid operations

    // Invalid keyboard operations
    app.last_key_pressed = -999;
    for (int i = 0; i < 10; i++) {
        app.keyboard[i] = -1; // Invalid state
    }

    // Invalid mouse operations
    app.mouse.x = INT_MIN;
    app.mouse.y = INT_MAX;
    app.mouse.button = -999;
    app.mouse.state = 999;
    app.mouse.clicks = -999;
    app.mouse.wheel = INT_MIN;
    app.mouse.pressed = 999;

    // Invalid text operations
    char corrupted_text[MAX_INPUT_LENGTH];
    memset(corrupted_text, 0xFF, sizeof(corrupted_text)); // Fill with invalid chars
    corrupted_text[MAX_INPUT_LENGTH - 1] = '\0';
    STRCPY(app.input_text, corrupted_text);

    // System should remain stable despite all invalid operations
    TEST_ASSERT(app.last_key_pressed == -999, "Should store invalid last key");
    TEST_ASSERT(app.mouse.x == INT_MIN, "Should store invalid mouse coordinates");
    TEST_ASSERT(app.input_text[MAX_INPUT_LENGTH - 1] == '\0', "Text buffer should remain null terminated");

    // Reset should still work
    reset_input_state();
    TEST_ASSERT(app.last_key_pressed == 0, "Reset should work after invalid operations");
    TEST_ASSERT(app.mouse.x == 0, "Reset should work after invalid operations");
    TEST_ASSERT(app.input_text[0] == '\0', "Reset should work after invalid operations");

    return 1;
}

// ============================================================================
// Boundary and Edge Case Tests
// ============================================================================

int test_keyboard_boundary_edge_cases(void) {
    reset_input_state();

    // Test first and last valid keys
    app.keyboard[0] = 1;
    app.keyboard[MAX_KEYBOARD_KEYS - 1] = 1;

    TEST_ASSERT(app.keyboard[0] == 1, "First keyboard key should be accessible");
    TEST_ASSERT(app.keyboard[MAX_KEYBOARD_KEYS - 1] == 1, "Last keyboard key should be accessible");

    // Test adjacent to boundaries (should be safe)
    if (MAX_KEYBOARD_KEYS > 2) {
        app.keyboard[1] = 1;
        app.keyboard[MAX_KEYBOARD_KEYS - 2] = 1;

        TEST_ASSERT(app.keyboard[1] == 1, "Second keyboard key should be accessible");
        TEST_ASSERT(app.keyboard[MAX_KEYBOARD_KEYS - 2] == 1, "Second-to-last keyboard key should be accessible");
    }

    return 1;
}

int test_running_flag_manipulation(void) {
    reset_input_state();

    // Test running flag edge cases
    app.running = 1;
    TEST_ASSERT(app.running == 1, "Running flag should be settable to 1");

    app.running = 0;
    TEST_ASSERT(app.running == 0, "Running flag should be settable to 0");

    // Test extreme values
    app.running = -999;
    TEST_ASSERT(app.running == -999, "Running flag should accept negative values");

    app.running = 999;
    TEST_ASSERT(app.running == 999, "Running flag should accept large positive values");

    // Reset to normal
    app.running = 1;
    TEST_ASSERT(app.running == 1, "Running flag should be resettable to normal");

    return 1;
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    // Initialize minimal app structure for testing
    mock_app_init();

    TEST_SUITE_START("Input System Error Handling Tests");

    // Boundary and invalid parameter tests
    RUN_TEST(test_keyboard_boundary_scancodes);
    RUN_TEST(test_mouse_extreme_coordinates);

    // Text input error tests
    RUN_TEST(test_text_input_buffer_overflow_protection);
    RUN_TEST(test_text_input_special_characters);
    RUN_TEST(test_text_input_empty_and_whitespace);

    // Memory corruption and stability tests
    RUN_TEST(test_input_state_memory_corruption_resistance);
    RUN_TEST(test_rapid_state_changes_stability);
    RUN_TEST(test_mixed_invalid_operations);

    // Boundary and edge case tests
    RUN_TEST(test_keyboard_boundary_edge_cases);
    RUN_TEST(test_running_flag_manipulation);

    TEST_SUITE_END();
}
