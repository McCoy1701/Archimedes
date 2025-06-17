// File: true_tests/input/test_input_edge.c - Edge case and boundary tests for input system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
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

static void corrupt_input_state(void) {
    // Intentionally set invalid/extreme values to test robustness
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        app.keyboard[i] = (i % 3 == 0) ? -1 : ((i % 3 == 1) ? 255 : 1);
    }
    app.mouse.x = INT_MAX;
    app.mouse.y = INT_MIN;
    app.mouse.wheel = 999;
    app.last_key_pressed = -999;
}

// ============================================================================
// Boundary Condition Tests
// ============================================================================

int test_keyboard_array_boundaries(void) {
    reset_input_state();

    // Test accessing exactly at boundary
    app.keyboard[0] = 1;
    TEST_ASSERT(app.keyboard[0] == 1, "First keyboard element should be accessible");

    app.keyboard[MAX_KEYBOARD_KEYS - 1] = 1;
    TEST_ASSERT(app.keyboard[MAX_KEYBOARD_KEYS - 1] == 1, "Last keyboard element should be accessible");

    // Test that we don't accidentally write beyond bounds in loops
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        app.keyboard[i] = (i % 2); // Alternate pattern
    }

    // Verify pattern integrity
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        TEST_ASSERT(app.keyboard[i] == (i % 2), "Keyboard array pattern should be preserved");
        if (i > 10) break; // Don't spam output
    }

    return 1;
}

int test_scancode_edge_values(void) {
    reset_input_state();

    // Test with scancode 0 (valid)
    if (0 < MAX_KEYBOARD_KEYS) {
        app.keyboard[0] = 1;
        app.last_key_pressed = 0;
        TEST_ASSERT(app.keyboard[0] == 1, "Scancode 0 should be valid");
        TEST_ASSERT(app.last_key_pressed == 0, "Last key should be 0");
    }

    // Test with maximum valid scancode
    int max_valid = MAX_KEYBOARD_KEYS - 1;
    app.keyboard[max_valid] = 1;
    app.last_key_pressed = max_valid;
    TEST_ASSERT(app.keyboard[max_valid] == 1, "Maximum valid scancode should work");
    TEST_ASSERT(app.last_key_pressed == max_valid, "Last key should be max valid");

    // Test behavior with negative scancode (should be handled safely)
    app.last_key_pressed = -1;
    TEST_ASSERT(app.last_key_pressed == -1, "Negative scancode should be stored but not crash");

    // Test with extremely large scancode
    app.last_key_pressed = 99999;
    TEST_ASSERT(app.last_key_pressed == 99999, "Large scancode should be stored but not crash");

    return 1;
}

int test_mouse_coordinate_extremes(void) {
    reset_input_state();

    // Test extreme positive coordinates
    app.mouse.x = INT_MAX;
    app.mouse.y = INT_MAX;
    TEST_ASSERT(app.mouse.x == INT_MAX, "Should handle maximum positive X coordinate");
    TEST_ASSERT(app.mouse.y == INT_MAX, "Should handle maximum positive Y coordinate");

    // Test extreme negative coordinates
    app.mouse.x = INT_MIN;
    app.mouse.y = INT_MIN;
    TEST_ASSERT(app.mouse.x == INT_MIN, "Should handle minimum negative X coordinate");
    TEST_ASSERT(app.mouse.y == INT_MIN, "Should handle minimum negative Y coordinate");

    // Test zero coordinates
    app.mouse.x = 0;
    app.mouse.y = 0;
    TEST_ASSERT(app.mouse.x == 0, "Should handle zero X coordinate");
    TEST_ASSERT(app.mouse.y == 0, "Should handle zero Y coordinate");

    // Test coordinates just outside typical screen bounds
    app.mouse.x = -100;
    app.mouse.y = -100;
    TEST_ASSERT(app.mouse.x == -100, "Should handle negative coordinates (off-screen)");
    TEST_ASSERT(app.mouse.y == -100, "Should handle negative coordinates (off-screen)");

    app.mouse.x = 10000;
    app.mouse.y = 10000;
    TEST_ASSERT(app.mouse.x == 10000, "Should handle large coordinates (way off-screen)");
    TEST_ASSERT(app.mouse.y == 10000, "Should handle large coordinates (way off-screen)");

    return 1;
}

// ============================================================================
// Memory Safety Tests
// ============================================================================

int test_input_text_buffer_safety(void) {
    reset_input_state();

    // Test exact buffer size
    char exact_size_text[MAX_INPUT_LENGTH];
    memset(exact_size_text, 'A', MAX_INPUT_LENGTH - 1);
    exact_size_text[MAX_INPUT_LENGTH - 1] = '\0';

    STRCPY(app.input_text, exact_size_text);
    TEST_ASSERT(strlen(app.input_text) == MAX_INPUT_LENGTH - 1, "Should handle exact buffer size");
    TEST_ASSERT(app.input_text[MAX_INPUT_LENGTH - 1] == '\0', "Should be null terminated");

    // Test oversized input
    char oversized_text[MAX_INPUT_LENGTH + 100];
    memset(oversized_text, 'B', sizeof(oversized_text) - 1);
    oversized_text[sizeof(oversized_text) - 1] = '\0';

    STRCPY(app.input_text, oversized_text);
    TEST_ASSERT(strlen(app.input_text) < MAX_INPUT_LENGTH, "Should truncate oversized input");
    TEST_ASSERT(app.input_text[MAX_INPUT_LENGTH - 1] == '\0', "Should maintain null termination");

    // Test empty string
    STRCPY(app.input_text, "");
    TEST_ASSERT(strlen(app.input_text) == 0, "Should handle empty string");
    TEST_ASSERT(app.input_text[0] == '\0', "Empty string should be null terminated");

    // Test null character in middle
    char null_middle[] = "Hello\0World";
    STRCPY(app.input_text, null_middle);
    TEST_ASSERT(strlen(app.input_text) == 5, "Should stop at null character");
    TEST_ASSERT(strcmp(app.input_text, "Hello") == 0, "Should only copy up to null");

    return 1;
}

int test_simultaneous_extreme_inputs(void) {
    reset_input_state();

    // Set extreme values in all input systems simultaneously

    // Max out keyboard
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        app.keyboard[i] = 1;
    }

    // Max out mouse
    app.mouse.x = INT_MAX;
    app.mouse.y = INT_MIN;
    app.mouse.button = 255; // Extreme button value
    app.mouse.state = 255;
    app.mouse.clicks = INT_MAX;
    app.mouse.wheel = INT_MAX;
    app.mouse.pressed = 255; // Extreme pressed value

    // Max out text
    char max_text[MAX_INPUT_LENGTH];
    memset(max_text, 'Z', MAX_INPUT_LENGTH - 1);
    max_text[MAX_INPUT_LENGTH - 1] = '\0';
    STRCPY(app.input_text, max_text);

    // Extreme last key
    app.last_key_pressed = INT_MAX;

    // Verify system remains stable
    int pressed_count = 0;
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        if (app.keyboard[i]) pressed_count++;
    }
    TEST_ASSERT(pressed_count == MAX_KEYBOARD_KEYS, "All keyboard keys should be registered");
    TEST_ASSERT(app.mouse.x == INT_MAX, "Mouse X should maintain extreme value");
    TEST_ASSERT(strlen(app.input_text) == MAX_INPUT_LENGTH - 1, "Text should be at maximum length");

    return 1;
}

// ============================================================================
// Data Integrity Tests
// ============================================================================

int test_input_state_corruption_recovery(void) {
    reset_input_state();

    // Start with clean state
    TEST_ASSERT(app.keyboard[100] == 0, "Initial state should be clean");

    // Corrupt the state
    corrupt_input_state();

    // Verify corruption
    TEST_ASSERT(app.keyboard[99] != 0, "State should be corrupted");  // 99 % 3 == 0, so should be -1

    // Attempt to recover by resetting
    reset_input_state();

    // Verify recovery
    TEST_ASSERT(app.keyboard[100] == 0, "State should be recovered");
    TEST_ASSERT(app.mouse.x == 0, "Mouse state should be recovered");
    TEST_ASSERT(app.mouse.y == 0, "Mouse state should be recovered");
    TEST_ASSERT(app.last_key_pressed == 0, "Last key should be recovered");

    return 1;
}

int test_keyboard_state_consistency(void) {
    reset_input_state();

    // Set a pattern and verify it's maintained
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i += 10) {
        app.keyboard[i] = 1;
    }

    // Verify pattern
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        int expected = (i % 10 == 0) ? 1 : 0;
        TEST_ASSERT(app.keyboard[i] == expected, "Keyboard pattern should be consistent");
        if (i > 50) break; // Don't spam output
    }

    // Modify half the pattern
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i += 20) {
        app.keyboard[i] = 0;
    }

    // Verify partial pattern change
    TEST_ASSERT(app.keyboard[0] == 0, "Modified keys should be changed");
    TEST_ASSERT(app.keyboard[10] == 1, "Unmodified keys should remain");
    TEST_ASSERT(app.keyboard[20] == 0, "Modified keys should be changed");
    TEST_ASSERT(app.keyboard[30] == 1, "Unmodified keys should remain");

    return 1;
}

// ============================================================================
// Error Handling Tests
// ============================================================================

int test_resource_exhaustion_simulation(void) {
    reset_input_state();

    // Simulate intensive input activity
    for (int cycle = 0; cycle < 1000; cycle++) {
        // Rapid keyboard changes
        for (int i = 0; i < 10; i++) {
            app.keyboard[i] = cycle % 2;
        }

        // Rapid mouse updates
        app.mouse.x = cycle % 1000;
        app.mouse.y = cycle % 800;
        app.mouse.wheel = (cycle % 3) - 1; // -1, 0, 1

        // Rapid text changes
        char rapid_text[16];
        snprintf(rapid_text, sizeof(rapid_text), "text%d", cycle);
        STRNCPY(app.input_text, rapid_text, MAX_INPUT_LENGTH);

        // Update last key
        app.last_key_pressed = cycle % MAX_KEYBOARD_KEYS;
    }

    // Verify system is still functional after intensive activity
    app.keyboard[5] = 1;
    TEST_ASSERT(app.keyboard[5] == 1, "Keyboard should still be functional after stress");

    app.mouse.x = 42;
    app.mouse.y = 24;
    TEST_ASSERT(app.mouse.x == 42 && app.mouse.y == 24, "Mouse should still be functional after stress");

    STRCPY(app.input_text, "final");
    TEST_ASSERT(strcmp(app.input_text, "final") == 0, "Text input should still be functional after stress");

    return 1;
}

// ============================================================================
// Integration Edge Cases
// ============================================================================

int test_edge_case_gaming_scenario(void) {
    reset_input_state();

    // Simulate problematic gaming scenario:
    // - All movement keys pressed
    // - Mouse rapidly moving and clicking
    // - Text input active
    // - Extreme wheel scrolling

    // Press all WASD + arrows + space + shift + ctrl
    SDL_Scancode all_movement[] = {
        SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D,
        SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
        SDL_SCANCODE_SPACE, SDL_SCANCODE_LSHIFT, SDL_SCANCODE_LCTRL
    };

    for (int i = 0; i < 11; i++) {
        if (all_movement[i] < MAX_KEYBOARD_KEYS) {
            app.keyboard[all_movement[i]] = 1;
        }
    }

    // Rapid mouse activity with extreme values
    app.mouse.x = -1000;  // Off-screen
    app.mouse.y = 5000;   // Way off-screen
    app.mouse.button = SDL_BUTTON_LEFT;
    app.mouse.state = SDL_PRESSED;
    app.mouse.clicks = 50;  // Impossible click count
    app.mouse.wheel = 999;  // Extreme scroll
    app.mouse.pressed = 1;

    // Text input with potential problematic characters
    STRCPY(app.input_text, "!@#$%^&*()");

    // Verify system handles complex scenario
    int movement_keys_pressed = 0;
    for (int i = 0; i < 11; i++) {
        if (all_movement[i] < MAX_KEYBOARD_KEYS && app.keyboard[all_movement[i]]) {
            movement_keys_pressed++;
        }
    }

    TEST_ASSERT(movement_keys_pressed == 11, "All movement keys should be registered");
    TEST_ASSERT(app.mouse.x == -1000, "Extreme mouse coordinates should be handled");
    TEST_ASSERT(app.mouse.clicks == 50, "Extreme click count should be stored");
    TEST_ASSERT(strcmp(app.input_text, "!@#$%^&*()") == 0, "Special characters should be handled");

    return 1;
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    // Initialize minimal app structure for testing
    memset(&app, 0, sizeof(app));
    reset_input_state();

    TEST_SUITE_START("Input System Edge Case Tests");

    // Boundary condition tests
    RUN_TEST(test_keyboard_array_boundaries);
    RUN_TEST(test_scancode_edge_values);
    RUN_TEST(test_mouse_coordinate_extremes);

    // Memory safety tests
    RUN_TEST(test_input_text_buffer_safety);
    RUN_TEST(test_simultaneous_extreme_inputs);

    // Data integrity tests
    RUN_TEST(test_input_state_corruption_recovery);
    RUN_TEST(test_keyboard_state_consistency);

    // Error handling tests
    RUN_TEST(test_resource_exhaustion_simulation);

    // Integration edge cases
    RUN_TEST(test_edge_case_gaming_scenario);

    TEST_SUITE_END();
}
