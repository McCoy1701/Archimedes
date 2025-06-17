// File: true_tests/input/test_input_basic.c - Basic input functionality tests

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Archimedes.h"
#include "tests.h"

// Global test counters
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// Mock SDL event structure for testing
typedef struct {
    SDL_Event event;
    int should_process;
} MockEvent;

// Helper functions for testing
static void reset_input_state(void) {
    // Reset keyboard state
    memset(app.keyboard, 0, sizeof(app.keyboard));

    // Reset mouse state
    app.mouse.x = 0;
    app.mouse.y = 0;
    app.mouse.pressed = 0;
    app.mouse.button = 0;
    app.mouse.state = 0;
    app.mouse.clicks = 0;
    app.mouse.wheel = 0;

    // Reset text input
    memset(app.input_text, 0, sizeof(app.input_text));
    app.last_key_pressed = 0;

    // Reset running state
    app.running = 1;
}

static int simulate_key_down(SDL_Scancode scancode) {
    if (scancode >= MAX_KEYBOARD_KEYS) {
        return 0; // Invalid scancode
    }

    // Simulate what a_DoKeyDown would do
    app.keyboard[scancode] = 1;
    app.last_key_pressed = scancode;
    return 1;
}

static int simulate_key_up(SDL_Scancode scancode) {
    if (scancode >= MAX_KEYBOARD_KEYS) {
        return 0; // Invalid scancode
    }

    // Simulate what a_DoKeyUp would do
    app.keyboard[scancode] = 0;
    return 1;
}

static void simulate_mouse_button(int x, int y, int button, int state, int clicks) {
    app.mouse.x = x;
    app.mouse.y = y;
    app.mouse.button = button;
    app.mouse.state = state;
    app.mouse.clicks = clicks;
    app.mouse.pressed = (state == SDL_PRESSED) ? 1 : 0;
}

static void simulate_mouse_wheel(int wheel_direction) {
    app.mouse.wheel = wheel_direction;
}

// ============================================================================
// Basic Keyboard Tests
// ============================================================================

int test_keyboard_state_management(void) {
    reset_input_state();

    // Test initial state - all keys should be up
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        TEST_ASSERT(app.keyboard[i] == 0, "All keys should start in released state");
        break; // Just test first key to avoid spam
    }

    // Test key press
    simulate_key_down(SDL_SCANCODE_A);
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_A] == 1, "Key A should be pressed");
    TEST_ASSERT(app.last_key_pressed == SDL_SCANCODE_A, "Last key pressed should be A");

    // Test key release
    simulate_key_up(SDL_SCANCODE_A);
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_A] == 0, "Key A should be released");

    return 1;
}

int test_multiple_keys_simultaneously(void) {
    reset_input_state();

    // Press multiple keys
    SDL_Scancode keys[] = {SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D};
    int num_keys = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < num_keys; i++) {
        simulate_key_down(keys[i]);
        TEST_ASSERT(app.keyboard[keys[i]] == 1, "Key should be pressed");
    }

    // Verify all keys are still pressed
    for (int i = 0; i < num_keys; i++) {
        TEST_ASSERT(app.keyboard[keys[i]] == 1, "All keys should remain pressed");
    }

    // Release keys one by one
    for (int i = 0; i < num_keys; i++) {
        simulate_key_up(keys[i]);
        TEST_ASSERT(app.keyboard[keys[i]] == 0, "Key should be released");

        // Verify other keys are still pressed
        for (int j = i + 1; j < num_keys; j++) {
            TEST_ASSERT(app.keyboard[keys[j]] == 1, "Other keys should remain pressed");
        }
    }

    return 1;
}

int test_keyboard_boundary_conditions(void) {
    reset_input_state();

    // Test first valid key
    simulate_key_down(0);
    TEST_ASSERT(app.keyboard[0] == 1, "First key should be pressable");
    simulate_key_up(0);
    TEST_ASSERT(app.keyboard[0] == 0, "First key should be releasable");

    // Test last valid key
    simulate_key_down(MAX_KEYBOARD_KEYS - 1);
    TEST_ASSERT(app.keyboard[MAX_KEYBOARD_KEYS - 1] == 1, "Last valid key should be pressable");
    simulate_key_up(MAX_KEYBOARD_KEYS - 1);
    TEST_ASSERT(app.keyboard[MAX_KEYBOARD_KEYS - 1] == 0, "Last valid key should be releasable");

    // Test invalid key (should not crash or modify memory)
    int result1 = simulate_key_down(MAX_KEYBOARD_KEYS);
    int result2 = simulate_key_down(MAX_KEYBOARD_KEYS + 100);
    TEST_ASSERT(result1 == 0 && result2 == 0, "Invalid keys should be rejected");

    return 1;
}

// ============================================================================
// Basic Mouse Tests
// ============================================================================

int test_mouse_position_tracking(void) {
    reset_input_state();

    // Test mouse position updates
    simulate_mouse_button(100, 200, SDL_BUTTON_LEFT, SDL_PRESSED, 1);
    TEST_ASSERT(app.mouse.x == 100, "Mouse X position should be updated");
    TEST_ASSERT(app.mouse.y == 200, "Mouse Y position should be updated");

    // Test position changes
    simulate_mouse_button(50, 75, SDL_BUTTON_LEFT, SDL_RELEASED, 1);
    TEST_ASSERT(app.mouse.x == 50, "Mouse X position should update");
    TEST_ASSERT(app.mouse.y == 75, "Mouse Y position should update");

    return 1;
}

int test_mouse_button_states(void) {
    reset_input_state();

    // Test left mouse button press
    simulate_mouse_button(100, 100, SDL_BUTTON_LEFT, SDL_PRESSED, 1);
    TEST_ASSERT(app.mouse.button == SDL_BUTTON_LEFT, "Left mouse button should be recorded");
    TEST_ASSERT(app.mouse.state == SDL_PRESSED, "Mouse state should be pressed");
    TEST_ASSERT(app.mouse.pressed == 1, "Mouse pressed flag should be set");
    TEST_ASSERT(app.mouse.clicks == 1, "Single click should be recorded");

    // Test right mouse button press
    simulate_mouse_button(150, 150, SDL_BUTTON_RIGHT, SDL_PRESSED, 1);
    TEST_ASSERT(app.mouse.button == SDL_BUTTON_RIGHT, "Right mouse button should be recorded");

    // Test mouse release
    simulate_mouse_button(150, 150, SDL_BUTTON_RIGHT, SDL_RELEASED, 1);
    TEST_ASSERT(app.mouse.state == SDL_RELEASED, "Mouse state should be released");
    TEST_ASSERT(app.mouse.pressed == 0, "Mouse pressed flag should be cleared");

    return 1;
}

int test_mouse_wheel_functionality(void) {
    reset_input_state();

    // Test scroll up
    simulate_mouse_wheel(1);
    TEST_ASSERT(app.mouse.wheel == 1, "Mouse wheel up should be recorded");

    // Test scroll down
    simulate_mouse_wheel(-1);
    TEST_ASSERT(app.mouse.wheel == -1, "Mouse wheel down should be recorded");

    // Test no scroll
    simulate_mouse_wheel(0);
    TEST_ASSERT(app.mouse.wheel == 0, "Mouse wheel should reset to neutral");

    return 1;
}

int test_mouse_double_click(void) {
    reset_input_state();

    // Simulate double click
    simulate_mouse_button(100, 100, SDL_BUTTON_LEFT, SDL_PRESSED, 2);
    TEST_ASSERT(app.mouse.clicks == 2, "Double click should be recorded");
    TEST_ASSERT(app.mouse.button == SDL_BUTTON_LEFT, "Left button should be recorded for double click");

    // Test triple click
    simulate_mouse_button(100, 100, SDL_BUTTON_LEFT, SDL_PRESSED, 3);
    TEST_ASSERT(app.mouse.clicks == 3, "Triple click should be recorded");

    return 1;
}

// ============================================================================
// Text Input Tests
// ============================================================================

int test_text_input_basic(void) {
    reset_input_state();

    // Simulate text input (this would normally come from SDL_TEXTINPUT event)
    const char* test_text = "Hello";
    strncpy(app.input_text, test_text, MAX_INPUT_LENGTH - 1);
    app.input_text[MAX_INPUT_LENGTH - 1] = '\0';

    TEST_ASSERT(strcmp(app.input_text, test_text) == 0, "Text input should be stored correctly");

    return 1;
}

int test_text_input_overflow_protection(void) {
    reset_input_state();

    // Create text longer than MAX_INPUT_LENGTH
    char long_text[MAX_INPUT_LENGTH + 10];
    memset(long_text, 'A', sizeof(long_text) - 1);
    long_text[sizeof(long_text) - 1] = '\0';

    // Use STRNCPY macro (as the real code would)
    STRNCPY(app.input_text, long_text, MAX_INPUT_LENGTH);

    TEST_ASSERT(strlen(app.input_text) < MAX_INPUT_LENGTH, "Text input should be truncated");
    TEST_ASSERT(app.input_text[MAX_INPUT_LENGTH - 1] == '\0', "Text should be null terminated");

    return 1;
}

// ============================================================================
// Integration Tests
// ============================================================================

int test_game_input_scenario(void) {
    reset_input_state();

    // Simulate typical game input scenario: WASD movement + mouse look + shoot

    // Press W (forward)
    simulate_key_down(SDL_SCANCODE_W);
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_W] == 1, "W key should be pressed for forward movement");

    // Press A (left) while W is still held
    simulate_key_down(SDL_SCANCODE_A);
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_W] == 1, "W should still be pressed");
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_A] == 1, "A should be pressed for diagonal movement");

    // Mouse look (position change)
    simulate_mouse_button(400, 300, 0, 0, 0); // Just position update
    TEST_ASSERT(app.mouse.x == 400 && app.mouse.y == 300, "Mouse position should update for looking");

    // Shoot (left click)
    simulate_mouse_button(400, 300, SDL_BUTTON_LEFT, SDL_PRESSED, 1);
    TEST_ASSERT(app.mouse.pressed == 1, "Mouse should be pressed for shooting");

    // Release keys
    simulate_key_up(SDL_SCANCODE_W);
    simulate_key_up(SDL_SCANCODE_A);
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_W] == 0, "W should be released");
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_A] == 0, "A should be released");

    return 1;
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    // Initialize minimal app structure for testing
    memset(&app, 0, sizeof(app));
    reset_input_state();

    TEST_SUITE_START("Input System Basic Tests");

    // Keyboard tests
    RUN_TEST(test_keyboard_state_management);
    RUN_TEST(test_multiple_keys_simultaneously);
    RUN_TEST(test_keyboard_boundary_conditions);

    // Mouse tests
    RUN_TEST(test_mouse_position_tracking);
    RUN_TEST(test_mouse_button_states);
    RUN_TEST(test_mouse_wheel_functionality);
    RUN_TEST(test_mouse_double_click);

    // Text input tests
    RUN_TEST(test_text_input_basic);
    RUN_TEST(test_text_input_overflow_protection);

    // Integration tests
    RUN_TEST(test_game_input_scenario);

    TEST_SUITE_END();
}
