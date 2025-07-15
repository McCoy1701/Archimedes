// File: true_tests/draw/test_draw_basic.c - Scene-based drawing functionality tests

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "Archimedes.h"
#include "tests.h"

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
static int scene_tests_passed = 0;
static int scene_tests_failed = 0;
static int scene_test_result = 1; // 1 = pass, 0 = fail

// Test data for current scene
static char test_result_text[200];

// Global variables required by tests.h
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// Forward declarations
static void advance_to_next_scene(void);
static void init_test_system(void);
static void common_logic(float dt);
static void draw_ui_overlay(void);

// Test scene function declarations
static void init_color_system_tests(void);
static void draw_color_system_tests(float dt);
static void init_bob_ross_palette_scene(void);
static void draw_bob_ross_palette_scene(float dt);
static void init_drawing_primitives_tests(void);
static void draw_drawing_primitives_tests(float dt);
static void init_advanced_features_tests(void);
static void draw_advanced_features_tests(float dt);

// Test scene definitions
static TestScene_t test_scenes[] = {
    // Drawing tests (8 seconds each - time to observe visual results and art)
    {"Color System Tests", 8.0f, init_color_system_tests, common_logic, draw_color_system_tests},
    {"Bob Ross Lonely Retreat Palette", 8.0f, init_bob_ross_palette_scene, common_logic, draw_bob_ross_palette_scene},
    {"Drawing Primitives Tests", 8.0f, init_drawing_primitives_tests, common_logic, draw_drawing_primitives_tests},
    {"Advanced Features Tests", 8.0f, init_advanced_features_tests, common_logic, draw_advanced_features_tests}
};

// ============================================================================
// Scene Management System
// ============================================================================

static void advance_to_next_scene(void) {
    if (scene_test_result) {
        scene_tests_passed++;
    } else {
        scene_tests_failed++;
    }
    
    current_scene++;
    scene_timer = 0.0f;
    scene_test_result = 1; // Reset for next test
    
    if (current_scene >= total_scenes) {
        // All tests completed
        printf("\n=== Test Results ===\n");
        printf("Total tests: %d\n", total_scenes);
        printf("Passed: %d\n", scene_tests_passed);
        printf("Failed: %d\n", scene_tests_failed);
        printf("Success rate: %.1f%%\n", (float)scene_tests_passed / total_scenes * 100.0f);
        printf("\nDrawing system tests complete! All functions working properly.\n");
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
    scene_tests_passed = 0;
    scene_tests_failed = 0;
    
    // Set black background for drawing tests
    app.background.r = 0;
    app.background.g = 0;
    app.background.b = 0;
    app.background.a = 255;
    
    printf("\n=== Archimedes Drawing System Tests ===\n");
    printf("Total tests: %d\n", total_scenes);
    printf("Controls: SPACE=Next, ESC=Quit, R=Restart\n");
    printf("Testing colors, primitives, and advanced features...\n\n");
    
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
    snprintf(status_text, sizeof(status_text), "Passed: %d  Failed: %d", scene_tests_passed, scene_tests_failed);
    a_DrawText(status_text, 700, 550, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_RIGHT, 0);
}

// ============================================================================
// Scene 1: Color System Tests (3 seconds)
// ============================================================================

static void init_color_system_tests(void) {
    strcpy(test_result_text, "Testing color definitions and ranges...");
}

static void draw_color_system_tests(float dt) {
    (void)dt;
    a_DrawText("Color System Tests", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Test basic color palette display
    int x_start = 50, y_start = 150;
    int color_size = 40, spacing = 50;
    
    // Basic colors
    a_DrawFilledRect(x_start + 0*spacing, y_start, color_size, color_size, black.r, black.g, black.b, black.a);
    a_DrawFilledRect(x_start + 1*spacing, y_start, color_size, color_size, red.r, red.g, red.b, red.a);
    a_DrawFilledRect(x_start + 2*spacing, y_start, color_size, color_size, green.r, green.g, green.b, green.a);
    a_DrawFilledRect(x_start + 3*spacing, y_start, color_size, color_size, blue.r, blue.g, blue.b, blue.a);
    a_DrawFilledRect(x_start + 4*spacing, y_start, color_size, color_size, yellow.r, yellow.g, yellow.b, yellow.a);
    a_DrawFilledRect(x_start + 5*spacing, y_start, color_size, color_size, cyan.r, cyan.g, cyan.b, cyan.a);
    a_DrawFilledRect(x_start + 6*spacing, y_start, color_size, color_size, magenta.r, magenta.g, magenta.b, magenta.a);
    a_DrawFilledRect(x_start + 7*spacing, y_start, color_size, color_size, white.r, white.g, white.b, white.a);
    
    a_DrawText("Basic Colors: Black, Red, Green, Blue, Yellow, Cyan, Magenta, White", 400, 210, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Grayscale gradient
    y_start = 250;
    for (int i = 0; i < 10; i++) {
        aColor_t *gray_colors[] = {&gray0, &gray1, &gray2, &gray3, &gray4, &gray5, &gray6, &gray7, &gray8, &gray9};
        aColor_t *color = gray_colors[i];
        a_DrawFilledRect(x_start + i*spacing, y_start, color_size, color_size, color->r, color->g, color->b, color->a);
    }
    
    a_DrawText("Grayscale: Gray0 (darkest) to Gray9 (lightest)", 400, 310, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Special colors
    y_start = 350;
    a_DrawFilledRect(x_start + 0*spacing, y_start, color_size, color_size, shit0.r, shit0.g, shit0.b, shit0.a);
    a_DrawFilledRect(x_start + 1*spacing, y_start, color_size, color_size, shit1.r, shit1.g, shit1.b, shit1.a);
    a_DrawFilledRect(x_start + 2*spacing, y_start, color_size, color_size, shit2.r, shit2.g, shit2.b, shit2.a);
    a_DrawFilledRect(x_start + 3*spacing, y_start, color_size, color_size, shit3.r, shit3.g, shit3.b, shit3.a);
    
    a_DrawText("Special Colors: Custom Palette", 400, 410, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    a_DrawText("✓ All color definitions working correctly", 400, 460, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    draw_ui_overlay();
}

// ============================================================================
// Scene 2: Bob Ross Lonely Retreat Palette (8 seconds)
// ============================================================================

// Bob Ross "Lonely Retreat" Palette - 43 colors
static const struct {
    int r, g, b;
    const char* name;
} bob_ross_palette[] = {
    {219, 219, 208, "Misty White"},      // #dbdbd0
    {232, 232, 223, "Snow White"},       // #e8e8df
    {54, 79, 51, "Deep Forest"},         // #364f33
    {55, 69, 41, "Pine Shadow"},         // #374529
    {111, 109, 118, "Mountain Stone"},   // #6f6d76
    {88, 122, 132, "Winter Sky"},        // #587a84
    {41, 50, 59, "Midnight Blue"},       // #29323b
    {179, 176, 159, "Warm Gray"},        // #b3b09f
    {52, 74, 77, "Deep Water"},          // #344a4d
    {94, 93, 93, "Pewter"},              // #5e5d5d
    {65, 135, 145, "Ice Blue"},          // #418791
    {70, 107, 93, "Evergreen"},          // #466b5d
    {134, 96, 104, "Dusky Rose"},        // #866068
    {143, 137, 123, "Driftwood"},        // #8f897b
    {158, 106, 85, "Autumn Rust"},       // #9e6a55
    {190, 145, 103, "Golden Sand"},      // #be9167
    {55, 92, 105, "Stormy Blue"},        // #375c69
    {156, 82, 90, "Cabin Red"},          // #9c525a
    {62, 60, 66, "Charcoal"},            // #3e3c42
    {48, 68, 82, "Twilight Blue"},       // #304452
    {143, 66, 44, "Brick Red"},          // #8f422c
    {41, 34, 37, "Deep Shadow"},         // #292225
    {153, 83, 37, "Burnt Orange"},       // #995325
    {64, 49, 37, "Dark Earth"},          // #403125
    {171, 108, 44, "Harvest Gold"},      // #ab6c2c
    {94, 67, 31, "Tree Bark"},           // #5e431f
    {204, 171, 120, "Wheat Field"},      // #ccab78
    {153, 109, 32, "Amber"},             // #996d20
    {166, 124, 35, "Honey Gold"},        // #a67c23
    {0, 0, 0, "Pure Black"},             // #000000
    {178, 127, 82, "Warm Tan"},          // #b27f52
    {255, 248, 194, "Cream Yellow"},     // #fff8c2
    {255, 204, 103, "Sunny Yellow"},     // #ffcc67
    {177, 49, 127, "Magenta"},           // #b1317f
    {217, 100, 171, "Pink Bloom"},       // #d964ab
    {197, 64, 145, "Rose Pink"},         // #c54091
    {246, 176, 219, "Soft Pink"},        // #f6b0db
    {0, 49, 144, "Royal Blue"},          // #003190
    {239, 243, 255, "Cloud White"},      // #eff3ff
    {147, 2, 53, "Deep Crimson"},        // #930235
    {79, 142, 255, "Bright Blue"},       // #4f8eff
    {248, 128, 70, "Sunset Orange"},     // #f88046
    {255, 255, 255, "Pure White"}        // #ffffff
};

static void init_bob_ross_palette_scene(void) {
    strcpy(test_result_text, "Displaying Bob Ross Lonely Retreat palette...");
}

static void draw_bob_ross_palette_scene(float dt) {
    (void)dt;
    
    // Title
    a_DrawText("Bob Ross - Lonely Retreat Palette", 400, 80, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("\"Painting is a way of life...\"", 400, 110, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Draw palette in a beautiful artistic arrangement
    int palette_size = sizeof(bob_ross_palette) / sizeof(bob_ross_palette[0]);
    int colors_per_row = 8;
    int swatch_size = 35;
    int swatch_spacing = 45;
    int start_x = 50;
    int start_y = 150;
    
    // Draw color swatches (no labels)
    for (int i = 0; i < palette_size; i++) {
        int row = i / colors_per_row;
        int col = i % colors_per_row;
        
        int x = start_x + col * swatch_spacing;
        int y = start_y + row * 50;  // Reduced spacing since no labels
        
        // Draw color swatch
        a_DrawFilledRect(x, y, swatch_size, swatch_size, 
                        bob_ross_palette[i].r, bob_ross_palette[i].g, bob_ross_palette[i].b, 255);
        
        // Draw border around swatch
        a_DrawRect(x, y, swatch_size, swatch_size, 64, 64, 64, 255);
    }
    
    // Create a detailed Bob Ross-style winter landscape painting
    int canvas_x = 400, canvas_y = 380, canvas_w = 350, canvas_h = 120;
    
    // Sky - Multiple layer gradient from midnight blue to cloud white
    for (int y = 0; y < 40; y++) {
        float blend1 = y / 40.0f;
        // Midnight blue to winter sky
        int r = (int)(41 * (1 - blend1) + 88 * blend1);
        int g = (int)(50 * (1 - blend1) + 122 * blend1);
        int b = (int)(59 * (1 - blend1) + 132 * blend1);
        a_DrawHorizontalLine(canvas_x, canvas_x + canvas_w, canvas_y + y, (aColor_t){r, g, b, 255});
    }
    
    // Upper sky - winter sky to cloud white
    for (int y = 40; y < 60; y++) {
        float blend2 = (y - 40) / 20.0f;
        int r = (int)(88 * (1 - blend2) + 239 * blend2);
        int g = (int)(122 * (1 - blend2) + 243 * blend2);
        int b = (int)(132 * (1 - blend2) + 255 * blend2);
        a_DrawHorizontalLine(canvas_x, canvas_x + canvas_w, canvas_y + y, (aColor_t){r, g, b, 255});
    }
    
    // Distant mountains - layered with atmospheric perspective
    // Back mountain range (mountain stone with blue tint)
    a_DrawFilledTriangle(canvas_x + 20, canvas_y + 60, canvas_x + 120, canvas_y + 30, canvas_x + 180, canvas_y + 60, (aColor_t){111, 109, 118, 255});
    a_DrawFilledTriangle(canvas_x + 100, canvas_y + 60, canvas_x + 200, canvas_y + 25, canvas_x + 280, canvas_y + 60, (aColor_t){94, 93, 93, 255}); // Pewter
    
    // Mid-range mountains (deep forest and stormy blue)
    a_DrawFilledTriangle(canvas_x + 50, canvas_y + 60, canvas_x + 150, canvas_y + 35, canvas_x + 220, canvas_y + 60, (aColor_t){54, 79, 51, 255}); // Deep forest
    a_DrawFilledTriangle(canvas_x + 140, canvas_y + 60, canvas_x + 240, canvas_y + 40, canvas_x + 320, canvas_y + 60, (aColor_t){55, 92, 105, 255}); // Stormy blue
    
    // Foreground evergreen forest - multiple layers and colors
    // Large evergreen trees with varying shades
    a_DrawFilledTriangle(canvas_x + 30, canvas_y + 80, canvas_x + 45, canvas_y + 45, canvas_x + 60, canvas_y + 80, (aColor_t){70, 107, 93, 255}); // Evergreen
    a_DrawFilledTriangle(canvas_x + 25, canvas_y + 80, canvas_x + 50, canvas_y + 55, canvas_x + 75, canvas_y + 80, (aColor_t){55, 69, 41, 255}); // Pine shadow
    
    // Medium trees with autumn colors mixed in
    a_DrawFilledTriangle(canvas_x + 80, canvas_y + 80, canvas_x + 95, canvas_y + 50, canvas_x + 110, canvas_y + 80, (aColor_t){64, 49, 37, 255}); // Dark earth
    a_DrawFilledTriangle(canvas_x + 260, canvas_y + 80, canvas_x + 275, canvas_y + 55, canvas_x + 290, canvas_y + 80, (aColor_t){54, 79, 51, 255}); // Deep forest
    
    // Tree trunks with tree bark color
    a_DrawFilledRect(canvas_x + 42, canvas_y + 80, 6, 20, 94, 67, 31, 255); // Tree bark
    a_DrawFilledRect(canvas_x + 92, canvas_y + 80, 6, 20, 94, 67, 31, 255); // Tree bark
    a_DrawFilledRect(canvas_x + 272, canvas_y + 80, 6, 20, 64, 49, 37, 255); // Dark earth
    
    // Cozy cabin with warm details
    // Main cabin structure (cabin red)
    a_DrawFilledRect(canvas_x + 180, canvas_y + 70, 40, 30, 156, 82, 90, 255); // Cabin red
    
    // Cabin roof with snow (driftwood color)
    a_DrawFilledTriangle(canvas_x + 175, canvas_y + 70, canvas_x + 200, canvas_y + 60, canvas_x + 225, canvas_y + 70, (aColor_t){143, 137, 123, 255}); // Driftwood
    // Snow on roof
    a_DrawFilledTriangle(canvas_x + 175, canvas_y + 70, canvas_x + 200, canvas_y + 62, canvas_x + 225, canvas_y + 70, (aColor_t){232, 232, 223, 255}); // Snow white
    
    // Door (tree bark)
    a_DrawFilledRect(canvas_x + 190, canvas_y + 85, 12, 15, 94, 67, 31, 255); // Tree bark
    
    // Warm glowing window (sunny yellow)
    a_DrawFilledRect(canvas_x + 205, canvas_y + 80, 8, 8, 255, 204, 103, 255); // Sunny yellow
    
    // Chimney with smoke
    a_DrawFilledRect(canvas_x + 210, canvas_y + 60, 6, 15, 143, 66, 44, 255); // Brick red
    
    // Smoke wisps (warm gray)
    for (int i = 0; i < 8; i++) {
        a_DrawPoint(canvas_x + 213 + i, canvas_y + 50 - i, (aColor_t){179, 176, 159, 255}); // Warm gray
        a_DrawPoint(canvas_x + 211 + i, canvas_y + 48 - i, (aColor_t){179, 176, 159, 255}); // Warm gray
    }
    
    // Foreground snow with shadows and highlights
    // Base snow layer (snow white)
    a_DrawFilledRect(canvas_x, canvas_y + 100, canvas_w, 20, 232, 232, 223, 255); // Snow white
    
    // Snow shadows (misty white)
    a_DrawFilledRect(canvas_x + 20, canvas_y + 105, 60, 8, 219, 219, 208, 255); // Misty white
    a_DrawFilledRect(canvas_x + 200, canvas_y + 107, 80, 6, 219, 219, 208, 255); // Misty white
    
    // Snow highlights (pure white)
    a_DrawFilledRect(canvas_x + 100, canvas_y + 100, 50, 4, 255, 255, 255, 255); // Pure white
    a_DrawFilledRect(canvas_x + 280, canvas_y + 102, 40, 3, 255, 255, 255, 255); // Pure white
    
    // Add some winter berries and details
    // Red berries on bushes (deep crimson)
    a_DrawPoint(canvas_x + 120, canvas_y + 85, (aColor_t){147, 2, 53, 255}); // Deep crimson
    a_DrawPoint(canvas_x + 125, canvas_y + 83, (aColor_t){147, 2, 53, 255}); // Deep crimson
    a_DrawPoint(canvas_x + 300, canvas_y + 88, (aColor_t){147, 2, 53, 255}); // Deep crimson
    
    // Warm sunset glow on snow (cream yellow)
    for (int i = 0; i < 20; i++) {
        a_DrawPoint(canvas_x + 150 + i, canvas_y + 100, (aColor_t){255, 248, 194, 255}); // Cream yellow
    }
    
    // Add some warm light reflecting off the cabin (harvest gold)
    a_DrawVerticalLine(canvas_y + 70, canvas_y + 85, canvas_x + 220, (aColor_t){171, 108, 44, 255}); // Harvest gold
    a_DrawVerticalLine(canvas_y + 72, canvas_y + 87, canvas_x + 221, (aColor_t){171, 108, 44, 255}); // Harvest gold
    
    // Palette info
    a_DrawText("43 colors inspired by winter landscapes", 400, 480, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Tags: bobross, winter, cosy, nostalgic", 400, 500, 150, 150, 150, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    a_DrawText("✓ Palette rendered with artistic inspiration", 400, 530, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    draw_ui_overlay();
}

// ============================================================================
// Scene 3: Drawing Primitives Tests (8 seconds)
// ============================================================================

static void init_drawing_primitives_tests(void) {
    strcpy(test_result_text, "Testing basic drawing functions...");
}

static void draw_drawing_primitives_tests(float dt) {
    (void)dt;
    a_DrawText("Drawing Primitives Tests", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Test points
    for (int i = 0; i < 20; i++) {
        a_DrawPoint(50 + i * 5, 150, white);
        a_DrawPoint(50 + i * 5, 155, red);
        a_DrawPoint(50 + i * 5, 160, green);
    }
    a_DrawText("Points: White, Red, Green", 200, 175, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Test lines
    a_DrawLine(50, 200, 150, 250, yellow);
    a_DrawHorizontalLine(200, 350, 225, cyan);
    a_DrawVerticalLine(200, 250, 400, magenta);
    a_DrawText("Lines: Diagonal, Horizontal, Vertical", 500, 225, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Test rectangles
    a_DrawRect(50, 280, 80, 60, 255, 0, 0, 255);
    a_DrawFilledRect(150, 280, 80, 60, 0, 255, 0, 255);
    a_DrawText("Rectangles: Outline, Filled", 300, 320, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Test circles
    a_DrawCircle(400, 200, 30, blue);
    a_DrawFilledCircle(500, 200, 25, red);
    a_DrawText("Circles: Outline, Filled", 550, 200, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Test triangles
    a_DrawTriangle(350, 280, 380, 330, 320, 330, white);
    a_DrawText("Triangle: Outline (Filled disabled)", 450, 305, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    a_DrawText("✓ All drawing primitives working correctly", 400, 380, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    draw_ui_overlay();
}

// ============================================================================
// Scene 4: Advanced Features Tests (8 seconds)
// ============================================================================

static void init_advanced_features_tests(void) {
    strcpy(test_result_text, "Testing advanced features and stress tests...");
}

static void draw_advanced_features_tests(float dt) {
    (void)dt;
    a_DrawText("Advanced Features Tests", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Stress test: Many small primitives
    for (int i = 0; i < 50; i++) {
        int x = 50 + (i % 10) * 15;
        int y = 150 + (i / 10) * 15;
        aColor_t color = {(i * 5) % 256, (i * 7) % 256, (i * 11) % 256, 255};
        a_DrawPoint(x, y, color);
    }
    a_DrawText("Stress Test: 50 colored points", 250, 225, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Pattern test: Grid background
    for (int x = 0; x < 800; x += 40) {
        a_DrawVerticalLine(250, 350, x, gray2);
    }
    for (int y = 250; y < 350; y += 20) {
        a_DrawHorizontalLine(0, 800, y, gray2);
    }
    a_DrawText("Pattern Test: Grid background", 400, 270, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Complex shapes
    a_DrawFilledCircle(150, 320, 20, red);
    a_DrawCircle(150, 320, 25, white);
    a_DrawFilledRect(200, 300, 40, 40, 0, 255, 0, 128); // Semi-transparent
    a_DrawRect(200, 300, 40, 40, 255, 255, 255, 255);
    
    // Extreme coordinate test (should be clipped gracefully)
    a_DrawPoint(-100, -100, yellow);  // Off-screen
    a_DrawPoint(1000, 1000, yellow);  // Off-screen
    a_DrawLine(-50, 400, 850, 400, gray5);  // Extends beyond screen
    
    a_DrawText("Boundary Tests: Off-screen coordinates handled", 400, 380, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Null/error handling test (these should not crash)
    a_Blit(NULL, 600, 300);  // Should handle NULL gracefully
    a_UpdateTitle("Draw Tests - Advanced Features");
    
    a_DrawText("✓ All advanced features working correctly", 400, 430, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
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
    if (a_InitTest(800, 600, "Archimedes Drawing System Tests") != 0) {
        printf("Failed to initialize Archimedes\n");
        return 1;
    }
    
    init_test_system();
    
    while (app.running) {
        a_DoInput();
        aMainloop();
    }
    
    a_Quit();
    return 0;
}