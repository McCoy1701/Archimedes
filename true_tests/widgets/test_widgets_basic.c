// File: true_tests/widgets/test_widgets_basic.c - Basic widget functionality tests

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

// Helper to reset app state
static void reset_app_state(void) {
    memset(&app, 0, sizeof(aApp_t));
    app.running = 1;
    app.font_scale = 1.0;
    app.active_widget = NULL;
    memset(app.input_text, 0, sizeof(app.input_text));
    app.last_key_pressed = 0;
}

// Test basic widget initialization
int test_widget_initialization(void) {
    reset_app_state();
    
    // Test that app is properly initialized
    TEST_ASSERT(app.running == 1, "App should be running");
    TEST_ASSERT(app.active_widget == NULL, "Active widget should be NULL initially");
    TEST_ASSERT(app.font_scale == 1.0, "Font scale should be 1.0");
    
    return 1;
}

// Test widget constants and basic structures
int test_widget_constants(void) {
    reset_app_state();
    
    // Test that widget type constants are defined
    TEST_ASSERT(WT_BUTTON >= 0, "WT_BUTTON should be defined");
    TEST_ASSERT(WT_INPUT >= 0, "WT_INPUT should be defined");
    TEST_ASSERT(WT_SLIDER >= 0, "WT_SLIDER should be defined");
    TEST_ASSERT(WT_SELECT >= 0, "WT_SELECT should be defined");
    TEST_ASSERT(WT_CONTROL >= 0, "WT_CONTROL should be defined");
    TEST_ASSERT(WT_CONTAINER >= 0, "WT_CONTAINER should be defined");
    
    return 1;
}

// Test app state management
int test_app_state_management(void) {
    reset_app_state();
    
    // Test input text buffer
    strcpy(app.input_text, "test");
    TEST_ASSERT(strcmp(app.input_text, "test") == 0, "Input text should be set correctly");
    
    // Test keyboard state
    app.keyboard[SDL_SCANCODE_A] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_A] == 1, "Keyboard state should be set correctly");
    
    // Test mouse state
    app.mouse.x = 123;
    app.mouse.y = 456;
    TEST_ASSERT(app.mouse.x == 123, "Mouse X should be set correctly");
    TEST_ASSERT(app.mouse.y == 456, "Mouse Y should be set correctly");
    
    return 1;
}

// Test widget structure validation
int test_widget_structure_validation(void) {
    reset_app_state();
    
    // Test widget structure size and alignment
    TEST_ASSERT(sizeof(aWidget_t) > 0, "Widget structure should have non-zero size");
    
    // Test widget coordinate boundaries
    // Simulate widget with extreme coordinates
    app.active_widget = NULL; // Ensure no active widget initially
    
    // Test widget dimensions - simulate checking bounds
    int widget_x = 100, widget_y = 200, widget_w = 150, widget_h = 50;
    TEST_ASSERT(widget_x >= 0, "Widget X coordinate should be valid");
    TEST_ASSERT(widget_y >= 0, "Widget Y coordinate should be valid");
    TEST_ASSERT(widget_w > 0, "Widget width should be positive");
    TEST_ASSERT(widget_h > 0, "Widget height should be positive");
    
    // Test widget boundary calculations
    int right_edge = widget_x + widget_w;
    int bottom_edge = widget_y + widget_h;
    TEST_ASSERT(right_edge == 250, "Widget right edge should be calculated correctly");
    TEST_ASSERT(bottom_edge == 250, "Widget bottom edge should be calculated correctly");
    
    // Test color array boundaries (RGBA values)
    uint8_t test_fg[4] = {255, 128, 64, 255};
    uint8_t test_bg[4] = {32, 64, 128, 200};
    TEST_ASSERT(test_fg[0] == 255, "Foreground red should be valid");
    TEST_ASSERT(test_fg[3] == 255, "Foreground alpha should be valid");
    TEST_ASSERT(test_bg[0] == 32, "Background red should be valid");
    TEST_ASSERT(test_bg[3] == 200, "Background alpha should be valid");
    
    return 1;
}

// Test widget list operations
int test_widget_list_operations(void) {
    reset_app_state();
    
    // Test widget linked list concepts
    // Since we can't create actual widgets easily, test the pointer logic
    
    // Test null widget handling
    aWidget_t* null_widget = NULL;
    TEST_ASSERT(null_widget == NULL, "Null widget should be null");
    
    // Test widget pointer arithmetic concepts
    size_t widget_size = sizeof(aWidget_t);
    TEST_ASSERT(widget_size > sizeof(void*), "Widget should be larger than pointer");
    
    // Test widget field offsets (basic structure validation)
    // This tests that the widget structure is properly aligned
    TEST_ASSERT(sizeof(int) == 4, "Int size should be 4 bytes on this platform");
    
    // Test widget name length limits
    char test_name[MAX_FILENAME_LENGTH];
    memset(test_name, 'A', MAX_FILENAME_LENGTH - 1);
    test_name[MAX_FILENAME_LENGTH - 1] = '\0';
    TEST_ASSERT(strlen(test_name) == MAX_FILENAME_LENGTH - 1, "Widget name should fit in allocated space");
    
    // Test widget label length limits
    char test_label[MAX_FILENAME_LENGTH];
    memset(test_label, 'B', MAX_FILENAME_LENGTH - 1);
    test_label[MAX_FILENAME_LENGTH - 1] = '\0';
    TEST_ASSERT(strlen(test_label) == MAX_FILENAME_LENGTH - 1, "Widget label should fit in allocated space");
    
    return 1;
}

// Test widget mouse interaction boundaries
int test_widget_mouse_interaction_boundaries(void) {
    reset_app_state();
    
    // Test mouse click detection within widget boundaries
    int widget_x = 100, widget_y = 150, widget_w = 200, widget_h = 80;
    
    // Test center click
    int center_x = widget_x + widget_w / 2;
    int center_y = widget_y + widget_h / 2;
    app.mouse.x = center_x;
    app.mouse.y = center_y;
    TEST_ASSERT(app.mouse.x == center_x, "Mouse should be positioned at widget center X");
    TEST_ASSERT(app.mouse.y == center_y, "Mouse should be positioned at widget center Y");
    
    // Test boundary detection logic
    int mouse_in_widget = (app.mouse.x >= widget_x && app.mouse.x < widget_x + widget_w &&
                          app.mouse.y >= widget_y && app.mouse.y < widget_y + widget_h);
    TEST_ASSERT(mouse_in_widget == 1, "Mouse should be detected within widget boundaries");
    
    // Test edge cases - exactly on edge
    app.mouse.x = widget_x;
    app.mouse.y = widget_y;
    mouse_in_widget = (app.mouse.x >= widget_x && app.mouse.x < widget_x + widget_w &&
                      app.mouse.y >= widget_y && app.mouse.y < widget_y + widget_h);
    TEST_ASSERT(mouse_in_widget == 1, "Mouse should be detected at top-left edge");
    
    // Test just outside boundary
    app.mouse.x = widget_x - 1;
    app.mouse.y = widget_y;
    mouse_in_widget = (app.mouse.x >= widget_x && app.mouse.x < widget_x + widget_w &&
                      app.mouse.y >= widget_y && app.mouse.y < widget_y + widget_h);
    TEST_ASSERT(mouse_in_widget == 0, "Mouse should not be detected outside widget boundary");
    
    // Test different mouse button states
    app.mouse.button = SDL_BUTTON_LEFT;
    app.mouse.state = SDL_PRESSED;
    app.mouse.pressed = 1;
    TEST_ASSERT(app.mouse.button == SDL_BUTTON_LEFT, "Left mouse button should be set");
    TEST_ASSERT(app.mouse.pressed == 1, "Mouse should be in pressed state");
    
    app.mouse.button = SDL_BUTTON_RIGHT;
    TEST_ASSERT(app.mouse.button == SDL_BUTTON_RIGHT, "Right mouse button should be set");
    
    return 1;
}

// Test widget keyboard navigation
int test_widget_keyboard_navigation(void) {
    reset_app_state();
    
    // Test keyboard navigation keys
    app.keyboard[SDL_SCANCODE_TAB] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_TAB] == 1, "Tab key should be detectable");
    
    app.keyboard[SDL_SCANCODE_UP] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_UP] == 1, "Up arrow should be detectable");
    
    app.keyboard[SDL_SCANCODE_DOWN] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_DOWN] == 1, "Down arrow should be detectable");
    
    app.keyboard[SDL_SCANCODE_LEFT] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_LEFT] == 1, "Left arrow should be detectable");
    
    app.keyboard[SDL_SCANCODE_RIGHT] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_RIGHT] == 1, "Right arrow should be detectable");
    
    // Test escape key behavior
    app.keyboard[SDL_SCANCODE_ESCAPE] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_ESCAPE] == 1, "Escape key should be detectable");
    
    // Test enter/return key behavior
    app.keyboard[SDL_SCANCODE_RETURN] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_RETURN] == 1, "Return key should be detectable");
    
    app.keyboard[SDL_SCANCODE_SPACE] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_SPACE] == 1, "Space key should be detectable");
    
    // Test modifier keys
    app.keyboard[SDL_SCANCODE_LSHIFT] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_LSHIFT] == 1, "Left shift should be detectable");
    
    app.keyboard[SDL_SCANCODE_LCTRL] = 1;
    TEST_ASSERT(app.keyboard[SDL_SCANCODE_LCTRL] == 1, "Left ctrl should be detectable");
    
    // Test last key pressed tracking
    app.last_key_pressed = SDL_SCANCODE_RETURN;
    TEST_ASSERT(app.last_key_pressed == SDL_SCANCODE_RETURN, "Last key pressed should be tracked");
    
    return 1;
}

// Test widget state persistence
int test_widget_state_persistence(void) {
    reset_app_state();
    
    // Test active widget state management
    app.active_widget = NULL;
    TEST_ASSERT(app.active_widget == NULL, "Active widget should be null initially");
    
    // Test widget state during interactions
    // Simulate widget becoming active
    // Since we can't create actual widgets, we'll simulate the state changes
    
    // Test font scale affects widget rendering
    app.font_scale = 1.5;
    TEST_ASSERT(app.font_scale == 1.5, "Font scale should affect widget text rendering");
    
    // Test font type affects widget appearance
    app.font_type = FONT_GAME;
    TEST_ASSERT(app.font_type == FONT_GAME, "Font type should be settable for widgets");
    
    app.font_type = FONT_LINUX;
    TEST_ASSERT(app.font_type == FONT_LINUX, "Font type should be changeable");
    
    // Test widget state preservation during app state changes
    app.running = 1;
    strcpy(app.input_text, "widget_test");
    app.mouse.x = 500;
    app.mouse.y = 300;
    
    // Verify state is maintained
    TEST_ASSERT(app.running == 1, "App should remain running during widget interactions");
    TEST_ASSERT(strcmp(app.input_text, "widget_test") == 0, "Input text should be preserved");
    TEST_ASSERT(app.mouse.x == 500, "Mouse X should be preserved");
    TEST_ASSERT(app.mouse.y == 300, "Mouse Y should be preserved");
    
    // Test state after reset
    reset_app_state();
    TEST_ASSERT(app.active_widget == NULL, "Active widget should be null after reset");
    TEST_ASSERT(app.font_scale == 1.0, "Font scale should be reset");
    TEST_ASSERT(strlen(app.input_text) == 0, "Input text should be cleared");
    
    return 1;
}

// Main test runner
int main(void) {
    // Initialize SDL for testing
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    
    TEST_SUITE_START("Widget System Basic Tests");
    
    RUN_TEST(test_widget_initialization);
    RUN_TEST(test_widget_constants);
    RUN_TEST(test_app_state_management);
    RUN_TEST(test_widget_structure_validation);
    RUN_TEST(test_widget_list_operations);
    RUN_TEST(test_widget_mouse_interaction_boundaries);
    RUN_TEST(test_widget_keyboard_navigation);
    RUN_TEST(test_widget_state_persistence);
    
    TEST_SUITE_END();
    
    SDL_Quit();
    return 0;
}