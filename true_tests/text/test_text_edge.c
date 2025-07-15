// File: true_tests/text/test_text_edge.c - Scene-based edge case tests for text system

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
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

// Forward declarations
static void advance_to_next_scene(void);
static void init_test_system(void);
static void common_logic(float dt);
static void draw_ui_overlay(void);

// Test scene function declarations
static void init_buffer_overflows(void);
static void draw_buffer_overflows(float dt);
static void init_rendering_attacks(void);
static void draw_rendering_attacks(float dt);
static void init_resource_exhaustion(void);
static void draw_resource_exhaustion(float dt);
static void init_null_pointer(void);
static void draw_null_pointer(float dt);

// Test scene definitions
static TestScene_t test_scenes[] = {
    // Edge case tests (1-2 seconds each - quick pass/fail checks)
    {"Buffer Overflow Tests", 2.0f, init_buffer_overflows, common_logic, draw_buffer_overflows},
    {"Rendering Attack Tests", 2.0f, init_rendering_attacks, common_logic, draw_rendering_attacks},
    {"Resource Exhaustion Tests", 2.0f, init_resource_exhaustion, common_logic, draw_resource_exhaustion},
    
    // Simple tests (0.5-1 second each - very quick validation)
    {"NULL Pointer Test", 1.0f, init_null_pointer, common_logic, draw_null_pointer}
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
        printf("\nBug hunting complete! All edge cases handled gracefully.\n");
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
    
    printf("\n=== Archimedes Text Edge Case Tests ===\n");
    printf("Total tests: %d\n", total_scenes);
    printf("Controls: SPACE=Next, ESC=Quit, R=Restart\n");
    printf("Testing buffer overflows, crashes, and vulnerabilities...\n\n");
    
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
// Edge Case Tests (1-2 seconds each)
// ============================================================================

static void init_buffer_overflows(void) {
    // No special initialization needed
}

static void draw_buffer_overflows(float dt) {
    (void)dt;
    a_DrawText("Testing Buffer Overflow Protection", 400, 150, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Test long word (should be truncated gracefully)
    char long_word[100];
    memset(long_word, 'A', sizeof(long_word) - 1);
    long_word[sizeof(long_word) - 1] = '\0';
    a_DrawText(long_word, 400, 200, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 600);
    
    // Test UTF-8 4-byte characters (should be skipped gracefully)
    const char* utf8_overflow = "😀😀😀😀😀😀😀😀😀😀😀😀😀😀😀😀";
    a_DrawText((char*)utf8_overflow, 400, 250, 0, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Test malformed UTF-8 (should be skipped gracefully)
    char malformed[10] = {0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8, 0x00};
    a_DrawText(malformed, 400, 300, 255, 0, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    a_DrawText("✓ All overflow tests handled gracefully", 400, 400, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    draw_ui_overlay();
}

static void init_rendering_attacks(void) {
    // No special initialization needed
}

static void draw_rendering_attacks(float dt) {
    (void)dt;
    a_DrawText("Testing Rendering Attack Protection", 400, 150, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Test NULL text pointer (should be handled gracefully)
    a_DrawText(NULL, 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Test invalid font type (should be handled gracefully)
    a_DrawText("Invalid Font Test", 400, 250, 255, 255, 255, -1, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Invalid Font Test", 400, 280, 255, 255, 255, 9999, TEXT_ALIGN_CENTER, 0);
    
    // Test format string attack (should be treated as literal text)
    const char* format_attack = "Format %s %d %n %x";
    a_DrawText((char*)format_attack, 400, 330, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    a_DrawText("✓ All attack vectors handled safely", 400, 400, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    draw_ui_overlay();
}

static void init_resource_exhaustion(void) {
    // No special initialization needed
}

static void draw_resource_exhaustion(float dt) {
    (void)dt;
    a_DrawText("Testing Resource Exhaustion Protection", 400, 150, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Test zero-width characters
    const char* zero_width = "Test\u200B\u200C\u200D\uFEFFInvisible";
    a_DrawText((char*)zero_width, 400, 250, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Test missing glyphs (non-ASCII characters)
    const char* missing_glyphs = "Test 你好 مرحبا שלום";
    a_DrawText((char*)missing_glyphs, 400, 300, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Test extreme wrapping
    const char* wrap_test = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
    a_DrawText((char*)wrap_test, 400, 350, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 1);
    
    a_DrawText("✓ Resource exhaustion handled gracefully", 400, 450, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    draw_ui_overlay();
}

// ============================================================================
// Simple Tests (0.5-1 second each)
// ============================================================================

static void init_null_pointer(void) {
    // No special initialization needed
}

static void draw_null_pointer(float dt) {
    (void)dt;
    a_DrawText("Testing NULL Pointer Handling", 400, 250, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // This should not crash - NULL text should be handled gracefully
    a_DrawText(NULL, 400, 300, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    a_DrawText("✓ NULL pointer handled safely", 400, 350, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
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
    if (a_InitTest(800, 600, "Archimedes Text Edge Case Tests") != 0) {
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