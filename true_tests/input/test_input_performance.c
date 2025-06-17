// File: true_tests/input/test_input_performance.c - Performance and stress tests for input system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "Archimedes.h"
#include "tests.h"

// Global test counters
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// Performance test configuration
#define PERFORMANCE_ITERATIONS 10000
#define STRESS_TEST_CYCLES 100
#define RAPID_INPUT_COUNT 1000

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

static double get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

// ============================================================================
// Keyboard Performance Tests
// ============================================================================

int test_keyboard_rapid_state_changes(void) {
    reset_input_state();

    double start_time = get_time_ms();

    // Simulate rapid key presses and releases
    for (int i = 0; i < RAPID_INPUT_COUNT; i++) {
        SDL_Scancode key = (SDL_Scancode)(i % 100); // Cycle through first 100 keys

        if (key < MAX_KEYBOARD_KEYS) {
            // Press
            app.keyboard[key] = 1;
            app.last_key_pressed = key;

            // Release
            app.keyboard[key] = 0;
        }
    }

    double end_time = get_time_ms();
    double elapsed = end_time - start_time;

    printf("    Rapid keyboard changes: %d operations in %.3f ms (%.1f ops/ms)\n",
           RAPID_INPUT_COUNT * 2, elapsed, (RAPID_INPUT_COUNT * 2) / elapsed);

    TEST_ASSERT(elapsed < 100.0, "Rapid keyboard changes should complete within 100ms");
    TEST_ASSERT(app.keyboard[99] == 0, "Final key state should be correct");

    return 1;
}

int test_keyboard_simultaneous_keys_performance(void) {
    reset_input_state();

    double start_time = get_time_ms();

    // Press many keys simultaneously
    for (int cycle = 0; cycle < STRESS_TEST_CYCLES; cycle++) {
        // Press all WASD + arrow keys + modifiers
        SDL_Scancode gaming_keys[] = {
            SDL_SCANCODE_W, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D,
            SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT,
            SDL_SCANCODE_LSHIFT, SDL_SCANCODE_RSHIFT, SDL_SCANCODE_LCTRL, SDL_SCANCODE_RCTRL,
            SDL_SCANCODE_SPACE, SDL_SCANCODE_RETURN, SDL_SCANCODE_TAB, SDL_SCANCODE_ESCAPE
        };

        int num_keys = sizeof(gaming_keys) / sizeof(gaming_keys[0]);

        // Press all keys
        for (int i = 0; i < num_keys; i++) {
            if (gaming_keys[i] < MAX_KEYBOARD_KEYS) {
                app.keyboard[gaming_keys[i]] = 1;
            }
        }

        // Check state of all keys
        int pressed_count = 0;
        for (int i = 0; i < num_keys; i++) {
            if (gaming_keys[i] < MAX_KEYBOARD_KEYS && app.keyboard[gaming_keys[i]]) {
                pressed_count++;
            }
        }

        // Release all keys
        for (int i = 0; i < num_keys; i++) {
            if (gaming_keys[i] < MAX_KEYBOARD_KEYS) {
                app.keyboard[gaming_keys[i]] = 0;
            }
        }
    }

    double end_time = get_time_ms();
    double elapsed = end_time - start_time;

    printf("    Simultaneous key handling: %d cycles in %.3f ms (%.1f cycles/ms)\n",
           STRESS_TEST_CYCLES, elapsed, STRESS_TEST_CYCLES / elapsed);

    TEST_ASSERT(elapsed < 50.0, "Simultaneous key handling should be efficient");

    return 1;
}

int test_keyboard_array_access_performance(void) {
    reset_input_state();

    double start_time = get_time_ms();

    // Test array access patterns
    for (int i = 0; i < PERFORMANCE_ITERATIONS; i++) {
        // Sequential access
        for (int j = 0; j < 100; j++) {
            app.keyboard[j] = (i + j) % 2;
        }

        // Random access
        for (int j = 0; j < 50; j++) {
            int index = (i * 17 + j * 13) % MAX_KEYBOARD_KEYS;
            app.keyboard[index] = j % 2;
        }

        // Count pressed keys (common operation)
        int pressed_count = 0;
        for (int j = 0; j < 100; j++) {
            if (app.keyboard[j]) pressed_count++;
        }
    }

    double end_time = get_time_ms();
    double elapsed = end_time - start_time;

    printf("    Keyboard array access: %d iterations in %.3f ms (%.1f iter/ms)\n",
           PERFORMANCE_ITERATIONS, elapsed, PERFORMANCE_ITERATIONS / elapsed);

    TEST_ASSERT(elapsed < 200.0, "Keyboard array access should be fast");

    return 1;
}

// ============================================================================
// Mouse Performance Tests
// ============================================================================

int test_mouse_rapid_movement_tracking(void) {
    reset_input_state();

    double start_time = get_time_ms();

    // Simulate rapid mouse movement
    for (int i = 0; i < RAPID_INPUT_COUNT; i++) {
        app.mouse.x = (i * 7) % 1920;  // Simulate movement across screen
        app.mouse.y = (i * 11) % 1080;

        // Simulate occasional clicks
        if (i % 100 == 0) {
            app.mouse.button = SDL_BUTTON_LEFT;
            app.mouse.state = SDL_PRESSED;
            app.mouse.pressed = 1;
            app.mouse.clicks = 1;
        } else if (i % 100 == 50) {
            app.mouse.state = SDL_RELEASED;
            app.mouse.pressed = 0;
        }
    }

    double end_time = get_time_ms();
    double elapsed = end_time - start_time;

    printf("    Rapid mouse movement: %d updates in %.3f ms (%.1f updates/ms)\n",
           RAPID_INPUT_COUNT, elapsed, RAPID_INPUT_COUNT / elapsed);

    TEST_ASSERT(elapsed < 50.0, "Rapid mouse movement should be efficient");
    TEST_ASSERT(app.mouse.x == (RAPID_INPUT_COUNT - 1) * 7 % 1920, "Final mouse position should be correct");

    return 1;
}

int test_mouse_wheel_scroll_performance(void) {
    reset_input_state();

    double start_time = get_time_ms();

    // Simulate rapid wheel scrolling
    int total_scroll = 0;
    for (int i = 0; i < RAPID_INPUT_COUNT; i++) {
        int scroll_direction = (i % 3) - 1; // -1, 0, 1
        app.mouse.wheel = scroll_direction;

        // Simulate processing the wheel value
        if (app.mouse.wheel != 0) {
            total_scroll += app.mouse.wheel;
            // Reset wheel (common pattern)
            app.mouse.wheel = 0;
        }
    }

    double end_time = get_time_ms();
    double elapsed = end_time - start_time;

    printf("    Rapid wheel scrolling: %d scroll events in %.3f ms (%.1f events/ms)\n",
           RAPID_INPUT_COUNT, elapsed, RAPID_INPUT_COUNT / elapsed);

    TEST_ASSERT(elapsed < 30.0, "Rapid wheel scrolling should be efficient");
    TEST_ASSERT(app.mouse.wheel == 0, "Wheel should be reset after processing");

    return 1;
}

int test_mouse_click_sequence_performance(void) {
    reset_input_state();

    double start_time = get_time_ms();

    // Simulate rapid clicking patterns
    for (int i = 0; i < RAPID_INPUT_COUNT; i++) {
        // Cycle through different click patterns
        int pattern = i % 6;

        switch (pattern) {
            case 0: // Single left click
                app.mouse.button = SDL_BUTTON_LEFT;
                app.mouse.state = SDL_PRESSED;
                app.mouse.clicks = 1;
                break;
            case 1: // Release left
                app.mouse.state = SDL_RELEASED;
                app.mouse.pressed = 0;
                break;
            case 2: // Double click
                app.mouse.button = SDL_BUTTON_LEFT;
                app.mouse.state = SDL_PRESSED;
                app.mouse.clicks = 2;
                break;
            case 3: // Right click
                app.mouse.button = SDL_BUTTON_RIGHT;
                app.mouse.state = SDL_PRESSED;
                app.mouse.clicks = 1;
                break;
            case 4: // Middle click
                app.mouse.button = SDL_BUTTON_MIDDLE;
                app.mouse.state = SDL_PRESSED;
                app.mouse.clicks = 1;
                break;
            case 5: // Release
                app.mouse.state = SDL_RELEASED;
                app.mouse.pressed = 0;
                break;
        }
    }

    double end_time = get_time_ms();
    double elapsed = end_time - start_time;

    printf("    Rapid click sequences: %d click events in %.3f ms (%.1f events/ms)\n",
           RAPID_INPUT_COUNT, elapsed, RAPID_INPUT_COUNT / elapsed);

    TEST_ASSERT(elapsed < 40.0, "Rapid click sequences should be efficient");

    return 1;
}

// ============================================================================
// Text Input Performance Tests
// ============================================================================

int test_text_input_processing_performance(void) {
    reset_input_state();

    double start_time = get_time_ms();

    // Simulate rapid text input
    char test_strings[][16] = {
        "Hello", "World", "Test", "Input", "Speed", "Check", "Fast", "Type",
        "Quick", "Brown", "Fox", "Jump", "Over", "Lazy", "Dog", "End"
    };
    int num_strings = sizeof(test_strings) / sizeof(test_strings[0]);

    for (int i = 0; i < RAPID_INPUT_COUNT; i++) {
        const char* text = test_strings[i % num_strings];
        STRNCPY(app.input_text, text, MAX_INPUT_LENGTH);

        // Simulate processing the text (common operations)
        int text_len = strlen(app.input_text);
        if (text_len > 0) {
            // Character validation
            for (int j = 0; j < text_len; j++) {
                char c = app.input_text[j];
                if (c >= 'A' && c <= 'Z') {
                    // Process uppercase
                } else if (c >= 'a' && c <= 'z') {
                    // Process lowercase
                }
            }
        }
    }

    double end_time = get_time_ms();
    double elapsed = end_time - start_time;

    printf("    Text input processing: %d text updates in %.3f ms (%.1f updates/ms)\n",
           RAPID_INPUT_COUNT, elapsed, RAPID_INPUT_COUNT / elapsed);

    TEST_ASSERT(elapsed < 100.0, "Text input processing should be efficient");
    TEST_ASSERT(strlen(app.input_text) > 0, "Final text should be set");

    return 1;
}

// ============================================================================
// Combined Input Performance Tests
// ============================================================================

int test_mixed_input_stress_performance(void) {
    reset_input_state();

    double start_time = get_time_ms();

    // Simulate complex input scenarios with everything happening simultaneously
    for (int i = 0; i < STRESS_TEST_CYCLES; i++) {
        // Keyboard activity
        for (int j = 0; j < 10; j++) {
            SDL_Scancode key = (SDL_Scancode)((i * 10 + j) % 100);
            if (key < MAX_KEYBOARD_KEYS) {
                app.keyboard[key] = (i + j) % 2;
            }
        }

        // Mouse movement and clicks
        app.mouse.x = (i * 13) % 1920;
        app.mouse.y = (i * 17) % 1080;
        app.mouse.button = (i % 3) + 1; // Cycle through mouse buttons
        app.mouse.state = (i % 2) ? SDL_PRESSED : SDL_RELEASED;
        app.mouse.wheel = (i % 5) - 2; // -2 to 2 range

        // Text input
        char temp_text[16];
        snprintf(temp_text, sizeof(temp_text), "test%d", i);
        STRNCPY(app.input_text, temp_text, MAX_INPUT_LENGTH);

        // Simulate input processing (counting active inputs)
        int active_keys = 0;
        for (int j = 0; j < 100; j++) {
            if (app.keyboard[j]) active_keys++;
        }

        // Mouse distance calculation (common operation)
        int mouse_distance = abs(app.mouse.x - 960) + abs(app.mouse.y - 540);

        // Text length check
        int text_length = strlen(app.input_text);

        // Prevent compiler optimization
        if (active_keys > 50 || mouse_distance > 2000 || text_length > 10) {
            // Unlikely condition to prevent optimization
        }
    }

    double end_time = get_time_ms();
    double elapsed = end_time - start_time;

    printf("    Mixed input stress test: %d cycles in %.3f ms (%.1f cycles/ms)\n",
           STRESS_TEST_CYCLES, elapsed, STRESS_TEST_CYCLES / elapsed);

    TEST_ASSERT(elapsed < 100.0, "Mixed input stress test should complete efficiently");

    return 1;
}

int test_input_state_reset_performance(void) {
    // Test the performance of resetting input state (common operation)
    double start_time = get_time_ms();

    for (int i = 0; i < PERFORMANCE_ITERATIONS; i++) {
        reset_input_state();
    }

    double end_time = get_time_ms();
    double elapsed = end_time - start_time;

    printf("    Input state reset: %d resets in %.3f ms (%.1f resets/ms)\n",
           PERFORMANCE_ITERATIONS, elapsed, PERFORMANCE_ITERATIONS / elapsed);

    TEST_ASSERT(elapsed < 50.0, "Input state reset should be fast");
    TEST_ASSERT(app.keyboard[50] == 0, "Keyboard state should be reset");
    TEST_ASSERT(app.mouse.x == 0, "Mouse state should be reset");
    TEST_ASSERT(app.input_text[0] == '\0', "Text should be reset");

    return 1;
}

int test_input_boundary_access_performance(void) {
    reset_input_state();

    double start_time = get_time_ms();

    // Test boundary access patterns (first, last, and middle elements)
    for (int i = 0; i < PERFORMANCE_ITERATIONS; i++) {
        // Access boundary keyboard elements
        app.keyboard[0] = i % 2;
        app.keyboard[MAX_KEYBOARD_KEYS - 1] = (i + 1) % 2;
        app.keyboard[MAX_KEYBOARD_KEYS / 2] = (i + 2) % 2;

        // Read back the values
        int first = app.keyboard[0];
        int last = app.keyboard[MAX_KEYBOARD_KEYS - 1];
        int middle = app.keyboard[MAX_KEYBOARD_KEYS / 2];

        // Extreme mouse coordinates
        app.mouse.x = (i % 2) ? -1000 : 5000;
        app.mouse.y = (i % 2) ? -1000 : 5000;

        // Text buffer boundary
        if (i % 100 == 0) {
            char max_text[MAX_INPUT_LENGTH];
            memset(max_text, 'A', MAX_INPUT_LENGTH - 1);
            max_text[MAX_INPUT_LENGTH - 1] = '\0';
            STRNCPY(app.input_text, max_text, MAX_INPUT_LENGTH);
        }

        // Prevent compiler optimization
        if (first + last + middle > 10) {
            // Unlikely condition
        }
    }

    double end_time = get_time_ms();
    double elapsed = end_time - start_time;

    printf("    Boundary access test: %d iterations in %.3f ms (%.1f iter/ms)\n",
           PERFORMANCE_ITERATIONS, elapsed, PERFORMANCE_ITERATIONS / elapsed);

    TEST_ASSERT(elapsed < 100.0, "Boundary access should be efficient");

    return 1;
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    // Initialize minimal app structure for testing
    mock_app_init();

    TEST_SUITE_START("Input System Performance Tests");

    // Keyboard performance tests
    RUN_TEST(test_keyboard_rapid_state_changes);
    RUN_TEST(test_keyboard_simultaneous_keys_performance);
    RUN_TEST(test_keyboard_array_access_performance);

    // Mouse performance tests
    RUN_TEST(test_mouse_rapid_movement_tracking);
    RUN_TEST(test_mouse_wheel_scroll_performance);
    RUN_TEST(test_mouse_click_sequence_performance);

    // Text input performance tests
    RUN_TEST(test_text_input_processing_performance);

    // Combined performance tests
    RUN_TEST(test_mixed_input_stress_performance);
    RUN_TEST(test_input_state_reset_performance);
    RUN_TEST(test_input_boundary_access_performance);

    TEST_SUITE_END();
}
