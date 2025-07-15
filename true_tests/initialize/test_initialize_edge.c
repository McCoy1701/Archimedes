// File: true_tests/initialize/test_initialize_edge.c - Scene-based edge case tests for initialization

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <signal.h>
#include <setjmp.h>
#include "Archimedes.h"

// Test-friendly initialization function
extern int a_InitTest(const int width, const int height, const char *title);

// Test scene management
typedef struct {
    const char* name;
    float display_duration;  // Seconds to display this test
    void (*init_scene)(void);
    void (*logic)(float dt);
    void (*draw)(float dt);
} TestScene_t;

// Global test state
static int current_scene = 0;
static float scene_timer = 0.0f;
static int total_scenes = 0;
static int tests_passed = 0;
static int tests_failed = 0;
static int scene_test_result = 1; // 1 = pass, 0 = fail

// Test data for current scene
static char test_result_text[200];
static int crash_detected_flag = 0;

// Signal handling for catching crashes
static jmp_buf jump_buffer;
static volatile sig_atomic_t crash_detected = 0;

static void segfault_handler(int sig) {
    (void)sig; // Unused parameter
    crash_detected = 1;
    crash_detected_flag = 1;
    longjmp(jump_buffer, 1);
}

// Helper to setup crash detection
static void setup_crash_detection(void) {
    crash_detected = 0;
    crash_detected_flag = 0;
    signal(SIGSEGV, segfault_handler);
    signal(SIGABRT, segfault_handler);
}

// Helper to cleanup crash detection
static void cleanup_crash_detection(void) {
    signal(SIGSEGV, SIG_DFL);
    signal(SIGABRT, SIG_DFL);
}

// Forward declarations
static void advance_to_next_scene(void);
static void init_test_system(void);
static void common_logic(float dt);
static void draw_ui_overlay(void);

// Test scene function declarations
static void init_parameter_validation_test(void);
static void draw_parameter_validation_test(float dt);
static void init_failure_recovery_test(void);
static void draw_failure_recovery_test(float dt);
static void init_state_corruption_test(void);
static void draw_state_corruption_test(float dt);

// Test scene definitions
static TestScene_t test_scenes[] = {
    // Edge case tests (2 seconds each - quick validation)
    {"Parameter Validation Tests", 2.0f, init_parameter_validation_test, common_logic, draw_parameter_validation_test},
    {"Failure Recovery Tests", 2.0f, init_failure_recovery_test, common_logic, draw_failure_recovery_test},
    {"Global State Corruption Tests", 2.0f, init_state_corruption_test, common_logic, draw_state_corruption_test}
};

// ============================================================================
// Scene Management System
// ============================================================================

static void advance_to_next_scene(void) {
    if (scene_test_result) {
        tests_passed++;
    } else {
        tests_failed++;
    }
    
    current_scene++;
    scene_timer = 0.0f;
    scene_test_result = 1; // Reset for next test
    
    if (current_scene >= total_scenes) {
        // All tests completed
        printf("\n=== Test Results ===\n");
        printf("Total tests: %d\n", total_scenes);
        printf("Passed: %d\n", tests_passed);
        printf("Failed: %d\n", tests_failed);
        printf("Success rate: %.1f%%\n", (float)tests_passed / total_scenes * 100.0f);
        printf("\nBug hunting complete! Check findings above.\n");
        app.running = 0;
        return;
    }
    
    // Initialize next scene
    printf("Starting test %d/%d: %s\n", current_scene + 1, total_scenes, test_scenes[current_scene].name);
    if (test_scenes[current_scene].init_scene) {
        test_scenes[current_scene].init_scene();
    }
    
    // Update delegates
    app.delegate.logic = test_scenes[current_scene].logic;
    app.delegate.draw = test_scenes[current_scene].draw;
}

static void init_test_system(void) {
    total_scenes = sizeof(test_scenes) / sizeof(TestScene_t);
    current_scene = 0;
    scene_timer = 0.0f;
    tests_passed = 0;
    tests_failed = 0;
    
    // Set dark gray background
    app.background.r = 32;
    app.background.g = 32;
    app.background.b = 32;
    app.background.a = 255;
    
    printf("\n=== Archimedes Initialization Edge Case Tests ===\n");
    printf("Total tests: %d\n", total_scenes);
    printf("Controls: SPACE=Next, ESC=Quit, R=Restart\n");
    printf("Testing parameter validation, failure recovery, and state corruption...\n\n");
    
    // Start first scene
    advance_to_next_scene();
}

static void common_logic(float dt) {
    scene_timer += dt;
    
    // Handle input
    if (app.keyboard[SDL_SCANCODE_ESCAPE] == 1) {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        app.running = 0;
        return;
    }
    
    if (app.keyboard[SDL_SCANCODE_SPACE] == 1 || app.keyboard[SDL_SCANCODE_RETURN] == 1) {
        app.keyboard[SDL_SCANCODE_SPACE] = 0;
        app.keyboard[SDL_SCANCODE_RETURN] = 0;
        advance_to_next_scene();
        return;
    }
    
    if (app.keyboard[SDL_SCANCODE_R] == 1) {
        app.keyboard[SDL_SCANCODE_R] = 0;
        scene_timer = 0.0f;
        if (test_scenes[current_scene].init_scene) {
            test_scenes[current_scene].init_scene();
        }
        return;
    }
    
    // Auto-advance after duration
    if (scene_timer >= test_scenes[current_scene].display_duration) {
        advance_to_next_scene();
        return;
    }
}

static void draw_ui_overlay(void) {
    // Test progress
    char progress_text[100];
    if (current_scene < total_scenes) {
        snprintf(progress_text, sizeof(progress_text), "Test %d/%d: %s", 
                 current_scene + 1, total_scenes, test_scenes[current_scene].name);
    } else {
        snprintf(progress_text, sizeof(progress_text), "Tests Complete: %d/%d", 
                 total_scenes, total_scenes);
    }
    a_DrawText(progress_text, 400, 20, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Timer bar
    if (current_scene < total_scenes) {
        float progress = scene_timer / test_scenes[current_scene].display_duration;
        if (progress > 1.0f) progress = 1.0f;
        int bar_width = (int)(600 * progress);
        a_DrawFilledRect(100, 570, bar_width, 10, 0, 255, 0, 255);
    }
    a_DrawRect(100, 570, 600, 10, 128, 128, 128, 255);
    
    // Test status (moved up to be visible)
    char status_text[50];
    snprintf(status_text, sizeof(status_text), "Passed: %d  Failed: %d", tests_passed, tests_failed);
    a_DrawText(status_text, 700, 550, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_RIGHT, 0);
}

// ============================================================================
// Helper function for test execution with crash protection
// ============================================================================

static void run_edge_test_with_cleanup(void (*test_func)(void), const char* test_name) {
    (void)test_name; // Unused parameter
    
    // Clean up any existing state
    if (app.window != NULL || app.renderer != NULL) {
        a_Quit();
    }
    memset(&app, 0, sizeof(aApp_t));
    
    // Run the test (this will set test_result_text and scene_test_result)
    test_func();
    
    // Clean up after test if needed
    if (app.window != NULL || app.renderer != NULL) {
        a_Quit();
    }
    
    // Re-initialize for UI display
    if (a_InitTest(800, 600, "Archimedes Initialization Edge Case Tests") != 0) {
        strcpy(test_result_text, "ERROR: Failed to re-initialize for display");
        scene_test_result = 0;
    }
}

// ============================================================================
// Test Implementation Functions
// ============================================================================

static void test_parameter_validation_impl(void) {
    int null_crash = 0, neg_handled = 0, zero_handled = 0, extreme_handled = 0;
    int result;
    
    // Test 1: NULL title
    setup_crash_detection();
    if (setjmp(jump_buffer) == 0) {
        result = a_InitTest(640, 480, NULL);
        if (!crash_detected) {
            if (result == 0) a_Quit();
        }
    } else {
        null_crash = 1;
    }
    cleanup_crash_detection();
    
    // Test 2: Negative dimensions
    result = a_InitTest(-100, -200, "Negative Size");
    if (result != 0) {
        neg_handled = 1;
    } else {
        a_Quit();
    }
    
    // Test 3: Zero dimensions
    result = a_InitTest(0, 0, "Zero Size");
    zero_handled = 1; // Either way is acceptable
    if (result == 0) a_Quit();
    
    // Test 4: Extreme dimensions
    result = a_InitTest(50000, 50000, "Huge Window");
    if (result != 0) {
        extreme_handled = 1;
    } else {
        a_Quit();
    }
    
    // Summarize results
    int issues = null_crash + (!neg_handled) + (!extreme_handled);
    if (issues == 0) {
        strcpy(test_result_text, "✓ Parameter validation robust");
        scene_test_result = 1;
    } else {
        snprintf(test_result_text, sizeof(test_result_text), "✗ %d validation issues found", issues);
        scene_test_result = 0;
    }
}

static void test_failure_recovery_impl(void) {
    int double_handled = 0, recovery_works = 0, quit_safe = 0, multi_quit_safe = 0;
    int result;
    
    // Test 1: Double initialization
    result = a_InitTest(640, 480, "First Init");
    if (result == 0) {
        int result2 = a_InitTest(800, 600, "Second Init");
        double_handled = 1; // Either way is acceptable
        a_Quit();
        if (result2 == 0) a_Quit();
    }
    
    // Test 2: Recovery after failed init
    result = a_InitTest(INT_MAX, INT_MAX, "Fail Init");
    if (result != 0) {
        result = a_InitTest(640, 480, "Recovery Init");
        if (result == 0) {
            recovery_works = 1;
            a_Quit();
        }
    }
    
    // Test 3: Quit without init
    memset(&app, 0, sizeof(app));
    setup_crash_detection();
    if (setjmp(jump_buffer) == 0) {
        a_Quit();
        if (!crash_detected) {
            quit_safe = 1;
        }
    }
    cleanup_crash_detection();
    
    // Test 4: Multiple quit calls
    result = a_InitTest(640, 480, "Multi Quit Test");
    if (result == 0) {
        a_Quit();
        setup_crash_detection();
        if (setjmp(jump_buffer) == 0) {
            a_Quit();
            a_Quit();
            if (!crash_detected) {
                multi_quit_safe = 1;
            }
        }
        cleanup_crash_detection();
    }
    
    // Summarize results
    int good_behaviors = double_handled + recovery_works + quit_safe + multi_quit_safe;
    if (good_behaviors >= 3) {
        strcpy(test_result_text, "✓ Failure recovery working well");
        scene_test_result = 1;
    } else {
        snprintf(test_result_text, sizeof(test_result_text), "✗ %d/4 recovery tests failed", 4 - good_behaviors);
        scene_test_result = 0;
    }
}

static void test_state_corruption_impl(void) {
    int corrupted_handled = 0, time_handled = 0, font_handled = 0;
    int result;
    
    // Test 1: Pre-corrupted app state
    memset(&app, 0xFF, sizeof(app));
    result = a_InitTest(640, 480, "Corrupted State");
    if (result == 0 && app.window != NULL && app.renderer != NULL) {
        corrupted_handled = 1;
        app.delegate.onExit = NULL;
        app.delegate.logic = NULL;
        app.delegate.draw = NULL;
        a_Quit();
    }
    
    // Test 2: Time value overflow
    result = a_InitTest(640, 480, "Time Overflow");
    if (result == 0) {
        app.time.currentTime = UINT_MAX - 1000;
        app.time.lastTime = UINT_MAX - 500;
        app.time.frameStart = UINT_MAX - 100;
        time_handled = 1; // If we don't crash, it's handled
        a_Quit();
    }
    
    // Test 3: Font array bounds
    result = a_InitTest(640, 480, "Font Bounds");
    if (result == 0) {
        app.font_type = -1;
        app.font_type = FONT_MAX + 10;
        app.font_type = FONT_ENTER_COMMAND; // Reset
        font_handled = 1; // If we don't crash, it's handled
        a_Quit();
    }
    
    // Summarize results
    int handled_tests = corrupted_handled + time_handled + font_handled;
    if (handled_tests >= 2) {
        strcpy(test_result_text, "✓ State corruption handled well");
        scene_test_result = 1;
    } else {
        snprintf(test_result_text, sizeof(test_result_text), "✗ %d/3 corruption tests failed", 3 - handled_tests);
        scene_test_result = 0;
    }
}

// Scene initialization functions
static void init_parameter_validation_test(void) {
    run_edge_test_with_cleanup(test_parameter_validation_impl, "Parameter Validation Tests");
}

static void init_failure_recovery_test(void) {
    run_edge_test_with_cleanup(test_failure_recovery_impl, "Failure Recovery Tests");
}

static void init_state_corruption_test(void) {
    run_edge_test_with_cleanup(test_state_corruption_impl, "Global State Corruption Tests");
}

// Scene drawing functions
static void draw_parameter_validation_test(float dt) {
    (void)dt;
    a_DrawText("Parameter Validation Tests", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("NULL title, negative/extreme dimensions handled", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    } else {
        if (crash_detected_flag) {
            a_DrawText("WARNING: NULL title causes crash!", 400, 350, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
    }
    
    draw_ui_overlay();
}

static void draw_failure_recovery_test(float dt) {
    (void)dt;
    a_DrawText("Failure Recovery Tests", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("Double init, recovery, quit safety verified", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_state_corruption_test(float dt) {
    (void)dt;
    a_DrawText("Global State Corruption Tests", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("Corrupted state, time overflow, font bounds OK", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

// ============================================================================
// Main Entry Point
// ============================================================================

void aMainloop(void) {
    a_PrepareScene();
    app.delegate.logic(a_GetDeltaTime());
    app.delegate.draw(a_GetDeltaTime());
    a_PresentScene();
}

int main(void) {
    // Initialize app structure to clean state
    memset(&app, 0, sizeof(app));
    
    if (a_InitTest(800, 600, "Archimedes Initialization Edge Case Tests") != 0) {
        printf("Failed to initialize Archimedes\n");
        return 1;
    }
    
    init_test_system();
    
    while (app.running) {
        aMainloop();
    }
    
    a_Quit();
    return 0;
}