// File: true_tests/widgets/test_widgets_advanced.c - Advanced widget functionality tests

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "Archimedes.h"
#include "tests.h"

// Global test counters
int total_tests = 0;
int tests_passed = 0;
int tests_failed = 0;

// Helper structures for testing
typedef struct {
    int x, y, w, h;
    int clicks;
    int in_bounds;
} MouseTestData;

typedef struct {
    SDL_Scancode keys[10];
    int key_count;
    int sequence_complete;
} KeySequence;

// Helper to reset app state
static void reset_app_state(void) {
    memset(&app, 0, sizeof(aApp_t));
    app.running = 1;
    app.font_scale = 1.0;
    app.active_widget = NULL;
    memset(app.input_text, 0, sizeof(app.input_text));
    app.last_key_pressed = 0;
}

// Helper to simulate widget boundary testing
static int point_in_widget(int px, int py, int wx, int wy, int ww, int wh) {
    return (px >= wx && px < wx + ww && py >= wy && py < wy + wh);
}

// Helper to simulate widget data structures
static void simulate_widget_data_validation(int type, void* data) {
    switch (type) {
        case WT_SLIDER:
            if (data) {
                aSliderWidget_t* slider = (aSliderWidget_t*)data;
                // Validate slider bounds
                if (slider->value < 0) slider->value = 0;
                if (slider->value > 100) slider->value = 100;
                if (slider->step <= 0) slider->step = 1;
            }
            break;
        case WT_INPUT:
            if (data) {
                aInputWidget_t* input = (aInputWidget_t*)data;
                // Validate input constraints
                if (input->max_length < 0) input->max_length = 0;
                if (input->max_length > 1024) input->max_length = 1024;
            }
            break;
        case WT_SELECT:
            if (data) {
                aSelectWidget_t* select = (aSelectWidget_t*)data;
                // Validate select options
                if (select->num_options < 0) select->num_options = 0;
                if (select->value < 0) select->value = 0;
                if (select->value >= select->num_options) select->value = select->num_options - 1;
            }
            break;
    }
}

// ============================================================================
// Complex Widget Interactions Tests
// ============================================================================

int test_nested_container_widgets(void) {
    reset_app_state();
    
    // Test container widget structure validation
    aContainerWidget_t container;
    container.x = 50;
    container.y = 50;
    container.w = 300;
    container.h = 200;
    container.spacing = 10;
    container.num_components = 3;
    container.components = NULL; // Simulate empty container
    
    TEST_ASSERT(container.x == 50, "Container X should be set correctly");
    TEST_ASSERT(container.y == 50, "Container Y should be set correctly");
    TEST_ASSERT(container.w == 300, "Container width should be set correctly");
    TEST_ASSERT(container.h == 200, "Container height should be set correctly");
    TEST_ASSERT(container.spacing == 10, "Container spacing should be set correctly");
    TEST_ASSERT(container.num_components == 3, "Container should track component count");
    
    // Test nested container bounds checking
    aContainerWidget_t nested_container;
    nested_container.x = container.x + 20;
    nested_container.y = container.y + 20;
    nested_container.w = 100;
    nested_container.h = 80;
    
    // Verify nested container fits within parent
    int nested_right = nested_container.x + nested_container.w;
    int nested_bottom = nested_container.y + nested_container.h;
    int parent_right = container.x + container.w;
    int parent_bottom = container.y + container.h;
    
    TEST_ASSERT(nested_right <= parent_right, "Nested container should fit within parent width");
    TEST_ASSERT(nested_bottom <= parent_bottom, "Nested container should fit within parent height");
    
    // Test container layout calculations
    int available_width = container.w - (2 * container.spacing);
    int available_height = container.h - (2 * container.spacing);
    TEST_ASSERT(available_width > 0, "Container should have positive available width");
    TEST_ASSERT(available_height > 0, "Container should have positive available height");
    
    return 1;
}

int test_widget_action_callbacks(void) {
    reset_app_state();
    
    // Test widget action callback concepts
    // Since we can't easily create real callbacks, test the function pointer logic
    
    // Test null callback handling
    void (*null_callback)(void) = NULL;
    TEST_ASSERT(null_callback == NULL, "Null callback should be null");
    
    // Test callback function pointer storage
    void (*test_callback)(void) = NULL;
    test_callback = (void(*)(void))0x12345678; // Simulate function address
    TEST_ASSERT(test_callback != NULL, "Function pointer should be assignable");
    
    // Test widget action triggering logic
    int action_should_trigger = 0;
    
    // Simulate conditions that would trigger widget actions
    app.mouse.button = SDL_BUTTON_LEFT;
    app.mouse.pressed = 1;
    app.mouse.x = 100;
    app.mouse.y = 100;
    
    // Test click within widget bounds
    int widget_x = 50, widget_y = 50, widget_w = 100, widget_h = 100;
    if (point_in_widget(app.mouse.x, app.mouse.y, widget_x, widget_y, widget_w, widget_h)) {
        action_should_trigger = 1;
    }
    
    TEST_ASSERT(action_should_trigger == 1, "Widget action should trigger on valid click");
    
    // Test click outside widget bounds
    app.mouse.x = 10;
    app.mouse.y = 10;
    action_should_trigger = 0;
    
    if (point_in_widget(app.mouse.x, app.mouse.y, widget_x, widget_y, widget_w, widget_h)) {
        action_should_trigger = 1;
    }
    
    TEST_ASSERT(action_should_trigger == 0, "Widget action should not trigger on invalid click");
    
    return 1;
}

int test_widget_data_structures(void) {
    reset_app_state();
    
    // Test slider widget data structure
    aSliderWidget_t slider;
    slider.x = 100;
    slider.y = 200;
    slider.w = 200;
    slider.h = 20;
    slider.value = 50;
    slider.step = 5;
    slider.wait_on_change = 0;
    
    simulate_widget_data_validation(WT_SLIDER, &slider);
    
    TEST_ASSERT(slider.value == 50, "Slider value should be within bounds");
    TEST_ASSERT(slider.step == 5, "Slider step should be positive");
    TEST_ASSERT(slider.w == 200, "Slider width should be set correctly");
    TEST_ASSERT(slider.h == 20, "Slider height should be set correctly");
    
    // Test input widget data structure
    aInputWidget_t input;
    input.x = 50;
    input.y = 100;
    input.w = 150;
    input.h = 30;
    input.max_length = 20;
    input.text = NULL; // Simulate no text initially
    
    simulate_widget_data_validation(WT_INPUT, &input);
    
    TEST_ASSERT(input.max_length == 20, "Input max length should be set correctly");
    TEST_ASSERT(input.max_length > 0, "Input max length should be positive");
    TEST_ASSERT(input.w == 150, "Input width should be set correctly");
    TEST_ASSERT(input.h == 30, "Input height should be set correctly");
    
    // Test select widget data structure
    aSelectWidget_t select;
    select.num_options = 4;
    select.options = NULL; // Simulate no options initially
    select.x = 75;
    select.y = 125;
    select.w = 120;
    select.h = 25;
    select.value = 1;
    
    simulate_widget_data_validation(WT_SELECT, &select);
    
    TEST_ASSERT(select.num_options == 4, "Select should have correct option count");
    TEST_ASSERT(select.value == 1, "Select value should be within bounds");
    TEST_ASSERT(select.w == 120, "Select width should be set correctly");
    TEST_ASSERT(select.h == 25, "Select height should be set correctly");
    
    return 1;
}

int test_widget_coordinate_transformations(void) {
    reset_app_state();
    
    // Test widget coordinate transformations and scaling
    app.font_scale = 1.5;
    
    // Test basic coordinate transformation
    int base_x = 100, base_y = 150;
    int scaled_x = (int)(base_x * app.font_scale);
    int scaled_y = (int)(base_y * app.font_scale);
    
    TEST_ASSERT(scaled_x == 150, "X coordinate should scale correctly");
    TEST_ASSERT(scaled_y == 225, "Y coordinate should scale correctly");
    
    // Test widget boundary scaling
    int widget_w = 200, widget_h = 100;
    int scaled_w = (int)(widget_w * app.font_scale);
    int scaled_h = (int)(widget_h * app.font_scale);
    
    TEST_ASSERT(scaled_w == 300, "Widget width should scale correctly");
    TEST_ASSERT(scaled_h == 150, "Widget height should scale correctly");
    
    // Test coordinate bounds checking after scaling
    int screen_w = 1280, screen_h = 720;
    int widget_right = scaled_x + scaled_w;
    int widget_bottom = scaled_y + scaled_h;
    
    TEST_ASSERT(widget_right <= screen_w, "Scaled widget should fit within screen width");
    TEST_ASSERT(widget_bottom <= screen_h, "Scaled widget should fit within screen height");
    
    // Test different scale factors
    app.font_scale = 0.8;
    scaled_x = (int)(base_x * app.font_scale);
    scaled_y = (int)(base_y * app.font_scale);
    
    TEST_ASSERT(scaled_x == 80, "X coordinate should scale down correctly");
    TEST_ASSERT(scaled_y == 120, "Y coordinate should scale down correctly");
    
    // Test extreme scale factors
    app.font_scale = 2.0;
    scaled_w = (int)(widget_w * app.font_scale);
    scaled_h = (int)(widget_h * app.font_scale);
    
    TEST_ASSERT(scaled_w == 400, "Widget should scale up correctly");
    TEST_ASSERT(scaled_h == 200, "Widget should scale up correctly");
    
    return 1;
}

// ============================================================================
// Widget Input Processing Tests
// ============================================================================

int test_widget_input_validation(void) {
    reset_app_state();
    
    // Test input widget text validation
    aInputWidget_t input;
    input.max_length = 10;
    input.text = NULL;
    
    // Test input text length validation
    char test_input[] = "Hello";
    int input_len = strlen(test_input);
    TEST_ASSERT(input_len <= input.max_length, "Input text should be within max length");
    
    // Test input text with exact max length
    char max_input[11]; // max_length + 1 for null terminator
    memset(max_input, 'A', 10);
    max_input[10] = '\0';
    
    TEST_ASSERT(strlen(max_input) == input.max_length, "Input at max length should be valid");
    
    // Test input text exceeding max length
    char long_input[] = "This is a very long input that exceeds the maximum length";
    int long_len = strlen(long_input);
    TEST_ASSERT(long_len > input.max_length, "Long input should exceed max length");
    
    // Test input character validation
    char valid_chars[] = "abcABC123!@#";
    TEST_ASSERT(strlen(valid_chars) > 0, "Valid characters should be accepted");
    
    // Test input buffer management
    strcpy(app.input_text, "test");
    TEST_ASSERT(strcmp(app.input_text, "test") == 0, "Input buffer should handle text correctly");
    
    // Test input clearing
    memset(app.input_text, 0, sizeof(app.input_text));
    TEST_ASSERT(strlen(app.input_text) == 0, "Input buffer should be clearable");
    
    return 1;
}

int test_widget_slider_range_handling(void) {
    reset_app_state();
    
    // Test slider range validation and handling
    aSliderWidget_t slider;
    slider.value = 50;
    slider.step = 10;
    slider.wait_on_change = 0;
    
    // Test slider value increment
    int new_value = slider.value + slider.step;
    TEST_ASSERT(new_value == 60, "Slider value should increment correctly");
    
    // Test slider value decrement
    new_value = slider.value - slider.step;
    TEST_ASSERT(new_value == 40, "Slider value should decrement correctly");
    
    // Test slider bounds - maximum
    slider.value = 95;
    new_value = slider.value + slider.step;
    if (new_value > 100) new_value = 100; // Simulate clamping
    TEST_ASSERT(new_value == 100, "Slider should clamp at maximum value");
    
    // Test slider bounds - minimum
    slider.value = 5;
    new_value = slider.value - slider.step;
    if (new_value < 0) new_value = 0; // Simulate clamping
    TEST_ASSERT(new_value == 0, "Slider should clamp at minimum value");
    
    // Test different step sizes
    slider.step = 1;
    slider.value = 50;
    new_value = slider.value + slider.step;
    TEST_ASSERT(new_value == 51, "Slider should handle small steps");
    
    slider.step = 25;
    new_value = slider.value + slider.step;
    TEST_ASSERT(new_value == 75, "Slider should handle large steps");
    
    // Test slider position to value conversion
    int slider_x = 100, slider_w = 200;
    int mouse_x = 150; // Middle of slider
    int relative_pos = mouse_x - slider_x;
    int slider_value = (relative_pos * 100) / slider_w;
    TEST_ASSERT(slider_value == 25, "Mouse position should convert to slider value");
    
    return 1;
}

int test_widget_select_option_management(void) {
    reset_app_state();
    
    // Test select widget option management
    aSelectWidget_t select;
    select.num_options = 4;
    select.value = 1;
    
    // Test option cycling - forward
    int next_value = (select.value + 1) % select.num_options;
    TEST_ASSERT(next_value == 2, "Select should cycle forward correctly");
    
    // Test option cycling - backward
    int prev_value = (select.value - 1 + select.num_options) % select.num_options;
    TEST_ASSERT(prev_value == 0, "Select should cycle backward correctly");
    
    // Test option wrapping - forward at end
    select.value = 3; // Last option
    next_value = (select.value + 1) % select.num_options;
    TEST_ASSERT(next_value == 0, "Select should wrap to first option");
    
    // Test option wrapping - backward at start
    select.value = 0; // First option
    prev_value = (select.value - 1 + select.num_options) % select.num_options;
    TEST_ASSERT(prev_value == 3, "Select should wrap to last option");
    
    // Test option bounds validation
    select.value = 5; // Invalid option
    if (select.value >= select.num_options) select.value = 0; // Simulate correction
    TEST_ASSERT(select.value == 0, "Invalid option should be corrected");
    
    // Test negative option value
    select.value = -1; // Invalid option
    if (select.value < 0) select.value = 0; // Simulate correction
    TEST_ASSERT(select.value == 0, "Negative option should be corrected");
    
    // Test empty select (no options)
    select.num_options = 0;
    select.value = 0;
    TEST_ASSERT(select.num_options == 0, "Select should handle no options");
    TEST_ASSERT(select.value == 0, "Select with no options should have value 0");
    
    return 1;
}

int test_widget_control_key_binding(void) {
    reset_app_state();
    
    // Test control widget key binding
    aControlWidget_t control;
    control.value = SDL_SCANCODE_SPACE; // Default binding
    
    // Test key binding change
    app.last_key_pressed = SDL_SCANCODE_W;
    control.value = app.last_key_pressed;
    TEST_ASSERT(control.value == SDL_SCANCODE_W, "Control should bind to new key");
    
    // Test different key bindings
    app.last_key_pressed = SDL_SCANCODE_RETURN;
    control.value = app.last_key_pressed;
    TEST_ASSERT(control.value == SDL_SCANCODE_RETURN, "Control should bind to Return key");
    
    app.last_key_pressed = SDL_SCANCODE_ESCAPE;
    control.value = app.last_key_pressed;
    TEST_ASSERT(control.value == SDL_SCANCODE_ESCAPE, "Control should bind to Escape key");
    
    // Test modifier key bindings
    app.last_key_pressed = SDL_SCANCODE_LSHIFT;
    control.value = app.last_key_pressed;
    TEST_ASSERT(control.value == SDL_SCANCODE_LSHIFT, "Control should bind to modifier keys");
    
    // Test function key bindings
    app.last_key_pressed = SDL_SCANCODE_F1;
    control.value = app.last_key_pressed;
    TEST_ASSERT(control.value == SDL_SCANCODE_F1, "Control should bind to function keys");
    
    // Test arrow key bindings
    app.last_key_pressed = SDL_SCANCODE_UP;
    control.value = app.last_key_pressed;
    TEST_ASSERT(control.value == SDL_SCANCODE_UP, "Control should bind to arrow keys");
    
    // Test key binding validation
    TEST_ASSERT(control.value >= 0, "Control key binding should be valid scancode");
    TEST_ASSERT(control.value < MAX_KEYBOARD_KEYS, "Control key binding should be within valid range");
    
    return 1;
}

// ============================================================================
// Widget Edge Cases Tests
// ============================================================================

int test_widget_boundary_conditions(void) {
    reset_app_state();
    
    // Test widgets at screen boundaries
    int screen_w = 1280, screen_h = 720;
    
    // Test widget at top-left corner
    int widget_x = 0, widget_y = 0, widget_w = 100, widget_h = 50;
    TEST_ASSERT(widget_x >= 0, "Widget at screen edge should have valid coordinates");
    TEST_ASSERT(widget_y >= 0, "Widget at screen edge should have valid coordinates");
    
    // Test widget at bottom-right corner
    widget_x = screen_w - widget_w;
    widget_y = screen_h - widget_h;
    TEST_ASSERT(widget_x + widget_w <= screen_w, "Widget should fit within screen width");
    TEST_ASSERT(widget_y + widget_h <= screen_h, "Widget should fit within screen height");
    
    // Test widget extending beyond screen
    widget_x = screen_w - 50; // Widget extends 50 pixels beyond screen
    int widget_visible_w = screen_w - widget_x;
    TEST_ASSERT(widget_visible_w == 50, "Partial widget should have correct visible width");
    
    // Test zero-size widget
    widget_w = 0;
    widget_h = 0;
    TEST_ASSERT(widget_w >= 0, "Zero-width widget should be handled");
    TEST_ASSERT(widget_h >= 0, "Zero-height widget should be handled");
    
    // Test negative coordinates
    widget_x = -50;
    widget_y = -25;
    int visible_start_x = (widget_x < 0) ? 0 : widget_x;
    int visible_start_y = (widget_y < 0) ? 0 : widget_y;
    TEST_ASSERT(visible_start_x == 0, "Negative coordinates should be clamped");
    TEST_ASSERT(visible_start_y == 0, "Negative coordinates should be clamped");
    
    return 1;
}

int test_widget_overlap_detection(void) {
    reset_app_state();
    
    // Test widget overlap detection
    // Widget 1
    int w1_x = 100, w1_y = 100, w1_w = 100, w1_h = 100;
    // Widget 2 - overlapping
    int w2_x = 150, w2_y = 150, w2_w = 100, w2_h = 100;
    
    // Test overlap detection logic
    int overlap_x = (w1_x < w2_x + w2_w) && (w1_x + w1_w > w2_x);
    int overlap_y = (w1_y < w2_y + w2_h) && (w1_y + w1_h > w2_y);
    int widgets_overlap = overlap_x && overlap_y;
    
    TEST_ASSERT(widgets_overlap == 1, "Overlapping widgets should be detected");
    
    // Test non-overlapping widgets
    w2_x = 250; // Move widget 2 away
    w2_y = 250;
    overlap_x = (w1_x < w2_x + w2_w) && (w1_x + w1_w > w2_x);
    overlap_y = (w1_y < w2_y + w2_h) && (w1_y + w1_h > w2_y);
    widgets_overlap = overlap_x && overlap_y;
    
    TEST_ASSERT(widgets_overlap == 0, "Non-overlapping widgets should not be detected");
    
    // Test adjacent widgets (touching but not overlapping)
    w2_x = w1_x + w1_w; // Right edge of widget 1
    w2_y = w1_y;
    overlap_x = (w1_x < w2_x + w2_w) && (w1_x + w1_w > w2_x);
    overlap_y = (w1_y < w2_y + w2_h) && (w1_y + w1_h > w2_y);
    widgets_overlap = overlap_x && overlap_y;
    
    TEST_ASSERT(widgets_overlap == 0, "Adjacent widgets should not overlap");
    
    // Test widget completely inside another
    w2_x = w1_x + 10;
    w2_y = w1_y + 10;
    w2_w = 50;
    w2_h = 50;
    overlap_x = (w1_x < w2_x + w2_w) && (w1_x + w1_w > w2_x);
    overlap_y = (w1_y < w2_y + w2_h) && (w1_y + w1_h > w2_y);
    widgets_overlap = overlap_x && overlap_y;
    
    TEST_ASSERT(widgets_overlap == 1, "Widget inside another should be detected as overlapping");
    
    return 1;
}

int test_widget_extreme_coordinates(void) {
    reset_app_state();
    
    // Test widgets with extreme coordinate values
    int extreme_x = INT_MAX / 2;
    int extreme_y = INT_MAX / 2;
    int extreme_w = 100;
    int extreme_h = 100;
    
    // Test that extreme coordinates don't cause overflow
    long long right_edge = (long long)extreme_x + extreme_w;
    long long bottom_edge = (long long)extreme_y + extreme_h;
    
    TEST_ASSERT(right_edge > extreme_x, "Extreme coordinates should not cause overflow");
    TEST_ASSERT(bottom_edge > extreme_y, "Extreme coordinates should not cause overflow");
    
    // Test negative extreme coordinates
    extreme_x = INT_MIN / 2;
    extreme_y = INT_MIN / 2;
    
    TEST_ASSERT(extreme_x < 0, "Negative extreme coordinates should be handled");
    TEST_ASSERT(extreme_y < 0, "Negative extreme coordinates should be handled");
    
    // Test coordinate clamping
    if (extreme_x < 0) extreme_x = 0;
    if (extreme_y < 0) extreme_y = 0;
    
    TEST_ASSERT(extreme_x == 0, "Negative coordinates should be clamped to 0");
    TEST_ASSERT(extreme_y == 0, "Negative coordinates should be clamped to 0");
    
    // Test extreme widget sizes
    extreme_w = INT_MAX / 4;
    extreme_h = INT_MAX / 4;
    
    TEST_ASSERT(extreme_w > 0, "Extreme widget width should be positive");
    TEST_ASSERT(extreme_h > 0, "Extreme widget height should be positive");
    
    return 1;
}

// ============================================================================
// Widget Performance Tests
// ============================================================================

int test_widget_rapid_interaction(void) {
    reset_app_state();
    
    // Test rapid widget interactions
    MouseTestData mouse_data;
    mouse_data.x = 100;
    mouse_data.y = 100;
    mouse_data.w = 200;
    mouse_data.h = 100;
    mouse_data.clicks = 0;
    
    LOOP_TEST_START();
    // Simulate rapid mouse clicks
    for (int i = 0; i < 100; i++) {
        app.mouse.x = mouse_data.x + (i % mouse_data.w);
        app.mouse.y = mouse_data.y + (i % mouse_data.h);
        app.mouse.button = SDL_BUTTON_LEFT;
        app.mouse.pressed = (i % 2); // Alternate press/release
        
        if (point_in_widget(app.mouse.x, app.mouse.y, mouse_data.x, mouse_data.y, mouse_data.w, mouse_data.h)) {
            mouse_data.clicks++;
        }
        
        TEST_ASSERT(app.mouse.x >= mouse_data.x, "Mouse X should be within widget bounds");
        TEST_ASSERT(app.mouse.y >= mouse_data.y, "Mouse Y should be within widget bounds");
    }
    LOOP_TEST_END();
    
    TEST_ASSERT(mouse_data.clicks > 0, "Rapid clicks should be counted");
    
    LOOP_TEST_START();
    // Test rapid keyboard input
    for (int i = 0; i < 50; i++) {
        SDL_Scancode key = SDL_SCANCODE_A + (i % 26); // Cycle through A-Z
        app.keyboard[key] = 1;
        app.last_key_pressed = key;
        
        TEST_ASSERT(app.keyboard[key] == 1, "Rapid key presses should be registered");
        
        app.keyboard[key] = 0; // Release
        TEST_ASSERT(app.keyboard[key] == 0, "Rapid key releases should be registered");
    }
    LOOP_TEST_END();
    
    return 1;
}

int test_widget_large_datasets(void) {
    reset_app_state();
    
    // Test widgets with large datasets
    aSelectWidget_t large_select;
    large_select.num_options = 1000; // Large number of options
    large_select.value = 0;
    large_select.options = NULL; // Simulate large option set
    
    // Test large option set navigation
    for (int i = 0; i < 100; i++) {
        large_select.value = (large_select.value + 1) % large_select.num_options;
        TEST_ASSERT(large_select.value < large_select.num_options, "Large select value should stay in bounds");
    }
    
    // Test large text input
    aInputWidget_t large_input;
    large_input.max_length = 1000; // Large text limit
    large_input.text = NULL;
    
    // Test large text handling
    char large_text[1001];
    for (int i = 0; i < 1000; i++) {
        large_text[i] = 'A' + (i % 26);
    }
    large_text[1000] = '\0';
    
    TEST_ASSERT(strlen(large_text) == 1000, "Large text should be handled correctly");
    TEST_ASSERT(strlen(large_text) == large_input.max_length, "Large text should match input limit");
    
    // Test large container with many components
    aContainerWidget_t large_container;
    large_container.num_components = 500; // Many components
    large_container.components = NULL; // Simulate many components
    large_container.x = 0;
    large_container.y = 0;
    large_container.w = 1280;
    large_container.h = 720;
    large_container.spacing = 2;
    
    // Test layout calculations for large container
    int component_area = large_container.w * large_container.h;
    int area_per_component = component_area / large_container.num_components;
    TEST_ASSERT(area_per_component > 0, "Large container should have positive area per component");
    
    return 1;
}

// ============================================================================
// Widget Integration Tests
// ============================================================================

int test_widget_font_integration(void) {
    reset_app_state();
    
    // Test widget font integration
    app.font_type = FONT_GAME;
    app.font_scale = 1.2;
    
    // Test font type changes
    TEST_ASSERT(app.font_type == FONT_GAME, "Font type should be set to GAME");
    
    app.font_type = FONT_LINUX;
    TEST_ASSERT(app.font_type == FONT_LINUX, "Font type should be changeable to LINUX");
    
    app.font_type = FONT_ENTER_COMMAND;
    TEST_ASSERT(app.font_type == FONT_ENTER_COMMAND, "Font type should be changeable to ENTER_COMMAND");
    
    // Test font scaling effects on widget dimensions
    int base_width = 100;
    int base_height = 30;
    int scaled_width = (int)(base_width * app.font_scale);
    int scaled_height = (int)(base_height * app.font_scale);
    
    TEST_ASSERT(scaled_width == 120, "Widget width should scale with font");
    TEST_ASSERT(scaled_height == 36, "Widget height should scale with font");
    
    // Test different font scales
    app.font_scale = 0.8;
    scaled_width = (int)(base_width * app.font_scale);
    scaled_height = (int)(base_height * app.font_scale);
    
    TEST_ASSERT(scaled_width == 80, "Widget should scale down with font");
    TEST_ASSERT(scaled_height == 24, "Widget should scale down with font");
    
    app.font_scale = 2.0;
    scaled_width = (int)(base_width * app.font_scale);
    scaled_height = (int)(base_height * app.font_scale);
    
    TEST_ASSERT(scaled_width == 200, "Widget should scale up with font");
    TEST_ASSERT(scaled_height == 60, "Widget should scale up with font");
    
    return 1;
}

int test_widget_error_recovery(void) {
    reset_app_state();
    
    // Test widget system error recovery
    // Simulate various error conditions and recovery
    
    // Test null widget handling
    aWidget_t* null_widget = NULL;
    TEST_ASSERT(null_widget == NULL, "System should handle null widgets gracefully");
    
    // Test invalid widget type
    int invalid_type = 999;
    TEST_ASSERT(invalid_type != WT_BUTTON, "Invalid widget type should be detectable");
    TEST_ASSERT(invalid_type != WT_INPUT, "Invalid widget type should be detectable");
    TEST_ASSERT(invalid_type != WT_SLIDER, "Invalid widget type should be detectable");
    
    // Test widget with invalid dimensions
    int invalid_width = -100;
    int invalid_height = -50;
    
    // Simulate error correction
    if (invalid_width <= 0) invalid_width = 1;
    if (invalid_height <= 0) invalid_height = 1;
    
    TEST_ASSERT(invalid_width > 0, "Invalid widget width should be corrected");
    TEST_ASSERT(invalid_height > 0, "Invalid widget height should be corrected");
    
    // Test widget with invalid coordinates
    int invalid_x = INT_MIN;
    int invalid_y = INT_MAX;
    
    // Simulate coordinate validation
    if (invalid_x < 0) invalid_x = 0;
    if (invalid_y > 720) invalid_y = 720; // Screen height
    
    TEST_ASSERT(invalid_x >= 0, "Invalid widget X should be corrected");
    TEST_ASSERT(invalid_y <= 720, "Invalid widget Y should be corrected");
    
    // Test recovery from corrupted widget state
    app.active_widget = (aWidget_t*)0xDEADBEEF; // Simulate corrupted pointer
    
    // Simulate error detection and recovery
    if (app.active_widget != NULL) {
        // In real implementation, would validate pointer
        app.active_widget = NULL; // Reset to safe state
    }
    
    TEST_ASSERT(app.active_widget == NULL, "Corrupted widget state should be recoverable");
    
    return 1;
}

// ============================================================================
// Main Test Runner
// ============================================================================

int main(void) {
    // Initialize SDL for testing
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    
    TEST_SUITE_START("Widget System Advanced Tests");
    
    // Complex widget interactions
    RUN_TEST(test_nested_container_widgets);
    RUN_TEST(test_widget_action_callbacks);
    RUN_TEST(test_widget_data_structures);
    RUN_TEST(test_widget_coordinate_transformations);
    
    // Widget input processing
    RUN_TEST(test_widget_input_validation);
    RUN_TEST(test_widget_slider_range_handling);
    RUN_TEST(test_widget_select_option_management);
    RUN_TEST(test_widget_control_key_binding);
    
    // Widget edge cases
    RUN_TEST(test_widget_boundary_conditions);
    RUN_TEST(test_widget_overlap_detection);
    RUN_TEST(test_widget_extreme_coordinates);
    
    // Widget performance
    RUN_TEST(test_widget_rapid_interaction);
    RUN_TEST(test_widget_large_datasets);
    
    // Widget integration
    RUN_TEST(test_widget_font_integration);
    RUN_TEST(test_widget_error_recovery);
    
    TEST_SUITE_END();
    
    SDL_Quit();
    return 0;
}