/**
 * @file emscripten_test_base.c
 * @brief Common base infrastructure for Emscripten-based tests
 * 
 * This file provides the foundational functionality for running Archimedes
 * tests in a web browser environment using Emscripten. It handles:
 * - Web-optimized event handling
 * - Arrow key navigation between test scenes
 * - Integration with the HTML template interface
 * - Scene management adapted for web execution
 * - Canvas and WebGL context management
 * 
 * @author Archimedes Team
 * @date 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "Archimedes.h"

// ============================================================================
// Global Web Test State
// ============================================================================

// Test scene management
typedef struct {
    const char* name;
    const char* description;
    float display_duration;  // Seconds to display this test
    void (*init_scene)(void);
    void (*logic)(float dt);
    void (*draw)(float dt);
} WebTestScene_t;

// Global test state
static int current_scene = 0;
static float scene_timer = 0.0f;
static int total_scenes = 0;
static int tests_passed = 0;
static int tests_failed = 0;
static int scene_test_result = 1; // 1 = pass, 0 = fail
static int auto_advance = 0;
static int web_initialized = 0;

// Per-key debouncing to prevent double-movement
static float key_timers[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // Left, Right, Up, Down
static const float KEY_DEBOUNCE_TIME = 0.3f; // 300ms debounce for web environment

// Scene management
static WebTestScene_t* test_scenes = NULL;
static char current_test_title[256] = "Archimedes Test";
static char current_test_description[512] = "Initializing...";

// ============================================================================
// Web Integration Functions
// ============================================================================

/**
 * @brief Updates the web UI with current scene information
 * 
 * Calls JavaScript functions to update the HTML interface with:
 * - Current scene number and total scenes
 * - Test title and description
 * - Progress bar
 */
static void update_web_ui(void) {
#ifdef __EMSCRIPTEN__
    // Update scene counter
    char scene_update[128];
    snprintf(scene_update, sizeof(scene_update), 
             "emscripten_update_scene(%d, %d)", current_scene, total_scenes);
    emscripten_run_script(scene_update);
    
    // Update test title
    char title_update[512];
    snprintf(title_update, sizeof(title_update), 
             "emscripten_update_test_title('%s')", current_test_title);
    emscripten_run_script(title_update);
    
    // Update test description
    char desc_update[1024];
    snprintf(desc_update, sizeof(desc_update), 
             "emscripten_update_test_description('%s')", current_test_description);
    emscripten_run_script(desc_update);
#endif
}

/**
 * @brief Signals test pass to web UI
 */
static void signal_test_passed(void) {
#ifdef __EMSCRIPTEN__
    emscripten_run_script("emscripten_test_passed()");
#endif
}

/**
 * @brief Signals test failure to web UI
 */
static void signal_test_failed(void) {
#ifdef __EMSCRIPTEN__
    emscripten_run_script("emscripten_test_failed()");
#endif
}

// ============================================================================
// Scene Management Functions
// ============================================================================

/**
 * @brief Advances to the next test scene
 */
static void advance_to_next_scene(void) {
    // Check if we're already at the last scene
    if (current_scene >= total_scenes - 1) {
        // Already at the last scene, don't advance further
        printf("Already at last scene %d/%d\n", current_scene + 1, total_scenes);
        return;
    }
    
    if (scene_test_result) {
        tests_passed++;
        signal_test_passed();
    } else {
        tests_failed++;
        signal_test_failed();
    }
    
    current_scene++;
    scene_timer = 0.0f;
    scene_test_result = 1; // Reset for next test
    
    // Initialize next scene
    printf("Starting web test %d/%d: %s\n", current_scene + 1, total_scenes, test_scenes[current_scene].name);
    
    // Update UI with new scene info
    strcpy(current_test_title, test_scenes[current_scene].name);
    strcpy(current_test_description, test_scenes[current_scene].description);
    update_web_ui();
    
    if (test_scenes[current_scene].init_scene) {
        test_scenes[current_scene].init_scene();
    }
    
    // Update delegates
    app.delegate.logic = test_scenes[current_scene].logic;
    app.delegate.draw = test_scenes[current_scene].draw;
}

/**
 * @brief Goes back to the previous test scene
 */
static void go_to_previous_scene(void) {
    if (current_scene > 0) {
        current_scene--;
        scene_timer = 0.0f;
        scene_test_result = 1;
        
        printf("Going back to web test %d/%d: %s\n", current_scene + 1, total_scenes, test_scenes[current_scene].name);
        
        // Update UI
        strcpy(current_test_title, test_scenes[current_scene].name);
        strcpy(current_test_description, test_scenes[current_scene].description);
        update_web_ui();
        
        if (test_scenes[current_scene].init_scene) {
            test_scenes[current_scene].init_scene();
        }
        
        // Update delegates
        app.delegate.logic = test_scenes[current_scene].logic;
        app.delegate.draw = test_scenes[current_scene].draw;
    } else {
        // Already at first scene, don't go back further
        printf("Already at first scene %d/%d\n", current_scene + 1, total_scenes);
    }
}

/**
 * @brief Restarts the current test scene
 */
static void restart_current_scene(void) {
    scene_timer = 0.0f;
    scene_test_result = 1;
    
    printf("Restarting web test %d/%d: %s\n", current_scene + 1, total_scenes, test_scenes[current_scene].name);
    
    if (test_scenes[current_scene].init_scene) {
        test_scenes[current_scene].init_scene();
    }
}

// ============================================================================
// Web-Optimized Input Handling
// ============================================================================

/**
 * @brief Handles web-specific input logic
 * 
 * Processes keyboard input optimized for web browser environment:
 * - Arrow key navigation between scenes
 * - Auto-advance toggle
 * - Scene restart
 * - Exit handling
 * - Per-key debouncing to prevent double-movement
 */
static void web_input_logic(float dt) {
    scene_timer += dt;
    
    // Update all key timers
    for (int i = 0; i < 4; i++) {
        key_timers[i] += dt;
    }
    
    // Handle arrow key navigation with per-key debouncing
    // Left arrow (index 0)
    if (app.keyboard[SDL_SCANCODE_LEFT] == 1 && key_timers[0] >= KEY_DEBOUNCE_TIME) {
        app.keyboard[SDL_SCANCODE_LEFT] = 0;
        key_timers[0] = 0.0f; // Reset only this key's timer
        go_to_previous_scene();
        return;
    }
    
    // Right arrow (index 1)
    if (app.keyboard[SDL_SCANCODE_RIGHT] == 1 && key_timers[1] >= KEY_DEBOUNCE_TIME) {
        app.keyboard[SDL_SCANCODE_RIGHT] = 0;
        key_timers[1] = 0.0f; // Reset only this key's timer
        advance_to_next_scene();
        return;
    }
    
    // Up arrow (index 2)
    if (app.keyboard[SDL_SCANCODE_UP] == 1 && key_timers[2] >= KEY_DEBOUNCE_TIME) {
        app.keyboard[SDL_SCANCODE_UP] = 0;
        key_timers[2] = 0.0f; // Reset only this key's timer
        restart_current_scene();
        return;
    }
    
    // Down arrow (index 3)
    if (app.keyboard[SDL_SCANCODE_DOWN] == 1 && key_timers[3] >= KEY_DEBOUNCE_TIME) {
        app.keyboard[SDL_SCANCODE_DOWN] = 0;
        key_timers[3] = 0.0f; // Reset only this key's timer
        auto_advance = !auto_advance;
        printf("Auto-advance: %s\n", auto_advance ? "ON" : "OFF");
        return;
    }
    
    // Handle other controls
    if (app.keyboard[SDL_SCANCODE_ESCAPE] == 1) {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        app.running = 0;
        return;
    }
    
    if (app.keyboard[SDL_SCANCODE_R] == 1) {
        app.keyboard[SDL_SCANCODE_R] = 0;
        restart_current_scene();
        return;
    }
    
    if (app.keyboard[SDL_SCANCODE_SPACE] == 1) {
        app.keyboard[SDL_SCANCODE_SPACE] = 0;
        advance_to_next_scene();
        return;
    }
    
    // Auto-advance logic
    if (auto_advance && current_scene < total_scenes) {
        if (scene_timer >= test_scenes[current_scene].display_duration) {
            advance_to_next_scene();
            return;
        }
    }
    
    // Manual advance based on timer (for non-auto mode)
    if (!auto_advance && current_scene < total_scenes) {
        // Allow manual override - don't auto-advance
        // User must use arrow keys or space to advance
    }
}

// ============================================================================
// JavaScript-Callable Functions
// ============================================================================

#ifdef __EMSCRIPTEN__
/**
 * @brief Called by JavaScript to advance to next scene
 */
EMSCRIPTEN_KEEPALIVE
void emscripten_next_scene(void) {
    advance_to_next_scene();
}

/**
 * @brief Called by JavaScript to go to previous scene
 */
EMSCRIPTEN_KEEPALIVE
void emscripten_previous_scene(void) {
    go_to_previous_scene();
}

/**
 * @brief Called by JavaScript to restart current scene
 */
EMSCRIPTEN_KEEPALIVE
void emscripten_restart_scene(void) {
    restart_current_scene();
}

/**
 * @brief Called by JavaScript to toggle auto-advance
 */
EMSCRIPTEN_KEEPALIVE
void emscripten_toggle_auto_advance(void) {
    auto_advance = !auto_advance;
    printf("Auto-advance: %s\n", auto_advance ? "ON" : "OFF");
}
#endif

// ============================================================================
// Common UI Drawing Functions
// ============================================================================

/**
 * @brief Draws a common UI overlay for web tests
 * 
 * Provides consistent UI elements across all test scenes:
 * - Test progress indicator
 * - Timer bar
 * - Status information
 * - Web-specific controls hint
 */
static void draw_web_ui_overlay(float dt) {
    (void)dt;
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
    
    // Web-specific controls hint (moved above progress bar)
    a_DrawText("Use arrow keys to navigate • Click or tap to advance", 400, 545, 150, 150, 150, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Test status (moved down to make room for controls)
    char status_text[50];
    snprintf(status_text, sizeof(status_text), "Passed: %d  Failed: %d", tests_passed, tests_failed);
    a_DrawText(status_text, 700, 565, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_RIGHT, 0);
    
    // Auto-advance indicator
    if (auto_advance) {
        a_DrawText("AUTO", 50, 565, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    }
    
    // Timer bar (moved down to improve layout hierarchy)
    if (auto_advance && current_scene < total_scenes) {
        float progress = scene_timer / test_scenes[current_scene].display_duration;
        if (progress > 1.0f) progress = 1.0f;
        int bar_width = (int)(600 * progress);
        a_DrawFilledRect(100, 580, bar_width, 10, 0, 255, 0, 255);
    }
    a_DrawRect(100, 580, 600, 10, 128, 128, 128, 255);
}

// ============================================================================
// Public API Functions
// ============================================================================

/**
 * @brief Initializes the web test system
 * 
 * @param scenes Array of test scenes to execute
 * @param scene_count Number of scenes in the array
 * @param title Test suite title
 * @param description Test suite description
 * @return 0 on success, -1 on failure
 */
int web_test_init(WebTestScene_t* scenes, int scene_count, const char* title, const char* description) {
    if (!scenes || scene_count <= 0) {
        printf("Error: Invalid scene array or count\n");
        return -1;
    }
    
    test_scenes = scenes;
    total_scenes = scene_count;
    current_scene = 0;
    scene_timer = 0.0f;
    tests_passed = 0;
    tests_failed = 0;
    scene_test_result = 1;
    auto_advance = 0; // Disabled by default - manual navigation preferred
    
    // Set initial title and description
    if (title) {
        strncpy(current_test_title, title, sizeof(current_test_title) - 1);
        current_test_title[sizeof(current_test_title) - 1] = '\0';
    }
    
    if (description) {
        strncpy(current_test_description, description, sizeof(current_test_description) - 1);
        current_test_description[sizeof(current_test_description) - 1] = '\0';
    }
    
    printf("\n=== Web Test Suite Initialized ===\n");
    printf("Title: %s\n", current_test_title);
    printf("Description: %s\n", current_test_description);
    printf("Total scenes: %d\n", total_scenes);
    printf("Controls: Arrow keys for navigation, Space to advance\n\n");
    
    // Initialize first scene
    if (test_scenes[0].init_scene) {
        test_scenes[0].init_scene();
    }
    
    // Set up delegates - start with the first scene
    app.delegate.logic = test_scenes[0].logic;
    app.delegate.draw = test_scenes[0].draw;
    
    // Update web UI
    update_web_ui();
    
    web_initialized = 1;
    return 0;
}

/**
 * @brief Gets the current scene index
 * @return Current scene index (0-based)
 */
int web_test_get_current_scene(void) {
    return current_scene;
}

/**
 * @brief Gets the total number of scenes
 * @return Total scene count
 */
int web_test_get_total_scenes(void) {
    return total_scenes;
}

/**
 * @brief Checks if web test system is initialized
 * @return 1 if initialized, 0 otherwise
 */
int web_test_is_initialized(void) {
    return web_initialized;
}

/**
 * @brief Gets the web-optimized input logic function
 * @return Pointer to web input logic function
 */
void (*web_test_get_input_logic(void))(float) {
    return web_input_logic;
}

/**
 * @brief Gets the web UI overlay drawing function
 * @return Pointer to web UI drawing function
 */
void (*web_test_get_ui_overlay(void))(float) {
    return draw_web_ui_overlay;
}

/**
 * @brief Sets the current scene test result
 * @param passed 1 if test passed, 0 if failed
 */
void web_test_set_result(int passed) {
    scene_test_result = passed;
}

/**
 * @brief Updates the current test description
 * @param description New description text
 */
void web_test_update_description(const char* description) {
    if (description) {
        strncpy(current_test_description, description, sizeof(current_test_description) - 1);
        current_test_description[sizeof(current_test_description) - 1] = '\0';
        update_web_ui();
    }
}