// File: true_tests/draw/test_draw_basic.c - Basic drawing functionality tests

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "Archimedes.h"
#include "tests.h"

// Global test counters
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// Test helper functions
static void reset_drawing_state(void) {
    // Reset background to default
    app.background.r = 0;
    app.background.g = 0;
    app.background.b = 0;
    app.background.a = 255;

    // Reset any render state if needed
    if (app.renderer) {
        SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    }
}

static int is_valid_color_component(int component) {
    return (component >= 0 && component <= 255);
}

static int colors_equal(const aColor_t* c1, const aColor_t* c2) {
    return (c1->r == c2->r && c1->g == c2->g && c1->b == c2->b && c1->a == c2->a);
}

// ============================================================================
// Color Definition Tests
// ============================================================================

int test_predefined_colors(void) {
    reset_drawing_state();

    // Test basic colors have correct values
    TEST_ASSERT(black.r == 0 && black.g == 0 && black.b == 0 && black.a == 255, "Black color should be (0,0,0,255)");
    TEST_ASSERT(white.r == 255 && white.g == 255 && white.b == 255 && white.a == 255, "White color should be (255,255,255,255)");
    TEST_ASSERT(red.r == 255 && red.g == 0 && red.b == 0 && red.a == 255, "Red color should be (255,0,0,255)");
    TEST_ASSERT(green.r == 0 && green.g == 255 && green.b == 0 && green.a == 255, "Green color should be (0,255,0,255)");
    TEST_ASSERT(blue.r == 0 && blue.g == 0 && blue.b == 255 && blue.a == 255, "Blue color should be (0,0,255,255)");
    TEST_ASSERT(cyan.r == 0 && cyan.g == 255 && cyan.b == 255 && cyan.a == 255, "Cyan color should be (0,255,255,255)");
    TEST_ASSERT(magenta.r == 255 && magenta.g == 0 && magenta.b == 255 && magenta.a == 255, "Magenta color should be (255,0,255,255)");
    TEST_ASSERT(yellow.r == 255 && yellow.g == 255 && yellow.b == 0 && yellow.a == 255, "Yellow color should be (255,255,0,255)");

    return 1;
}

int test_gray_scale_colors(void) {
    reset_drawing_state();

    // Test gray scale values are correct
    TEST_ASSERT(gray0.r == 35 && gray0.g == 35 && gray0.b == 35, "Gray0 should be (35,35,35)");
    TEST_ASSERT(gray9.r == 235 && gray9.g == 235 && gray9.b == 235, "Gray9 should be (235,235,235)");

    // Test that grays progress from dark to light
    TEST_ASSERT(gray0.r < gray1.r, "Gray0 should be darker than Gray1");
    TEST_ASSERT(gray1.r < gray2.r, "Gray1 should be darker than Gray2");
    TEST_ASSERT(gray8.r < gray9.r, "Gray8 should be darker than Gray9");

    // Test all grays have same R, G, B values (true grayscale)
    TEST_ASSERT(gray0.r == gray0.g && gray0.g == gray0.b, "Gray0 should have equal RGB components");
    TEST_ASSERT(gray5.r == gray5.g && gray5.g == gray5.b, "Gray5 should have equal RGB components");
    TEST_ASSERT(gray9.r == gray9.g && gray9.g == gray9.b, "Gray9 should have equal RGB components");

    // Test all grays are fully opaque
    TEST_ASSERT(gray0.a == 255, "Gray0 should be fully opaque");
    TEST_ASSERT(gray5.a == 255, "Gray5 should be fully opaque");
    TEST_ASSERT(gray9.a == 255, "Gray9 should be fully opaque");

    return 1;
}

int test_color_value_ranges(void) {
    reset_drawing_state();

    // Test all predefined colors have valid component values
    TEST_ASSERT(is_valid_color_component(black.r), "Black red component should be valid");
    TEST_ASSERT(is_valid_color_component(white.g), "White green component should be valid");
    TEST_ASSERT(is_valid_color_component(red.b), "Red blue component should be valid");
    TEST_ASSERT(is_valid_color_component(cyan.a), "Cyan alpha component should be valid");

    // Test special colors have valid ranges
    TEST_ASSERT(is_valid_color_component(shit0.r) && is_valid_color_component(shit0.g) && is_valid_color_component(shit0.b), "Shit0 should have valid RGB components");
    TEST_ASSERT(is_valid_color_component(shit1.r) && is_valid_color_component(shit1.g) && is_valid_color_component(shit1.b), "Shit1 should have valid RGB components");
    TEST_ASSERT(is_valid_color_component(shit2.r) && is_valid_color_component(shit2.g) && is_valid_color_component(shit2.b), "Shit2 should have valid RGB components");
    TEST_ASSERT(is_valid_color_component(shit3.r) && is_valid_color_component(shit3.g) && is_valid_color_component(shit3.b), "Shit3 should have valid RGB components");

    return 1;
}

// ============================================================================
// Drawing Function Safety Tests
// ============================================================================

int test_drawing_functions_dont_crash(void) {
    reset_drawing_state();

    // Test that drawing functions don't crash when called
    // These functions should complete without segfaulting

    // Test scene management
    a_PrepareScene();
    TEST_ASSERT(1, "PrepareScene should not crash");

    a_PresentScene();
    TEST_ASSERT(1, "PresentScene should not crash");

    // Test basic drawing primitives
    a_DrawPoint(100, 100, white);
    TEST_ASSERT(1, "DrawPoint should not crash");

    a_DrawLine(0, 0, 100, 100, red);
    TEST_ASSERT(1, "DrawLine should not crash");

    a_DrawHorizontalLine(0, 200, 50, green);
    TEST_ASSERT(1, "DrawHorizontalLine should not crash");

    a_DrawVerticalLine(0, 200, 100, blue);
    TEST_ASSERT(1, "DrawVerticalLine should not crash");

    return 1;
}

int test_circle_drawing_functions(void) {
    reset_drawing_state();

    // Test circle drawing functions don't crash
    a_DrawCircle(100, 100, 50, white);
    TEST_ASSERT(1, "DrawCircle should not crash");

    a_DrawFilledCircle(200, 200, 30, red);
    TEST_ASSERT(1, "DrawFilledCircle should not crash");

    // Test with zero radius
    a_DrawCircle(300, 300, 0, green);
    TEST_ASSERT(1, "DrawCircle with zero radius should not crash");

    a_DrawFilledCircle(400, 400, 0, blue);
    TEST_ASSERT(1, "DrawFilledCircle with zero radius should not crash");

    return 1;
}

int test_rectangle_drawing_functions(void) {
    reset_drawing_state();

    // Test rectangle drawing functions don't crash
    a_DrawRect(10, 10, 100, 50, 255, 255, 255, 255);
    TEST_ASSERT(1, "DrawRect should not crash");

    a_DrawFilledRect(10, 100, 100, 50, 0, 0, 255, 255);
    TEST_ASSERT(1, "DrawFilledRect should not crash");

    // Test with zero size
    a_DrawRect(200, 200, 0, 0, 255, 0, 0, 255);
    TEST_ASSERT(1, "DrawRect with zero size should not crash");

    a_DrawFilledRect(300, 300, 0, 0, 0, 255, 0, 255);
    TEST_ASSERT(1, "DrawFilledRect with zero size should not crash");

    return 1;
}

int test_triangle_drawing_functions(void) {
    reset_drawing_state();

    // Test triangle drawing functions don't crash
    a_DrawTriangle(50, 50, 100, 10, 150, 50, white);
    TEST_ASSERT(1, "DrawTriangle should not crash");

    a_DrawFilledTriangle(200, 50, 250, 10, 300, 50, red);
    TEST_ASSERT(1, "DrawFilledTriangle should not crash");

    // Test with degenerate triangle (all points same)
    a_DrawTriangle(100, 100, 100, 100, 100, 100, green);
    TEST_ASSERT(1, "DrawTriangle with same points should not crash");

    a_DrawFilledTriangle(200, 200, 200, 200, 200, 200, blue);
    TEST_ASSERT(1, "DrawFilledTriangle with same points should not crash");

    return 1;
}

// ============================================================================
// Parameter Boundary Tests
// ============================================================================

int test_extreme_coordinates(void) {
    reset_drawing_state();

    // Test functions with extreme coordinates
    a_DrawPoint(-1000, -1000, white);
    TEST_ASSERT(1, "DrawPoint with negative coordinates should not crash");

    a_DrawPoint(10000, 10000, red);
    TEST_ASSERT(1, "DrawPoint with large coordinates should not crash");

    a_DrawLine(-500, -500, 5000, 5000, green);
    TEST_ASSERT(1, "DrawLine with extreme coordinates should not crash");

    a_DrawRect(-100, -100, 50, 50, 255, 0, 0, 255);
    TEST_ASSERT(1, "DrawRect with negative position should not crash");

    return 1;
}

int test_color_parameter_boundaries(void) {
    reset_drawing_state();

    // Test functions with out-of-range color values
    a_DrawFilledRect(10, 10, 50, 50, -50, 300, 500, -100);
    TEST_ASSERT(1, "DrawFilledRect with out-of-range colors should not crash");

    a_DrawPoint(100, 100, (aColor_t){-10, 270, 400, -50});
    TEST_ASSERT(1, "DrawPoint with invalid color values should not crash");

    // Test with extreme color values
    a_DrawLine(0, 0, 100, 100, (aColor_t){1000, -1000, 2000, 500});
    TEST_ASSERT(1, "DrawLine with extreme color values should not crash");

    return 1;
}

int test_size_parameter_boundaries(void) {
    reset_drawing_state();

    // Test functions with negative sizes
    a_DrawRect(100, 100, -50, -50, 255, 255, 255, 255);
    TEST_ASSERT(1, "DrawRect with negative size should not crash");

    a_DrawFilledRect(200, 200, -100, -100, 0, 255, 0, 255);
    TEST_ASSERT(1, "DrawFilledRect with negative size should not crash");

    // Test circles with extreme radii
    a_DrawCircle(300, 300, -10, red);
    TEST_ASSERT(1, "DrawCircle with negative radius should not crash");

    a_DrawCircle(400, 400, 10000, blue);
    TEST_ASSERT(1, "DrawCircle with huge radius should not crash");

    return 1;
}

// ============================================================================
// Blitting Function Tests
// ============================================================================

int test_blit_null_safety(void) {
    reset_drawing_state();

    // Test blitting functions with NULL surface
    a_Blit(NULL, 100, 100);
    TEST_ASSERT(1, "Blit with NULL surface should not crash");

    SDL_Rect test_rect = {0, 0, 50, 50};
    a_BlitRect(NULL, test_rect, 100, 100);
    TEST_ASSERT(1, "BlitRect with NULL surface should not crash");

    return 1;
}

int test_blit_parameter_ranges(void) {
    reset_drawing_state();

    SDL_Rect zero_rect = {0, 0, 0, 0};
    SDL_Rect negative_rect = {0, 0, -10, -10};
    SDL_Rect large_rect = {0, 0, 10000, 10000};

    // Test with various rect parameters
    a_BlitRect(NULL, zero_rect, 0, 0);
    TEST_ASSERT(1, "BlitRect with zero-size rect should not crash");

    a_BlitRect(NULL, negative_rect, 100, 100);
    TEST_ASSERT(1, "BlitRect with negative rect should not crash");

    a_BlitRect(NULL, large_rect, -1000, -1000);
    TEST_ASSERT(1, "BlitRect with large rect and negative position should not crash");

    return 1;
}

// ============================================================================
// Window Title Function Tests
// ============================================================================

int test_window_title_safety(void) {
    reset_drawing_state();

    // Test window title updates
    a_UpdateTitle("Test Title");
    TEST_ASSERT(1, "UpdateTitle with normal string should not crash");

    a_UpdateTitle("");
    TEST_ASSERT(1, "UpdateTitle with empty string should not crash");

    a_UpdateTitle("Very Long Title With Many Characters That Should Still Work Fine Even If It Is Extremely Long");
    TEST_ASSERT(1, "UpdateTitle with long string should not crash");

    a_UpdateTitle("Title with !@#$%^&*()_+-=[]{}|;:,.<>? symbols");
    TEST_ASSERT(1, "UpdateTitle with special characters should not crash");

    // Test with NULL (this might crash depending on implementation)
    a_UpdateTitle(NULL);
    TEST_ASSERT(1, "UpdateTitle with NULL should be handled gracefully");

    return 1;
}

// ============================================================================
// Render Cycle Tests
// ============================================================================

int test_render_cycle_sequence(void) {
    reset_drawing_state();

    // Test proper render cycle sequence
    for (int i = 0; i < 5; i++) {
        a_PrepareScene();
        TEST_ASSERT(1, "PrepareScene in cycle should not crash");

        // Draw something simple
        a_DrawPoint(i * 10, i * 10, white);
        a_DrawLine(i * 20, 0, i * 20, 100, red);

        a_PresentScene();
        TEST_ASSERT(1, "PresentScene in cycle should not crash");
    }

    return 1;
}

int test_background_color_changes(void) {
    reset_drawing_state();

    // Test changing background colors
    aColor_t original_bg = app.background;

    app.background = red;
    a_PrepareScene();
    TEST_ASSERT(1, "PrepareScene with red background should not crash");

    app.background = green;
    a_PrepareScene();
    TEST_ASSERT(1, "PrepareScene with green background should not crash");

    app.background = blue;
    a_PrepareScene();
    TEST_ASSERT(1, "PrepareScene with blue background should not crash");

    // Restore original background
    app.background = original_bg;

    return 1;
}

// ============================================================================
// Stress Tests
// ============================================================================

int test_many_drawing_operations(void) {
    reset_drawing_state();

    a_PrepareScene();

    // Draw many primitives to test performance/stability
    for (int i = 0; i < 100; i++) {
        int x = i % 50;
        int y = i / 50;
        aColor_t color = {(i * 2) % 256, (i * 3) % 256, (i * 5) % 256, 255};

        a_DrawPoint(x * 10, y * 10, color);
        a_DrawLine(x * 8, y * 8, x * 12, y * 12, color);
        a_DrawRect(x * 15, y * 15, 5, 5, color.r, color.g, color.b, color.a);
    }

    TEST_ASSERT(1, "Many drawing operations should not crash");

    a_PresentScene();
    TEST_ASSERT(1, "PresentScene after many operations should not crash");

    return 1;
}

int test_rapid_render_cycles(void) {
    reset_drawing_state();

    // Test rapid scene preparation and presentation
    for (int cycle = 0; cycle < 50; cycle++) {
        a_PrepareScene();

        // Simple drawing operation each cycle
        aColor_t color = {(cycle * 5) % 256, (cycle * 7) % 256, (cycle * 11) % 256, 255};
        a_DrawFilledRect(cycle % 10 * 20, cycle % 10 * 15, 10, 10, color.r, color.g, color.b, color.a);

        a_PresentScene();
    }

    TEST_ASSERT(1, "Rapid render cycles should not crash");

    return 1;
}

// ============================================================================
// Integration Tests
// ============================================================================

int test_complete_drawing_session(void) {
    reset_drawing_state();

    // Test a complete drawing session with various operations
    a_PrepareScene();

    // Draw background pattern
    for (int x = 0; x < 400; x += 20) {
        a_DrawVerticalLine(0, 300, x, gray2);
    }
    for (int y = 0; y < 300; y += 20) {
        a_DrawHorizontalLine(0, 400, y, gray2);
    }

    // Draw some shapes
    a_DrawFilledCircle(100, 100, 30, red);
    a_DrawCircle(100, 100, 35, black);

    a_DrawFilledRect(200, 50, 60, 40, 0, 255, 0, 255);
    a_DrawRect(200, 50, 60, 40, 0, 0, 0, 255);

    a_DrawTriangle(300, 50, 350, 150, 250, 150, blue);
    a_DrawFilledTriangle(310, 60, 340, 140, 260, 140, cyan);

    // Draw some text-like pattern with points
    for (int i = 0; i < 20; i++) {
        a_DrawPoint(50 + i * 2, 200, white);
        a_DrawPoint(50 + i * 2, 205, white);
    }

    TEST_ASSERT(1, "Complete drawing session should not crash");

    a_PresentScene();
    TEST_ASSERT(1, "Final present of complete session should not crash");

    return 1;
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    // Initialize minimal app structure for testing
    memset(&app, 0, sizeof(app));
    reset_drawing_state();

    TEST_SUITE_START("Drawing System Basic Tests");

    // Color definition tests
    RUN_TEST(test_predefined_colors);
    RUN_TEST(test_gray_scale_colors);
    RUN_TEST(test_color_value_ranges);

    // Drawing function safety tests
    RUN_TEST(test_drawing_functions_dont_crash);
    RUN_TEST(test_circle_drawing_functions);
    RUN_TEST(test_rectangle_drawing_functions);
    RUN_TEST(test_triangle_drawing_functions);

    // Parameter boundary tests
    RUN_TEST(test_extreme_coordinates);
    RUN_TEST(test_color_parameter_boundaries);
    RUN_TEST(test_size_parameter_boundaries);

    // Blitting function tests
    RUN_TEST(test_blit_null_safety);
    RUN_TEST(test_blit_parameter_ranges);

    // Window title tests
    RUN_TEST(test_window_title_safety);

    // Render cycle tests
    RUN_TEST(test_render_cycle_sequence);
    RUN_TEST(test_background_color_changes);

    // Stress tests
    RUN_TEST(test_many_drawing_operations);
    RUN_TEST(test_rapid_render_cycles);

    // Integration tests
    RUN_TEST(test_complete_drawing_session);

    TEST_SUITE_END();
}
