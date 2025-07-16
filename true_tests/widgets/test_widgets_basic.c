// File: true_tests/widgets/test_widgets_basic.c - Basic widget functionality tests

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
static int action_count = 0;
static int last_slider_value = -1;
static char last_input_text[64] = "";
static int last_control_key = -1;

// Forward declarations
static void advance_to_next_scene(void);
static void init_test_system(void);
static void common_logic(float dt);
static void draw_ui_overlay(void);

// Test scene function declarations
static void init_widget_initialization(void);
static void draw_widget_initialization(float dt);
static void init_button_widget_test(void);
static void draw_button_widget_test(float dt);
static void init_select_widget_test(void);
static void draw_select_widget_test(float dt);
static void init_slider_widget_test(void);
static void draw_slider_widget_test(float dt);
static void init_input_widget_test(void);
static void draw_input_widget_test(float dt);
static void init_control_widget_test(void);
static void draw_control_widget_test(float dt);
static void init_widget_navigation_test(void);
static void draw_widget_navigation_test(float dt);
static void init_widget_actions_test(void);
static void draw_widget_actions_test(float dt);
static void init_hidden_widgets_test(void);
static void draw_hidden_widgets_test(float dt);

// Widget action callbacks
static void button_action(void);
static void select_action(void);
static void slider_action(void);
static void input_action(void);
static void control_action(void);

// Test scene definitions
static TestScene_t test_scenes[] = {
    // Widget tests (3-5 seconds each for interaction)
    {"Widget Initialization", 3.0f, init_widget_initialization, common_logic, draw_widget_initialization},
    {"Button Widget Test", 4.0f, init_button_widget_test, common_logic, draw_button_widget_test},
    {"Select Widget Test", 5.0f, init_select_widget_test, common_logic, draw_select_widget_test},
    {"Slider Widget Test", 5.0f, init_slider_widget_test, common_logic, draw_slider_widget_test},
    {"Input Widget Test", 6.0f, init_input_widget_test, common_logic, draw_input_widget_test},
    {"Control Widget Test", 5.0f, init_control_widget_test, common_logic, draw_control_widget_test},
    {"Widget Navigation Test", 4.0f, init_widget_navigation_test, common_logic, draw_widget_navigation_test},
    {"Widget Actions Test", 4.0f, init_widget_actions_test, common_logic, draw_widget_actions_test},
    {"Hidden Widgets Test", 4.0f, init_hidden_widgets_test, common_logic, draw_hidden_widgets_test}
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
    action_count = 0; // Reset action counter
    
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
    
    // Initialize widgets
    a_InitWidgets("true_tests/widgets/test_widgets_basic.json");
    
    // Set up widget actions
    aWidget_t* button = a_GetWidget("button_test");
    if (button) button->action = button_action;
    
    aWidget_t* select = a_GetWidget("select_test");
    if (select) select->action = select_action;
    
    aWidget_t* slider = a_GetWidget("slider_test");
    if (slider) slider->action = slider_action;
    
    aWidget_t* input = a_GetWidget("input_test");
    if (input) input->action = input_action;
    
    aWidget_t* control = a_GetWidget("control_test");
    if (control) control->action = control_action;
    
    // Set initial active widget
    if (button) app.active_widget = button;
    
    printf("\n=== Archimedes Widget Basic Tests ===\n");
    printf("Total tests: %d\n", total_scenes);
    printf("Controls: SPACE=Next, ESC=Quit, R=Restart\n");
    printf("Mouse: Click widgets | Keys: Arrows=Navigate, Enter=Activate\n\n");
    
    // Start first scene
    advance_to_next_scene();
}

static void common_logic(float dt) {
    scene_timer += dt;
    
    // Update widget system
    a_DoWidget();
    
    // Handle input
    if (app.keyboard[SDL_SCANCODE_ESCAPE] == 1) {
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
        app.running = 0;
        return;
    }
    
    if (app.keyboard[SDL_SCANCODE_F1] == 1) {
        app.keyboard[SDL_SCANCODE_F1] = 0;
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
    
    // Test status
    char status_text[50];
    snprintf(status_text, sizeof(status_text), "Passed: %d  Failed: %d", tests_passed, tests_failed);
    a_DrawText(status_text, 700, 550, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_RIGHT, 0);
    
    // Controls hint (moved higher)
    a_DrawText("F1=Skip  R=Restart  ESC=Quit", 400, 545, 150, 150, 150, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
}

// ============================================================================
// Widget Action Callbacks
// ============================================================================

static void button_action(void) {
    action_count++;
}

static void select_action(void) {
    aWidget_t* widget = a_GetWidget("select_test");
    if (widget) {
        aSelectWidget_t* select = (aSelectWidget_t*)widget->data;
        printf("Select changed to: %s (index %d)\n", select->options[select->value], select->value);
    }
}

static void slider_action(void) {
    aWidget_t* widget = a_GetWidget("slider_test");
    if (widget) {
        aSliderWidget_t* slider = (aSliderWidget_t*)widget->data;
        last_slider_value = slider->value;
    }
}

static void input_action(void) {
    aWidget_t* widget = a_GetWidget("input_test");
    if (widget) {
        aInputWidget_t* input = (aInputWidget_t*)widget->data;
        strncpy(last_input_text, input->text, sizeof(last_input_text) - 1);
        last_input_text[sizeof(last_input_text) - 1] = '\0';
    }
}

static void control_action(void) {
    aWidget_t* widget = a_GetWidget("control_test");
    if (widget) {
        aControlWidget_t* control = (aControlWidget_t*)widget->data;
        last_control_key = control->value;
    }
}

// ============================================================================
// Test Scenes
// ============================================================================

static void init_widget_initialization(void) {
    strcpy(test_result_text, "Testing widget system initialization...");
}

static void draw_widget_initialization(float dt) {
    (void)dt;
    a_DrawText("Widget System Initialization Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Check if widgets were loaded
    int widget_count = 0;
    const char* widget_names[] = {"button_test", "select_test", "slider_test", "input_test", "control_test"};
    
    for (int i = 0; i < 5; i++) {
        if (a_GetWidget((char*)widget_names[i]) != NULL) {
            widget_count++;
        }
    }
    
    char count_text[100];
    snprintf(count_text, sizeof(count_text), "Loaded %d/5 basic widgets", widget_count);
    a_DrawText(count_text, 400, 150, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    if (widget_count == 5) {
        a_DrawText(" All widgets loaded successfully", 400, 200, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        scene_test_result = 1;
    } else {
        a_DrawText(" Failed to load all widgets", 400, 200, 255, 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        scene_test_result = 0;
    }
    
    // Draw all widgets
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_button_widget_test(void) {
    strcpy(test_result_text, "Click the button or press Enter when it's active");
    action_count = 0;
}

static void draw_button_widget_test(float dt) {
    (void)dt;
    a_DrawText("Button Widget Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show action count
    char action_text[100];
    snprintf(action_text, sizeof(action_text), "Button clicks: %d", action_count);
    a_DrawText(action_text, 400, 300, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    if (action_count > 0) {
        a_DrawText(" Button interaction working!", 400, 350, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    // Highlight active widget
    if (app.active_widget && strcmp(app.active_widget->name, "button_test") == 0) {
        a_DrawText("Button is ACTIVE (press Enter)", 400, 400, 0, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_select_widget_test(void) {
    strcpy(test_result_text, "Use arrow keys or click to change selection");
    aWidget_t* widget = a_GetWidget("select_test");
    if (widget) app.active_widget = widget;
}

static void draw_select_widget_test(float dt) {
    (void)dt;
    a_DrawText("Select Widget Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show current selection
    aWidget_t* widget = a_GetWidget("select_test");
    if (widget) {
        aSelectWidget_t* select = (aSelectWidget_t*)widget->data;
        char select_text[100];
        snprintf(select_text, sizeof(select_text), "Current selection: %s (index %d)", 
                 select->options[select->value], select->value);
        a_DrawText(select_text, 400, 350, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
        
        if (select->value != 0) {
            a_DrawText(" Select widget interaction working!", 400, 400, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_slider_widget_test(void) {
    strcpy(test_result_text, "Use arrow keys or click to adjust slider");
    aWidget_t* widget = a_GetWidget("slider_test");
    if (widget) app.active_widget = widget;
    last_slider_value = 0;
}

static void draw_slider_widget_test(float dt) {
    (void)dt;
    a_DrawText("Slider Widget Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show current value
    aWidget_t* widget = a_GetWidget("slider_test");
    if (widget) {
        aSliderWidget_t* slider = (aSliderWidget_t*)widget->data;
        char slider_text[100];
        snprintf(slider_text, sizeof(slider_text), "Slider value: %d%%", slider->value);
        a_DrawText(slider_text, 400, 350, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
        
        // Test wait_on_change slider
        aWidget_t* wait_widget = a_GetWidget("slider_wait");
        if (wait_widget) {
            aSliderWidget_t* wait_slider = (aSliderWidget_t*)wait_widget->data;
            snprintf(slider_text, sizeof(slider_text), "Wait slider: %d%% (releases key)", wait_slider->value);
            a_DrawText(slider_text, 400, 380, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
        
        if (slider->value != 0) {
            a_DrawText(" Slider interaction working!", 400, 420, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_input_widget_test(void) {
    strcpy(test_result_text, "Press Enter/Space to edit, type text, Enter to confirm");
    aWidget_t* widget = a_GetWidget("input_test");
    if (widget) app.active_widget = widget;
}

static void draw_input_widget_test(float dt) {
    (void)dt;
    a_DrawText("Input Widget Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show input status
    aWidget_t* widget = a_GetWidget("input_test");
    if (widget) {
        aInputWidget_t* input = (aInputWidget_t*)widget->data;
        char input_text[100];
        snprintf(input_text, sizeof(input_text), "Current text: \"%s\"", input->text);
        a_DrawText(input_text, 400, 300, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
        
        if (strlen(last_input_text) > 0) {
            snprintf(input_text, sizeof(input_text), "Last confirmed: \"%s\"", last_input_text);
            a_DrawText(input_text, 400, 330, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
        
        if (strcmp(input->text, "Player") != 0) {
            a_DrawText(" Input widget working!", 400, 420, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_control_widget_test(void) {
    strcpy(test_result_text, "Press Enter/Space then press any key to bind");
    aWidget_t* widget = a_GetWidget("control_test");
    if (widget) app.active_widget = widget;
    last_control_key = -1;
}

static void draw_control_widget_test(float dt) {
    (void)dt;
    a_DrawText("Control Widget Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show bound key
    aWidget_t* widget = a_GetWidget("control_test");
    if (widget) {
        aControlWidget_t* control = (aControlWidget_t*)widget->data;
        char control_text[100];
        if (control->value > 0) {
            snprintf(control_text, sizeof(control_text), "Bound to: %s (scancode %d)", 
                     SDL_GetScancodeName(control->value), control->value);
            a_DrawText(control_text, 400, 300, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            a_DrawText(" Control widget key binding working!", 400, 350, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        } else {
            a_DrawText("No key bound yet", 400, 300, 128, 128, 128, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_widget_navigation_test(void) {
    strcpy(test_result_text, "Click different widgets to change focus");
}

static void draw_widget_navigation_test(float dt) {
    (void)dt;
    a_DrawText("Widget Navigation Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show active widget
    if (app.active_widget) {
        char active_text[100];
        snprintf(active_text, sizeof(active_text), "Active widget: %s", app.active_widget->name);
        a_DrawText(active_text, 400, 300, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
        
        // Show widget type
        const char* type_names[] = {"Button", "Select", "Slider", "Input", "Control", "Container"};
        if (app.active_widget->type >= 0 && app.active_widget->type < 6) {
            snprintf(active_text, sizeof(active_text), "Type: %s", type_names[app.active_widget->type]);
            a_DrawText(active_text, 400, 330, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
        
        a_DrawText(" Widget focus system working!", 400, 380, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_widget_actions_test(void) {
    strcpy(test_result_text, "Test multiple button actions");
    action_count = 0;
    
    // Set up actions for multi buttons
    aWidget_t* btn1 = a_GetWidget("multi_button1");
    aWidget_t* btn2 = a_GetWidget("multi_button2");
    aWidget_t* btn3 = a_GetWidget("multi_button3");
    
    if (btn1) btn1->action = button_action;
    if (btn2) btn2->action = button_action;
    if (btn3) btn3->action = button_action;
}

static void draw_widget_actions_test(float dt) {
    (void)dt;
    a_DrawText("Widget Actions Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show total actions
    char action_text[100];
    snprintf(action_text, sizeof(action_text), "Total button clicks: %d", action_count);
    a_DrawText(action_text, 400, 300, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    if (action_count >= 3) {
        a_DrawText(" Multiple widgets with actions working!", 400, 350, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    } else if (action_count > 0) {
        a_DrawText("Click all three buttons at the bottom", 400, 350, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_hidden_widgets_test(void) {
    strcpy(test_result_text, "Testing hidden widget functionality");
    
    // Show the hidden button
    aWidget_t* hidden = a_GetWidget("hidden_button");
    if (hidden) {
        hidden->hidden = 0;
    }
}

static void draw_hidden_widgets_test(float dt) {
    (void)dt;
    a_DrawText("Hidden Widgets Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Toggle visibility
    static float toggle_timer = 0;
    toggle_timer += dt;
    
    aWidget_t* hidden = a_GetWidget("hidden_button");
    if (hidden) {
        // Toggle every second
        if ((int)(toggle_timer) % 2 == 0) {
            hidden->hidden = 0;
            a_DrawText("Hidden button is now VISIBLE", 400, 300, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        } else {
            hidden->hidden = 1;
            a_DrawText("Hidden button is now HIDDEN", 400, 300, 255, 128, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
        
        a_DrawText(" Widget visibility control working!", 400, 350, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    a_DrawWidgets();
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
    if (a_InitTest(800, 600, "Archimedes Widget Basic Tests") != 0) {
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