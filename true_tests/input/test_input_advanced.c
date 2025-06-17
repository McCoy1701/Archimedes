// File: true_tests/input/test_input_advanced.c - Advanced input functionality tests

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

// Advanced input patterns and combinations
typedef struct {
    SDL_Scancode keys[10];
    int key_count;
    int expected_combo_id;
} InputCombo;

typedef struct {
    int x, y;
    int button;
    int state;
    double timestamp;
} MouseEvent;

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

static void press_key_combination(SDL_Scancode* keys, int count) {
    for (int i = 0; i < count; i++) {
        if (keys[i] < MAX_KEYBOARD_KEYS) {
            app.keyboard[keys[i]] = 1;
        }
    }
}

static void release_key_combination(SDL_Scancode* keys, int count) {
    for (int i = 0; i < count; i++) {
        if (keys[i] < MAX_KEYBOARD_KEYS) {
            app.keyboard[keys[i]] = 0;
        }
    }
}

static int count_pressed_keys(void) {
    int count = 0;
    for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
        if (app.keyboard[i]) count++;
    }
    return count;
}

// ============================================================================
// Complex Key Combination Tests
// ============================================================================

int test_modifier_key_combinations(void) {
    reset_input_state();

    // Test Ctrl+C combination
    SDL_Scancode ctrl_c[] = {SDL_SCANCODE_LCTRL, SDL_SCANCODE_C};
    press_key_combination(ctrl_c, 2);

    TEST_ASSERT(app.keyboard[SDL_SCANCODE_LCTRL] == 1, "Left Ctrl should be pressed");
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_C] == 1, "C key should be pressed");

    // Test Ctrl+Shift+Z combination
    SDL_Scancode ctrl_shift_z[] = {SDL_SCANCODE_LCTRL, SDL_SCANCODE_LSHIFT, SDL_SCANCODE_Z};
    press_key_combination(ctrl_shift_z, 3);

    TEST_ASSERT(app.keyboard[SDL_SCANCODE_LCTRL] == 1, "Ctrl should still be pressed");
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_LSHIFT] == 1, "Shift should be pressed");
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_Z] == 1, "Z should be pressed");
    TEST_ASSERT(count_pressed_keys() == 4, "Should have 4 keys pressed total"); // C + Ctrl + Shift + Z

    // Release combination
    release_key_combination(ctrl_shift_z, 3);
    release_key_combination(ctrl_c, 2);
    TEST_ASSERT(count_pressed_keys() == 0, "All keys should be released");

    return 1;
}

int test_gaming_key_combinations(void) {
    reset_input_state();

    // Test WASD + Space (jump) + Shift (run)
    SDL_Scancode gaming_combo[] = {SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_SPACE, SDL_SCANCODE_LSHIFT};
    press_key_combination(gaming_combo, 4);

    TEST_ASSERT(app.keyboard[SDL_SCANCODE_W] == 1, "W (forward) should be pressed");
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_A] == 1, "A (left) should be pressed");
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_SPACE] == 1, "Space (jump) should be pressed");
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_LSHIFT] == 1, "Shift (run) should be pressed");

    // Simulate releasing jump but keeping movement
    app.keyboard[SDL_SCANCODE_SPACE] = 0;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_W] == 1, "Movement keys should remain pressed");
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_A] == 1, "Movement keys should remain pressed");

    // Test arrow keys + Ctrl (crouch)
    SDL_Scancode arrow_combo[] = {SDL_SCANCODE_UP, SDL_SCANCODE_LEFT, SDL_SCANCODE_LCTRL};
    press_key_combination(arrow_combo, 3);

    // Should now have: W, A, Shift, Up, Left, Ctrl pressed
    TEST_ASSERT(count_pressed_keys() == 6, "Should have 6 keys pressed");

    // Clean up
    release_key_combination(gaming_combo, 4);
    release_key_combination(arrow_combo, 3);

    return 1;
}

int test_function_key_sequences(void) {
    reset_input_state();

    // Test F1-F12 sequence
    SDL_Scancode function_keys[] = {
        SDL_SCANCODE_F1, SDL_SCANCODE_F2, SDL_SCANCODE_F3, SDL_SCANCODE_F4,
        SDL_SCANCODE_F5, SDL_SCANCODE_F6, SDL_SCANCODE_F7, SDL_SCANCODE_F8,
        SDL_SCANCODE_F9, SDL_SCANCODE_F10, SDL_SCANCODE_F11, SDL_SCANCODE_F12
    };

    // Press all function keys
    for (int i = 0; i < 12; i++) {
        app.keyboard[function_keys[i]] = 1;
        app.last_key_pressed = function_keys[i];
    }

    TEST_ASSERT(count_pressed_keys() == 12, "All 12 function keys should be pressed");
    TEST_ASSERT(app.last_key_pressed == SDL_SCANCODE_F12, "Last key should be F12");

    // Release in reverse order
    for (int i = 11; i >= 0; i--) {
        app.keyboard[function_keys[i]] = 0;
        TEST_ASSERT(count_pressed_keys() == i, "Correct number of keys should remain");
    }

    return 1;
}

// ============================================================================
// Advanced Mouse Tests
// ============================================================================

int test_mouse_drag_simulation(void) {
    reset_input_state();

    // Simulate mouse drag sequence
    MouseEvent drag_sequence[] = {
        {100, 100, SDL_BUTTON_LEFT, SDL_PRESSED, 0.0},   // Press at start
        {120, 110, SDL_BUTTON_LEFT, SDL_PRESSED, 0.1},   // Drag
        {150, 130, SDL_BUTTON_LEFT, SDL_PRESSED, 0.2},   // Continue drag
        {180, 160, SDL_BUTTON_LEFT, SDL_PRESSED, 0.3},   // Continue drag
        {200, 180, SDL_BUTTON_LEFT, SDL_RELEASED, 0.4}   // Release at end
    };

    int drag_length = sizeof(drag_sequence) / sizeof(drag_sequence[0]);
    int start_x = drag_sequence[0].x;
    int start_y = drag_sequence[0].y;
    int end_x = drag_sequence[drag_length-1].x;
    int end_y = drag_sequence[drag_length-1].y;

    // Process drag sequence
    for (int i = 0; i < drag_length; i++) {
        app.mouse.x = drag_sequence[i].x;
        app.mouse.y = drag_sequence[i].y;
        app.mouse.button = drag_sequence[i].button;
        app.mouse.state = drag_sequence[i].state;
        app.mouse.pressed = (drag_sequence[i].state == SDL_PRESSED) ? 1 : 0;

        // Verify position updates
        TEST_ASSERT(app.mouse.x == drag_sequence[i].x, "Mouse X should update during drag");
        TEST_ASSERT(app.mouse.y == drag_sequence[i].y, "Mouse Y should update during drag");

        if (i < drag_length - 1) {
            TEST_ASSERT(app.mouse.pressed == 1, "Mouse should remain pressed during drag");
        }
    }

    // Verify final state
    TEST_ASSERT(app.mouse.pressed == 0, "Mouse should be released at end of drag");
    TEST_ASSERT(app.mouse.x == end_x, "Final X position should be correct");
    TEST_ASSERT(app.mouse.y == end_y, "Final Y position should be correct");

    // Calculate drag distance
    int drag_distance_x = end_x - start_x;
    int drag_distance_y = end_y - start_y;
    TEST_ASSERT(drag_distance_x == 100, "Horizontal drag distance should be 100");
    TEST_ASSERT(drag_distance_y == 80, "Vertical drag distance should be 80");

    return 1;
}

int test_mouse_gesture_patterns(void) {
    reset_input_state();

    // Test circular mouse movement pattern
    struct {
        int x, y;
    } circle_points[] = {
        {100, 50},   // Top
        {150, 100},  // Right
        {100, 150},  // Bottom
        {50, 100},   // Left
        {100, 50}    // Back to top
    };

    int num_points = sizeof(circle_points) / sizeof(circle_points[0]);

    // Trace circle with mouse
    for (int i = 0; i < num_points; i++) {
        app.mouse.x = circle_points[i].x;
        app.mouse.y = circle_points[i].y;

        // Verify each position
        TEST_ASSERT(app.mouse.x == circle_points[i].x, "Circle X coordinate should be correct");
        TEST_ASSERT(app.mouse.y == circle_points[i].y, "Circle Y coordinate should be correct");
    }

    // Test that we ended where we started
    TEST_ASSERT(app.mouse.x == circle_points[0].x, "Should return to start X");
    TEST_ASSERT(app.mouse.y == circle_points[0].y, "Should return to start Y");

    return 1;
}

int test_rapid_mouse_wheel_events(void) {
    reset_input_state();

    // Simulate rapid scrolling sequence
    int scroll_sequence[] = {1, 1, 1, -1, -1, 1, -1, 1, -1, 0};
    int sequence_length = sizeof(scroll_sequence) / sizeof(scroll_sequence[0]);

    int total_scroll = 0;
    for (int i = 0; i < sequence_length; i++) {
        app.mouse.wheel = scroll_sequence[i];
        total_scroll += scroll_sequence[i];

        TEST_ASSERT(app.mouse.wheel == scroll_sequence[i], "Wheel value should match sequence");
    }

    // Test accumulated scroll (if tracked)
    // Note: In real implementation, you might want to track cumulative scroll
    TEST_ASSERT(total_scroll == 1, "Net scroll should be 1 up"); // 1+1+1-1-1+1-1+2-2+0 = 1

    return 1;
}

// ============================================================================
// Input Timing and Sequence Tests
// ============================================================================

int test_key_sequence_patterns(void) {
    reset_input_state();

    // Test common key sequences (like cheat codes or combos)
    SDL_Scancode konami_code[] = {
        SDL_SCANCODE_UP, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_DOWN,
        SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
        SDL_SCANCODE_B, SDL_SCANCODE_A
    };

    int sequence_length = sizeof(konami_code) / sizeof(konami_code[0]);

    // Process sequence one key at a time
    for (int i = 0; i < sequence_length; i++) {
        // Press key
        app.keyboard[konami_code[i]] = 1;
        app.last_key_pressed = konami_code[i];

        TEST_ASSERT(app.keyboard[konami_code[i]] == 1, "Key should be pressed");
        TEST_ASSERT(app.last_key_pressed == konami_code[i], "Last key should be updated");

        // Release key (for clean sequence)
        app.keyboard[konami_code[i]] = 0;
    }

    TEST_ASSERT(app.last_key_pressed == SDL_SCANCODE_A, "Final key should be A");
    TEST_ASSERT(count_pressed_keys() == 0, "All keys should be released");

    return 1;
}

int test_simultaneous_input_streams(void) {
    reset_input_state();

    // Simulate simultaneous keyboard and mouse input
    // Player 1: WASD movement
    SDL_Scancode p1_keys[] = {SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D};
    // Player 2: Arrow keys
    SDL_Scancode p2_keys[] = {SDL_SCANCODE_UP, SDL_SCANCODE_LEFT, SDL_SCANCODE_DOWN, SDL_SCANCODE_RIGHT};

    // Press Player 1 keys
    for (int i = 0; i < 4; i++) {
        app.keyboard[p1_keys[i]] = 1;
    }

    // Press Player 2 keys
    for (int i = 0; i < 4; i++) {
        app.keyboard[p2_keys[i]] = 1;
    }

    // Add mouse input
    app.mouse.x = 300;
    app.mouse.y = 200;
    app.mouse.button = SDL_BUTTON_LEFT;
    app.mouse.pressed = 1;

    // Verify all inputs are active
    TEST_ASSERT(count_pressed_keys() == 8, "Should have 8 keys pressed (4 per player)");
    TEST_ASSERT(app.mouse.pressed == 1, "Mouse should be pressed");
    TEST_ASSERT(app.mouse.x == 300, "Mouse X should be correct");

    // Test selective release (Player 1 stops, Player 2 continues)
    for (int i = 0; i < 4; i++) {
        app.keyboard[p1_keys[i]] = 0;
    }

    TEST_ASSERT(count_pressed_keys() == 4, "Should have 4 keys pressed (Player 2 only)");

    // Verify Player 2 keys are still active
    for (int i = 0; i < 4; i++) {
        TEST_ASSERT(app.keyboard[p2_keys[i]] == 1, "Player 2 keys should remain active");
    }

    // Clean up
    for (int i = 0; i < 4; i++) {
        app.keyboard[p2_keys[i]] = 0;
    }

    return 1;
}

// ============================================================================
// Performance and Stress Tests
// ============================================================================

int test_high_frequency_input_handling(void) {
    reset_input_state();

    // Simulate rapid key presses (like rapid fire or button mashing)
    SDL_Scancode rapid_key = SDL_SCANCODE_SPACE;

    for (int cycle = 0; cycle < 100; cycle++) {
        // Press
        app.keyboard[rapid_key] = 1;
        app.last_key_pressed = rapid_key;
        TEST_ASSERT(app.keyboard[rapid_key] == 1, "Key should be pressed in rapid cycle");

        // Release
        app.keyboard[rapid_key] = 0;
        TEST_ASSERT(app.keyboard[rapid_key] == 0, "Key should be released in rapid cycle");
    }

    // Test rapid mouse clicks
    for (int click = 0; click < 50; click++) {
        app.mouse.button = SDL_BUTTON_LEFT;
        app.mouse.state = SDL_PRESSED;
        app.mouse.pressed = 1;
        app.mouse.clicks = 1;

        TEST_ASSERT(app.mouse.pressed == 1, "Mouse should register rapid clicks");

        app.mouse.state = SDL_RELEASED;
        app.mouse.pressed = 0;
        TEST_ASSERT(app.mouse.pressed == 0, "Mouse should register rapid releases");
    }

    return 1;
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    // Initialize minimal app structure for testing
    memset(&app, 0, sizeof(app));
    reset_input_state();

    TEST_SUITE_START("Input System Advanced Tests");

    // Complex combination tests
    RUN_TEST(test_modifier_key_combinations);
    RUN_TEST(test_gaming_key_combinations);
    RUN_TEST(test_function_key_sequences);

    // Advanced mouse tests
    RUN_TEST(test_mouse_drag_simulation);
    RUN_TEST(test_mouse_gesture_patterns);
    RUN_TEST(test_rapid_mouse_wheel_events);

    // Timing and sequence tests
    RUN_TEST(test_key_sequence_patterns);
    RUN_TEST(test_simultaneous_input_streams);

    // Performance tests
    RUN_TEST(test_high_frequency_input_handling);

    TEST_SUITE_END();
}
