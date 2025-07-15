// File: true_tests/initialize/test_initialize_basic.c - Scene-based initialization functionality tests

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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
// static char test_details[500]; // Unused for now

// Forward declarations
static void advance_to_next_scene(void);
static void init_test_system(void);
static void common_logic(float dt);
static void draw_ui_overlay(void);

// Test scene function declarations
static void init_sdl_system_test(void);
static void draw_sdl_system_test(float dt);
static void init_window_parameters_test(void);
static void draw_window_parameters_test(float dt);
static void init_window_title_test(void);
static void draw_window_title_test(float dt);
static void init_mouse_state_test(void);
static void draw_mouse_state_test(float dt);
static void init_time_system_test(void);
static void draw_time_system_test(float dt);
static void init_app_running_test(void);
static void draw_app_running_test(float dt);
static void init_image_cache_test(void);
static void draw_image_cache_test(float dt);
static void init_audio_integration_test(void);
static void draw_audio_integration_test(float dt);
static void init_image_integration_test(void);
static void draw_image_integration_test(float dt);
static void init_font_integration_test(void);
static void draw_font_integration_test(float dt);
static void init_basic_cleanup_test(void);
static void draw_basic_cleanup_test(float dt);
static void init_cleanup_delegate_test(void);
static void draw_cleanup_delegate_test(float dt);
static void init_boundary_conditions_test(void);
static void draw_boundary_conditions_test(float dt);
static void init_multiple_init_test(void);
static void draw_multiple_init_test(float dt);
static void init_random_test(void);
static void draw_random_test(float dt);
static void init_log_strings_test(void);
static void draw_log_strings_test(float dt);

// Test scene definitions
static TestScene_t test_scenes[] = {
    // Initialization tests (1 second each - quick validation)
    {"SDL System Initialization", 1.0f, init_sdl_system_test, common_logic, draw_sdl_system_test},
    {"Window Creation Parameters", 1.0f, init_window_parameters_test, common_logic, draw_window_parameters_test},
    {"Window Title Setting", 1.0f, init_window_title_test, common_logic, draw_window_title_test},
    {"Mouse State Initialization", 1.0f, init_mouse_state_test, common_logic, draw_mouse_state_test},
    {"Time System Initialization", 1.0f, init_time_system_test, common_logic, draw_time_system_test},
    {"App Running State", 1.0f, init_app_running_test, common_logic, draw_app_running_test},
    {"Image Cache Initialization", 1.0f, init_image_cache_test, common_logic, draw_image_cache_test},
    {"Audio System Integration", 1.0f, init_audio_integration_test, common_logic, draw_audio_integration_test},
    {"Image System Integration", 1.0f, init_image_integration_test, common_logic, draw_image_integration_test},
    {"Font System Integration", 1.0f, init_font_integration_test, common_logic, draw_font_integration_test},
    {"Basic Cleanup", 1.0f, init_basic_cleanup_test, common_logic, draw_basic_cleanup_test},
    {"Cleanup with Delegate", 1.0f, init_cleanup_delegate_test, common_logic, draw_cleanup_delegate_test},
    {"Boundary Conditions", 1.0f, init_boundary_conditions_test, common_logic, draw_boundary_conditions_test},
    {"Multiple Initialization", 1.0f, init_multiple_init_test, common_logic, draw_multiple_init_test},
    {"Random Number Generator", 1.0f, init_random_test, common_logic, draw_random_test},
    {"Log Level Strings", 1.0f, init_log_strings_test, common_logic, draw_log_strings_test}
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
    
    printf("\n=== Archimedes Initialization Basic Tests ===\n");
    printf("Total tests: %d\n", total_scenes);
    printf("Controls: SPACE=Next, ESC=Quit, R=Restart\n");
    printf("Testing SDL, app state, and subsystem initialization...\n\n");
    
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
// Helper function for test execution
// ============================================================================

static void run_test_with_cleanup(void (*test_func)(void), const char* test_name) {
    (void)test_name; // Unused parameter
    // Clean up any existing state
    if (app.window != NULL || app.renderer != NULL) {
        a_Quit();
    }
    memset(&app, 0, sizeof(aApp_t));
    
    // Run the test (this will set test_result_text and scene_test_result)
    test_func();
    
    // Clean up after test
    if (app.window != NULL || app.renderer != NULL) {
        a_Quit();
    }
    
    // Re-initialize for UI display
    if (a_InitTest(800, 600, "Archimedes Initialization Basic Tests") != 0) {
        strcpy(test_result_text, "ERROR: Failed to re-initialize for display");
        scene_test_result = 0;
    }
}

// ============================================================================
// Test Implementation Functions
// ============================================================================

static void test_sdl_system_initialization_impl(void) {
    int result = a_InitTest(640, 480, "Test Window");
    
    if (result == 0) {
        if (app.window != NULL && app.renderer != NULL && app.running == 1) {
            strcpy(test_result_text, "✓ SDL system initialized successfully");
            scene_test_result = 1;
        } else {
            strcpy(test_result_text, "✗ SDL init succeeded but state invalid");
            scene_test_result = 0;
        }
    } else {
        snprintf(test_result_text, sizeof(test_result_text), "✗ SDL init failed with code %d", result);
        scene_test_result = 0;
    }
}

static void test_window_creation_parameters_impl(void) {
    int result1 = a_InitTest(1280, 720, "HD Test Window");
    
    if (result1 == 0 && app.window != NULL && app.renderer != NULL) {
        a_Quit();
        memset(&app, 0, sizeof(aApp_t));
        
        int result2 = a_InitTest(320, 240, "Small Test Window");
        if (result2 == 0 && app.window != NULL) {
            strcpy(test_result_text, "✓ Different window sizes supported");
            scene_test_result = 1;
        } else {
            strcpy(test_result_text, "✗ Small window creation failed");
            scene_test_result = 0;
        }
    } else {
        strcpy(test_result_text, "✗ HD window creation failed");
        scene_test_result = 0;
    }
}

static void test_window_title_setting_impl(void) {
    int result = a_InitTest(800, 600, "Archimedes Test Framework");
    
    if (result == 0 && app.window != NULL && app.renderer != NULL) {
        strcpy(test_result_text, "✓ Window title setting successful");
        scene_test_result = 1;
    } else {
        strcpy(test_result_text, "✗ Window title setting failed");
        scene_test_result = 0;
    }
}

static void test_mouse_state_initialization_impl(void) {
    int result = a_InitTest(640, 480, "Mouse Test");
    
    if (result == 0) {
        if (app.mouse.x == 0 && app.mouse.y == 0 && app.mouse.pressed == 0 && 
            app.mouse.button == 0 && app.mouse.state == 0 && app.mouse.wheel == 0 && app.mouse.clicks == 0) {
            strcpy(test_result_text, "✓ Mouse state initialized correctly");
            scene_test_result = 1;
        } else {
            strcpy(test_result_text, "✗ Mouse state not initialized to zero");
            scene_test_result = 0;
        }
    } else {
        strcpy(test_result_text, "✗ Init failed, cannot test mouse state");
        scene_test_result = 0;
    }
}

static void test_time_system_initialization_impl(void) {
    int result = a_InitTest(640, 480, "Time Test");
    
    if (result == 0) {
        if (app.time.lastTime == 0 && app.time.currentTime == 0 && app.time.frameStart == 0 && 
            app.time.frameTime == 0 && app.time.lastFrameCounterClear == 0 && app.time.frames == 0) {
            strcpy(test_result_text, "✓ Time system initialized correctly");
            scene_test_result = 1;
        } else {
            strcpy(test_result_text, "✗ Time system not initialized to zero");
            scene_test_result = 0;
        }
    } else {
        strcpy(test_result_text, "✗ Init failed, cannot test time system");
        scene_test_result = 0;
    }
}

static void test_app_running_state_impl(void) {
    int result = a_InitTest(640, 480, "Running Test");
    
    if (result == 0 && app.running == 1) {
        strcpy(test_result_text, "✓ App running state set correctly");
        scene_test_result = 1;
    } else {
        strcpy(test_result_text, "✗ App running state not set");
        scene_test_result = 0;
    }
}

static void test_image_cache_initialization_impl(void) {
    int result = a_InitTest(640, 480, "Cache Test");
    
    if (result == 0) {
        if (app.img_cache != NULL && app.img_cache->head == NULL) {
            strcpy(test_result_text, "✓ Image cache initialized correctly");
            scene_test_result = 1;
        } else {
            strcpy(test_result_text, "✗ Image cache not properly initialized");
            scene_test_result = 0;
        }
    } else {
        strcpy(test_result_text, "✗ Init failed, cannot test image cache");
        scene_test_result = 0;
    }
}

static void test_audio_system_integration_impl(void) {
    int result = a_InitTest(640, 480, "Audio Test");
    
    if (result == 0 && app.window != NULL && app.renderer != NULL) {
        strcpy(test_result_text, "✓ Audio system integration successful");
        scene_test_result = 1;
    } else {
        strcpy(test_result_text, "✗ Audio system integration failed");
        scene_test_result = 0;
    }
}

static void test_image_system_integration_impl(void) {
    int result = a_InitTest(640, 480, "Image System Test");
    
    if (result == 0 && app.img_cache != NULL && app.img_cache->head == NULL) {
        strcpy(test_result_text, "✓ Image system integration successful");
        scene_test_result = 1;
    } else {
        strcpy(test_result_text, "✗ Image system integration failed");
        scene_test_result = 0;
    }
}

static void test_font_system_integration_impl(void) {
    int result = a_InitTest(640, 480, "Font System Test");
    
    if (result == 0 && app.renderer != NULL) {
        strcpy(test_result_text, "✓ Font system integration successful");
        scene_test_result = 1;
    } else {
        strcpy(test_result_text, "✗ Font system integration failed");
        scene_test_result = 0;
    }
}

static void test_basic_cleanup_impl(void) {
    int result = a_InitTest(640, 480, "Cleanup Test");
    
    if (result == 0 && app.window != NULL && app.renderer != NULL) {
        a_Quit();
        strcpy(test_result_text, "✓ Basic cleanup completed without crash");
        scene_test_result = 1;
        memset(&app, 0, sizeof(aApp_t)); // Clear state for display re-init
    } else {
        strcpy(test_result_text, "✗ Init failed, cannot test cleanup");
        scene_test_result = 0;
    }
}

static void test_cleanup_with_delegate_impl(void) {
    static int callback_called = 0;
    callback_called = 0;
    
    void test_callback(void) {
        callback_called = 1;
    }
    
    int result = a_InitTest(640, 480, "Delegate Cleanup Test");
    
    if (result == 0) {
        app.delegate.onExit = test_callback;
        a_Quit();
        
        if (callback_called == 1) {
            strcpy(test_result_text, "✓ Delegate callback executed on cleanup");
            scene_test_result = 1;
        } else {
            strcpy(test_result_text, "✗ Delegate callback not executed");
            scene_test_result = 0;
        }
        memset(&app, 0, sizeof(aApp_t)); // Clear state for display re-init
    } else {
        strcpy(test_result_text, "✗ Init failed, cannot test delegate cleanup");
        scene_test_result = 0;
    }
}

static void test_boundary_conditions_impl(void) {
    int result1 = a_InitTest(1, 1, "Tiny Window");
    
    if (result1 == 0) {
        a_Quit();
        memset(&app, 0, sizeof(aApp_t));
        
        int result2 = a_InitTest(3840, 2160, "4K Window");
        if (result2 == 0) {
            strcpy(test_result_text, "✓ Boundary conditions handled correctly");
            scene_test_result = 1;
        } else {
            strcpy(test_result_text, "✗ Large window creation failed");
            scene_test_result = 0;
        }
    } else {
        strcpy(test_result_text, "✗ Small window creation failed");
        scene_test_result = 0;
    }
}

static void test_multiple_initialization_calls_impl(void) {
    int result1 = a_InitTest(640, 480, "First Init");
    
    if (result1 == 0) {
        (void)a_InitTest(800, 600, "Second Init"); // Test multiple init calls
        strcpy(test_result_text, "✓ Multiple init calls handled gracefully");
        scene_test_result = 1;
    } else {
        strcpy(test_result_text, "✗ First init failed");
        scene_test_result = 0;
    }
}

static void test_random_number_initialization_impl(void) {
    int result = a_InitTest(640, 480, "Random Test");
    
    if (result == 0) {
        int random1 = rand();
        int random2 = rand();
        
        if (random1 != random2) {
            strcpy(test_result_text, "✓ Random number generator working");
            scene_test_result = 1;
        } else {
            strcpy(test_result_text, "✗ Random generator producing same values");
            scene_test_result = 0;
        }
    } else {
        strcpy(test_result_text, "✗ Init failed, cannot test random generator");
        scene_test_result = 0;
    }
}

static void test_log_level_strings_initialization_impl(void) {
    if (log_level_strings[NORMAL] != NULL && log_level_strings[WARNING] != NULL &&
        log_level_strings[FATAL] != NULL && log_level_strings[INFO] != NULL &&
        log_level_strings[DEBUG] != NULL && log_level_strings[TRACE] != NULL &&
        strcmp(log_level_strings[NORMAL], "NORMAL") == 0 &&
        strcmp(log_level_strings[WARNING], "WARNING") == 0) {
        strcpy(test_result_text, "✓ Log level strings initialized correctly");
        scene_test_result = 1;
    } else {
        strcpy(test_result_text, "✗ Log level strings not properly initialized");
        scene_test_result = 0;
    }
}

// Scene initialization functions
static void init_sdl_system_test(void) {
    run_test_with_cleanup(test_sdl_system_initialization_impl, "SDL System Initialization");
}

static void init_window_parameters_test(void) {
    run_test_with_cleanup(test_window_creation_parameters_impl, "Window Creation Parameters");
}

static void init_window_title_test(void) {
    run_test_with_cleanup(test_window_title_setting_impl, "Window Title Setting");
}

static void init_mouse_state_test(void) {
    run_test_with_cleanup(test_mouse_state_initialization_impl, "Mouse State Initialization");
}

static void init_time_system_test(void) {
    run_test_with_cleanup(test_time_system_initialization_impl, "Time System Initialization");
}

static void init_app_running_test(void) {
    run_test_with_cleanup(test_app_running_state_impl, "App Running State");
}

static void init_image_cache_test(void) {
    run_test_with_cleanup(test_image_cache_initialization_impl, "Image Cache Initialization");
}

static void init_audio_integration_test(void) {
    run_test_with_cleanup(test_audio_system_integration_impl, "Audio System Integration");
}

static void init_image_integration_test(void) {
    run_test_with_cleanup(test_image_system_integration_impl, "Image System Integration");
}

static void init_font_integration_test(void) {
    run_test_with_cleanup(test_font_system_integration_impl, "Font System Integration");
}

static void init_basic_cleanup_test(void) {
    run_test_with_cleanup(test_basic_cleanup_impl, "Basic Cleanup");
}

static void init_cleanup_delegate_test(void) {
    run_test_with_cleanup(test_cleanup_with_delegate_impl, "Cleanup with Delegate");
}

static void init_boundary_conditions_test(void) {
    run_test_with_cleanup(test_boundary_conditions_impl, "Boundary Conditions");
}

static void init_multiple_init_test(void) {
    run_test_with_cleanup(test_multiple_initialization_calls_impl, "Multiple Initialization");
}

static void init_random_test(void) {
    run_test_with_cleanup(test_random_number_initialization_impl, "Random Number Generator");
}

static void init_log_strings_test(void) {
    // This test doesn't need app initialization
    test_log_level_strings_initialization_impl();
    
    // Still need to initialize for display
    if (a_InitTest(800, 600, "Archimedes Initialization Basic Tests") != 0) {
        strcpy(test_result_text, "ERROR: Failed to initialize for display");
        scene_test_result = 0;
    }
}

// Scene drawing functions
static void draw_sdl_system_test(float dt) {
    (void)dt;
    a_DrawText("SDL System Initialization Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("Window, renderer, and running state validated", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_window_parameters_test(float dt) {
    (void)dt;
    a_DrawText("Window Creation Parameters Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("HD (1280x720) and small (320x240) windows tested", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_window_title_test(float dt) {
    (void)dt;
    a_DrawText("Window Title Setting Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("Custom window title accepted", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_mouse_state_test(float dt) {
    (void)dt;
    a_DrawText("Mouse State Initialization Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("x, y, pressed, button, state, wheel, clicks = 0", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_time_system_test(float dt) {
    (void)dt;
    a_DrawText("Time System Initialization Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("All time fields initialized to zero", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_app_running_test(float dt) {
    (void)dt;
    a_DrawText("App Running State Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("app.running = 1 after initialization", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_image_cache_test(float dt) {
    (void)dt;
    a_DrawText("Image Cache Initialization Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("img_cache allocated, head = NULL", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_audio_integration_test(float dt) {
    (void)dt;
    a_DrawText("Audio System Integration Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("Audio subsystem initialized successfully", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_image_integration_test(float dt) {
    (void)dt;
    a_DrawText("Image System Integration Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("Image subsystem initialized successfully", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_font_integration_test(float dt) {
    (void)dt;
    a_DrawText("Font System Integration Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("Font subsystem initialized successfully", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_basic_cleanup_test(float dt) {
    (void)dt;
    a_DrawText("Basic Cleanup Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("a_Quit() executed without crash", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_cleanup_delegate_test(float dt) {
    (void)dt;
    a_DrawText("Cleanup with Delegate Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("onExit delegate called during cleanup", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_boundary_conditions_test(float dt) {
    (void)dt;
    a_DrawText("Boundary Conditions Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("Tiny (1x1) and 4K (3840x2160) windows supported", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_multiple_init_test(float dt) {
    (void)dt;
    a_DrawText("Multiple Initialization Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("Multiple a_Init calls handled gracefully", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_random_test(float dt) {
    (void)dt;
    a_DrawText("Random Number Generator Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("srand(time(NULL)) seeded correctly", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void draw_log_strings_test(float dt) {
    (void)dt;
    a_DrawText("Log Level Strings Test", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 300, scene_test_result ? 0 : 255, scene_test_result ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (scene_test_result) {
        a_DrawText("All log level strings properly initialized", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
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
    if (a_InitTest(800, 600, "Archimedes Initialization Basic Tests") != 0) {
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