// File: true_tests/initialize/test_init_simple.c - Scene-based simple initialization test

#include <stdio.h>
#include <string.h>
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
    (void)sig;
    crash_detected = 1;
    crash_detected_flag = 1;
    longjmp(jump_buffer, 1);
}

// Forward declarations
static void advance_to_next_scene(void);
static void init_test_system(void);
static void common_logic(float dt);
static void draw_ui_overlay(void);

// Test scene function declarations
static void init_null_title_test(void);
static void draw_null_title_test(float dt);

// Test scene definitions
static TestScene_t test_scenes[] = {
    // Simple tests (1 second each - very quick validation)
    {"NULL Title Test", 1.0f, init_null_title_test, common_logic, draw_null_title_test}
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
    
    printf("\n=== Archimedes Simple Initialization Test ===\n");
    printf("Total tests: %d\n", total_scenes);
    printf("Controls: SPACE=Next, ESC=Quit, R=Restart\n");
    printf("Testing NULL title parameter crash...\n\n");
    
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
// Test Implementation Functions
// ============================================================================

static void test_null_title_impl(void) {
    // Clean up any existing state
    if (app.window != NULL || app.renderer != NULL) {
        a_Quit();
    }
    memset(&app, 0, sizeof(aApp_t));
    
    signal(SIGSEGV, segfault_handler);
    signal(SIGABRT, segfault_handler);
    
    crash_detected = 0;
    crash_detected_flag = 0;
    
    if (setjmp(jump_buffer) == 0) {
        int result = a_InitTest(640, 480, NULL);
        if (!crash_detected) {
            if (result == 0) {
                strcpy(test_result_text, "✓ NULL title handled gracefully");
                scene_test_result = 1;
                a_Quit();
            } else {
                strcpy(test_result_text, "✓ NULL title rejected safely");
                scene_test_result = 1;
            }
        }
    } else {
        strcpy(test_result_text, "✗ BUG: NULL title causes crash!");
        scene_test_result = 0;
    }
    
    signal(SIGSEGV, SIG_DFL);
    signal(SIGABRT, SIG_DFL);
    
    // Re-initialize for UI display
    if (a_InitTest(800, 600, "Archimedes Simple Initialization Test") != 0) {
        strcpy(test_result_text, "ERROR: Failed to re-initialize for display");
        scene_test_result = 0;
    }
}

// Scene initialization functions
static void init_null_title_test(void) {
    test_null_title_impl();
}

// Scene drawing functions
static void draw_null_title_test(float dt) {
    (void)dt;
    a_DrawText("NULL Title Parameter Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("NULL title parameter handled safely", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    } else {
        a_DrawText("WARNING: NULL title causes segmentation fault!", 400, 350, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
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
    if (a_InitTest(800, 600, "Archimedes Simple Initialization Test") != 0) {
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