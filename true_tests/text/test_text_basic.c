// File: true_tests/text/test_text_basic.c - Scene-based visual text rendering tests

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

// Forward declarations
static void advance_to_next_scene(void);
static void init_test_system(void);
static void common_logic(float dt);
static void draw_ui_overlay(void);

// Test scene function declarations
static void init_basic_rendering(void);
static void draw_basic_rendering(float dt);
static void init_text_alignment(void);
static void draw_text_alignment(float dt);
static void init_text_colors(void);
static void draw_text_colors(float dt);
static void init_font_types(void);
static void draw_font_types(float dt);
static void init_text_wrapping(void);
static void draw_text_wrapping(float dt);
static void init_text_dimensions(void);
static void draw_text_dimensions(float dt);
static void init_font_scaling(void);
static void draw_font_scaling(float dt);
static void init_special_characters(void);
static void draw_special_characters(float dt);
static void init_text_positioning(void);
static void draw_text_positioning(float dt);

// Test scene definitions
static TestScene_t test_scenes[] = {
    // Visual text tests (3 seconds each - users need to read and verify)
    {"Basic Text Rendering", 3.0f, init_basic_rendering, common_logic, draw_basic_rendering},
    {"Text Alignment", 3.0f, init_text_alignment, common_logic, draw_text_alignment},
    {"Text Colors", 3.0f, init_text_colors, common_logic, draw_text_colors},
    {"Font Types", 3.0f, init_font_types, common_logic, draw_font_types},
    {"Text Wrapping", 4.0f, init_text_wrapping, common_logic, draw_text_wrapping},
    {"Text Dimensions", 3.0f, init_text_dimensions, common_logic, draw_text_dimensions},
    {"Font Scaling", 3.0f, init_font_scaling, common_logic, draw_font_scaling},
    {"Special Characters", 3.0f, init_special_characters, common_logic, draw_special_characters},
    {"Text Positioning", 3.0f, init_text_positioning, common_logic, draw_text_positioning}
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
    
    printf("\n=== Archimedes Text Basic Tests ===\n");
    printf("Total tests: %d\n", total_scenes);
    printf("Controls: SPACE=Next, ESC=Quit, R=Restart\n");
    printf("Visual tests: 3-4 seconds each\n\n");
    
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
// Visual Text Tests (3-4 seconds each)
// ============================================================================

static void init_basic_rendering(void) {
    // No special initialization needed
}

static void draw_basic_rendering(float dt) {
    (void)dt;
    a_DrawText("Hello, Archimedes!", 400, 300, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("This is a basic text rendering test.", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    draw_ui_overlay();
}

static void init_text_alignment(void) {
    // No special initialization needed
}

static void draw_text_alignment(float dt) {
    (void)dt;
    // Draw text with different alignments
    a_DrawText("Left Aligned Text", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_LEFT, 0);
    a_DrawText("Center Aligned Text", 400, 250, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Right Aligned Text", 400, 300, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_RIGHT, 0);
    
    // Draw vertical line at x=400 to show alignment reference
    for (int y = 150; y < 350; y++) {
        a_DrawPoint(400, y, (aColor_t){128, 128, 128, 255});
    }
    
    draw_ui_overlay();
}

static void init_text_colors(void) {
    // No special initialization needed
}

static void draw_text_colors(float dt) {
    (void)dt;
    // Draw text in different colors
    a_DrawText("Red Text", 400, 150, 255, 0, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Green Text", 400, 200, 0, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Blue Text", 400, 250, 0, 0, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Yellow Text", 400, 300, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Cyan Text", 400, 350, 0, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Magenta Text", 400, 400, 255, 0, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    draw_ui_overlay();
}

static void init_font_types(void) {
    // No special initialization needed
}

static void draw_font_types(float dt) {
    (void)dt;
    // Draw text in different fonts
    a_DrawText("EnterCommand Font", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Linux Font", 400, 300, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Note: FONT_GAME might not be available in test setup
    if (app.fonts[FONT_GAME] != NULL) {
        a_DrawText("Game Font", 400, 400, 255, 255, 255, FONT_GAME, TEXT_ALIGN_CENTER, 0);
    } else {
        a_DrawText("(Game Font Not Available)", 400, 400, 128, 128, 128, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void init_text_wrapping(void) {
    // No special initialization needed
}

static void draw_text_wrapping(float dt) {
    (void)dt;
    // Draw long text that should wrap
    const char* long_text = "This is a very long line of text that should wrap automatically when it exceeds the maximum width specified in the function call. The wrapping should break at word boundaries for readability.";
    
    // Draw wrapped text with max width of 600 pixels
    a_DrawText((char*)long_text, 100, 150, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 600);
    
    // Draw border to show wrapping boundary
    a_DrawRect(100, 150, 600, 300, 128, 128, 128, 255);
    
    draw_ui_overlay();
}

static int text_w, text_h;

static void init_text_dimensions(void) {
    const char* test_text = "Measure This Text";
    a_CalcTextDimensions((char*)test_text, FONT_ENTER_COMMAND, &text_w, &text_h);
}

static void draw_text_dimensions(float dt) {
    (void)dt;
    const char* test_text = "Measure This Text";
    
    // Draw the text
    int text_x = 400;
    int text_y = 250;
    a_DrawText((char*)test_text, text_x, text_y, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Draw bounding rectangle around text
    a_DrawRect(text_x - text_w/2, text_y, text_w, text_h, 255, 0, 0, 255);
    
    // Display dimensions
    char dim_text[100];
    snprintf(dim_text, sizeof(dim_text), "Width: %d, Height: %d", text_w, text_h);
    a_DrawText(dim_text, 400, 350, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    draw_ui_overlay();
}

static void init_font_scaling(void) {
    // No special initialization needed
}

static void draw_font_scaling(float dt) {
    (void)dt;
    // Test different font scales
    float scales[] = {0.5, 1.0, 1.5, 2.0};
    int y_positions[] = {150, 250, 350, 450};
    float original_scale = app.font_scale;
    
    for (int i = 0; i < 4; i++) {
        app.font_scale = scales[i];
        char scale_text[50];
        snprintf(scale_text, sizeof(scale_text), "Scale %.1f", scales[i]);
        a_DrawText(scale_text, 400, y_positions[i], 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    }
    
    // Reset font scale
    app.font_scale = original_scale;
    
    draw_ui_overlay();
}

static void init_special_characters(void) {
    // No special initialization needed
}

static void draw_special_characters(float dt) {
    (void)dt;
    // Test various special characters
    a_DrawText("Special: !@#$%^&*()", 400, 150, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Brackets: []{}()<>", 400, 200, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Math: +-*/=", 400, 250, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Punctuation: ,.;:?!", 400, 300, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Quotes: \"'`", 400, 350, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Other: _-~|\\", 400, 400, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    draw_ui_overlay();
}

static void init_text_positioning(void) {
    // No special initialization needed
}

static void draw_text_positioning(float dt) {
    (void)dt;
    // Test text at various screen positions
    a_DrawText("Top-Left", 10, 60, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("Top-Right", 790, 60, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_RIGHT, 0);
    a_DrawText("Bottom-Left", 10, 520, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("Bottom-Right", 790, 520, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_RIGHT, 0);
    a_DrawText("Center", 400, 300, 0, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Draw crosshair at center
    a_DrawLine(390, 300, 410, 300, (aColor_t){128, 128, 128, 255});
    a_DrawLine(400, 290, 400, 310, (aColor_t){128, 128, 128, 255});
    
    // Draw border to show screen edges
    a_DrawRect(0, 0, 799, 599, 64, 64, 64, 255);
    
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
    if (a_InitTest(800, 600, "Archimedes Text Basic Tests") != 0) {
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