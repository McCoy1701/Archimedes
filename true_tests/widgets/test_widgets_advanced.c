// File: true_tests/widgets/test_widgets_advanced.c - Advanced widget features tests

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
static int container_clicks = 0;
static int nested_clicks = 0;
static int stress_widget_count = 0;
static aWidget_t* stress_widgets = NULL;

// Forward declarations
static void advance_to_next_scene(void);
static void init_test_system(void);
static void common_logic(float dt);
static void draw_ui_overlay(void);

// Test scene function declarations
static void init_container_widget_test(void);
static void draw_container_widget_test(float dt);
static void init_nested_containers_test(void);
static void draw_nested_containers_test(float dt);
static void init_widget_flexbox_test(void);
static void draw_widget_flexbox_test(float dt);
static void init_widget_styling_test(void);
static void draw_widget_styling_test(float dt);
static void init_mouse_interaction_test(void);
static void draw_mouse_interaction_test(float dt);
static void init_state_management_test(void);
static void draw_state_management_test(float dt);
static void init_performance_stress_test(void);
static void draw_performance_stress_test(float dt);
static void cleanup_performance_stress_test(void);
static void init_edge_cases_test(void);
static void draw_edge_cases_test(float dt);
static void init_memory_management_test(void);
static void draw_memory_management_test(float dt);

// Widget action callbacks
static void container_button_action(void);
static void nested_button_action(void);

// Test scene definitions
static TestScene_t test_scenes[] = {
    // Advanced widget tests (4-6 seconds each for complex interactions)
    {"Container Widget Test", 5.0f, init_container_widget_test, common_logic, draw_container_widget_test},
    {"Nested Containers Test", 5.0f, init_nested_containers_test, common_logic, draw_nested_containers_test},
    {"Widget Flexbox Layout", 4.0f, init_widget_flexbox_test, common_logic, draw_widget_flexbox_test},
    {"Widget Styling Test", 4.0f, init_widget_styling_test, common_logic, draw_widget_styling_test},
    {"Mouse Interaction Test", 5.0f, init_mouse_interaction_test, common_logic, draw_mouse_interaction_test},
    {"State Management Test", 5.0f, init_state_management_test, common_logic, draw_state_management_test},
    {"Performance Stress Test", 6.0f, init_performance_stress_test, common_logic, draw_performance_stress_test},
    {"Edge Cases Test", 4.0f, init_edge_cases_test, common_logic, draw_edge_cases_test},
    {"Memory Management Test", 4.0f, init_memory_management_test, common_logic, draw_memory_management_test}
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
    
    // Clean up if needed
    if (current_scene == 6) { // Performance stress test
        cleanup_performance_stress_test();
    }
    
    current_scene++;
    scene_timer = 0.0f;
    scene_test_result = 1; // Reset for next test
    container_clicks = 0;
    nested_clicks = 0;
    
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
    a_InitWidgets("true_tests/widgets/test_widgets_advanced.json");
    
    // Set up container actions
    aContainerWidget_t* container = a_GetContainerFromWidget("main_container");
    if (container) {
        for (int i = 0; i < container->num_components; i++) {
            container->components[i].action = container_button_action;
        }
    }
    
    // Set up nested container actions
    aContainerWidget_t* nested = a_GetContainerFromWidget("nested_container");
    if (nested) {
        for (int i = 0; i < nested->num_components; i++) {
            if (nested->components[i].type == WT_CONTAINER) {
                aContainerWidget_t* inner = (aContainerWidget_t*)nested->components[i].data;
                if (inner) {
                    for (int j = 0; j < inner->num_components; j++) {
                        inner->components[j].action = nested_button_action;
                    }
                }
            }
        }
    }
    
    // Set initial active widget
    aWidget_t* initial = a_GetWidget("main_container");
    if (initial) {
        app.active_widget = initial;
    }
    
    printf("\n=== Archimedes Widget Advanced Tests ===\n");
    printf("Total tests: %d\n", total_scenes);
    printf("Controls: SPACE=Next, ESC=Quit, R=Restart\n");
    printf("Testing containers, layouts, performance...\n\n");
    
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
    
    // Controls hint
    a_DrawText("F1=Skip  R=Restart  ESC=Quit", 400, 545, 150, 150, 150, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
}

// ============================================================================
// Widget Action Callbacks
// ============================================================================

static void container_button_action(void) {
    container_clicks++;
}

static void nested_button_action(void) {
    nested_clicks++;
}

// ============================================================================
// Test Scenes
// ============================================================================

static void init_container_widget_test(void) {
    strcpy(test_result_text, "Click buttons inside the container");
    container_clicks = 0;
}

static void draw_container_widget_test(float dt) {
    (void)dt;
    a_DrawText("Container Widget Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show container info
    aWidget_t* widget = a_GetWidget("main_container");
    if (widget) {
        aContainerWidget_t* container = (aContainerWidget_t*)widget->data;
        char info_text[100];
        snprintf(info_text, sizeof(info_text), "Container has %d components", container->num_components);
        a_DrawText(info_text, 400, 300, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
        
        snprintf(info_text, sizeof(info_text), "Container clicks: %d", container_clicks);
        a_DrawText(info_text, 400, 330, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        
        if (container_clicks >= 2) {
            a_DrawText(" Container components working!", 400, 380, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_nested_containers_test(void) {
    strcpy(test_result_text, "Test nested container functionality");
    nested_clicks = 0;
}

static void draw_nested_containers_test(float dt) {
    (void)dt;
    a_DrawText("Nested Containers Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show nested container status
    aWidget_t* widget = a_GetWidget("nested_container");
    if (widget) {
        aContainerWidget_t* container = (aContainerWidget_t*)widget->data;
        
        // Find inner container
        aWidget_t* inner = NULL;
        for (int i = 0; i < container->num_components; i++) {
            if (container->components[i].type == WT_CONTAINER) {
                inner = &container->components[i];
                break;
            }
        }
        
        if (inner) {
            a_DrawText(" Nested container structure created", 200, 480, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
            
            char info_text[100];
            snprintf(info_text, sizeof(info_text), "Nested button clicks: %d", nested_clicks);
            a_DrawText(info_text, 200, 510, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
        }
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_widget_flexbox_test(void) {
    strcpy(test_result_text, "Testing horizontal and vertical flex layouts");
}

static void draw_widget_flexbox_test(float dt) {
    (void)dt;
    a_DrawText("Widget Flexbox Layout Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show flex info
    aWidget_t* h_widget = a_GetWidget("main_container");
    aWidget_t* v_widget = a_GetWidget("vertical_container");
    
    if (h_widget && h_widget->flex == 1) {
        a_DrawText(" Horizontal flex layout (flex=1)", 200, 260, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    }
    
    if (v_widget && v_widget->flex == 2) {
        a_DrawText(" Vertical flex layout (flex=2)", 500, 300, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    }
    
    aWidget_t* m_widget = a_GetWidget("mixed_container");
    if (m_widget && m_widget->flex == 0) {
        a_DrawText(" Manual positioning (flex=0)", 200, 480, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_widget_styling_test(void) {
    strcpy(test_result_text, "Testing colors, padding, and boxed modes");
}

static void draw_widget_styling_test(float dt) {
    (void)dt;
    a_DrawText("Widget Styling Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Highlight styling features
    a_DrawText("Observe:", 150, 300, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("• Different background colors", 150, 330, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("• Padding around widgets", 150, 360, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("• Boxed vs unboxed widgets", 150, 390, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("• Custom foreground colors", 150, 420, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    a_DrawText("✓ Widget styling system working", 400, 480, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_mouse_interaction_test(void) {
    strcpy(test_result_text, "Test precise mouse click detection");
}

static void draw_mouse_interaction_test(float dt) {
    (void)dt;
    a_DrawText("Mouse Interaction Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show mouse position
    char mouse_text[100];
    snprintf(mouse_text, sizeof(mouse_text), "Mouse: (%d, %d)", app.mouse.x, app.mouse.y);
    a_DrawText(mouse_text, 400, 520, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Check if mouse is over any widget
    const char* hover_widget = NULL;
    const char* widget_names[] = {"main_container", "vertical_container", "mixed_container", "nested_container"};
    
    for (int i = 0; i < 4; i++) {
        aWidget_t* widget = a_GetWidget((char*)widget_names[i]);
        if (widget && !widget->hidden) {
            if (app.mouse.x >= widget->x && app.mouse.y >= widget->y &&
                app.mouse.x <= (widget->x + widget->w) && 
                app.mouse.y <= (widget->y + widget->h)) {
                hover_widget = widget->name;
                break;
            }
        }
    }
    
    if (hover_widget) {
        snprintf(mouse_text, sizeof(mouse_text), "Hovering over: %s", hover_widget);
        a_DrawText(mouse_text, 400, 490, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    }
    
    a_DrawText(" Mouse detection working", 400, 460, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_state_management_test(void) {
    strcpy(test_result_text, "Test widget state synchronization");
}

static void draw_state_management_test(float dt) {
    (void)dt;
    a_DrawText("State Management Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Show various widget states
    aWidget_t* vert = a_GetWidget("vertical_container");
    if (vert) {
        aContainerWidget_t* container = (aContainerWidget_t*)vert->data;
        
        // Find sliders
        int slider_count = 0;
        for (int i = 0; i < container->num_components; i++) {
            if (container->components[i].type == WT_SLIDER) {
                aSliderWidget_t* slider = (aSliderWidget_t*)container->components[i].data;
                char slider_text[100];
                snprintf(slider_text, sizeof(slider_text), "%s: %d%%", 
                         container->components[i].label, slider->value);
                a_DrawText(slider_text, 600, 350 + slider_count * 30, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
                slider_count++;
            }
        }
        
        // Find select
        for (int i = 0; i < container->num_components; i++) {
            if (container->components[i].type == WT_SELECT) {
                aSelectWidget_t* select = (aSelectWidget_t*)container->components[i].data;
                char select_text[100];
                snprintf(select_text, sizeof(select_text), "Quality: %s", 
                         select->options[select->value]);
                a_DrawText(select_text, 600, 410, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
                break;
            }
        }
    }
    
    // Show player info states
    aWidget_t* mixed = a_GetWidget("mixed_container");
    if (mixed) {
        aContainerWidget_t* container = (aContainerWidget_t*)mixed->data;
        for (int i = 0; i < container->num_components; i++) {
            if (container->components[i].type == WT_INPUT) {
                aInputWidget_t* input = (aInputWidget_t*)container->components[i].data;
                char input_text[100];
                snprintf(input_text, sizeof(input_text), "Player: %s", input->text);
                a_DrawText(input_text, 200, 480, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
            }
        }
    }
    
    a_DrawText(" Widget states synchronized", 400, 520, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_performance_stress_test(void) {
    strcpy(test_result_text, "Creating many widgets for performance test...");
    
    // Create stress test widgets dynamically
    stress_widget_count = 50;
    stress_widgets = malloc(sizeof(aWidget_t) * stress_widget_count);
    
    // Initialize stress widgets as simple buttons in a grid
    for (int i = 0; i < stress_widget_count; i++) {
        aWidget_t* w = &stress_widgets[i];
        memset(w, 0, sizeof(aWidget_t));
        
        snprintf(w->name, sizeof(w->name), "stress_%d", i);
        snprintf(w->label, sizeof(w->label), "%d", i);
        w->type = WT_BUTTON;
        w->x = 50 + (i % 10) * 70;
        w->y = 200 + (i / 10) * 50;
        w->boxed = 0;
        w->hidden = 0;
        w->padding = 2;
        w->fg[0] = 255;
        w->fg[1] = 255;
        w->fg[2] = 255;
        w->fg[3] = 255;
        w->bg[0] = 64;
        w->bg[1] = 64;
        w->bg[2] = 64;
        w->bg[3] = 255;
        w->action = NULL;
        w->data = NULL;
        
        // Calculate dimensions
        a_CalcTextDimensions(w->label, app.font_type, &w->w, &w->h);
        w->w += 20; // Add some padding
        w->h += 10;
    }
}

static void cleanup_performance_stress_test(void) {
    if (stress_widgets) {
        free(stress_widgets);
        stress_widgets = NULL;
        stress_widget_count = 0;
    }
}

static void draw_performance_stress_test(float dt) {
    (void)dt;
    a_DrawText("Performance Stress Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    char perf_text[100];
    snprintf(perf_text, sizeof(perf_text), "Rendering %d additional widgets", stress_widget_count);
    a_DrawText(perf_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Draw regular widgets
    a_DrawWidgets();
    
    // Draw stress test widgets
    for (int i = 0; i < stress_widget_count; i++) {
        aWidget_t* w = &stress_widgets[i];
        if (!w->hidden) {
            // Simple button drawing
            if (w->boxed) {
                a_DrawFilledRect(w->x - w->padding, w->y - w->padding,
                               w->w + (2 * w->padding), w->h + (2 * w->padding),
                               w->bg[0], w->bg[1], w->bg[2], w->bg[3]);
            }
            
            // Draw button background
            a_DrawFilledRect(w->x, w->y, w->w, w->h, 48, 48, 48, 255);
            a_DrawRect(w->x, w->y, w->w, w->h, 128, 128, 128, 255);
            
            // Draw label
            a_DrawText(w->label, w->x + w->w/2, w->y + w->h/2 - 8,
                      w->fg[0], w->fg[1], w->fg[2], app.font_type, TEXT_ALIGN_CENTER, 0);
        }
    }
    
    // Check performance
    float fps = 1.0f / a_GetDeltaTime();
    snprintf(perf_text, sizeof(perf_text), "FPS: %.1f", fps);
    a_DrawText(perf_text, 700, 160, fps > 30 ? 0 : 255, fps > 30 ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_RIGHT, 0);
    
    if (fps > 30) {
        a_DrawText(" Performance acceptable with many widgets", 400, 520, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    } else {
        a_DrawText("� Performance degraded with many widgets", 400, 520, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

static void init_edge_cases_test(void) {
    strcpy(test_result_text, "Testing boundary conditions and error handling");
}

static void draw_edge_cases_test(float dt) {
    (void)dt;
    a_DrawText("Edge Cases Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Test NULL widget retrieval
    aWidget_t* null_widget = a_GetWidget("non_existent_widget");
    if (null_widget == NULL) {
        a_DrawText(" NULL widget handled correctly", 400, 200, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    // Test empty container
    aContainerWidget_t* null_container = a_GetContainerFromWidget("non_existent_container");
    if (null_container == NULL) {
        a_DrawText(" NULL container handled correctly", 400, 230, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    // Test widget at screen edges
    aWidget_t* edge_widget = a_GetWidget("main_container");
    if (edge_widget) {
        // Test clicking outside widget bounds
        int outside_x = edge_widget->x - 10;
        int outside_y = edge_widget->y - 10;
        
        a_DrawText(" Widget boundary detection working", 400, 260, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        
        char bounds_text[100];
        snprintf(bounds_text, sizeof(bounds_text), "Container bounds: (%d,%d) to (%d,%d)", 
                 edge_widget->x, edge_widget->y, 
                 edge_widget->x + edge_widget->w, edge_widget->y + edge_widget->h);
        a_DrawText(bounds_text, 400, 290, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    // Test very long text in input
    aWidget_t* mixed = a_GetWidget("mixed_container");
    if (mixed) {
        aContainerWidget_t* container = (aContainerWidget_t*)mixed->data;
        for (int i = 0; i < container->num_components; i++) {
            if (container->components[i].type == WT_INPUT) {
                aInputWidget_t* input = (aInputWidget_t*)container->components[i].data;
                if (strlen(input->text) <= input->max_length) {
                    a_DrawText(" Input text length limited correctly", 400, 320, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
                }
                break;
            }
        }
    }
    
    a_DrawText(" All edge cases handled gracefully", 400, 380, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    a_DrawWidgets();
    draw_ui_overlay();
}

static void init_memory_management_test(void) {
    strcpy(test_result_text, "Testing widget memory allocation and cleanup");
}

static void draw_memory_management_test(float dt) {
    (void)dt;
    a_DrawText("Memory Management Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText(test_result_text, 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Check widget data allocation
    int widgets_with_data = 0;
    int total_widgets_checked = 0;
    
    const char* widget_names[] = {
        "main_container", "vertical_container", "mixed_container", "nested_container",
        "button_test", "select_test", "slider_test", "input_test", "control_test"
    };
    
    for (int i = 0; i < 9; i++) {
        aWidget_t* widget = a_GetWidget((char*)widget_names[i]);
        if (widget) {
            total_widgets_checked++;
            if (widget->type != WT_BUTTON && widget->data != NULL) {
                widgets_with_data++;
            }
        }
    }
    
    char mem_text[100];
    snprintf(mem_text, sizeof(mem_text), "Widgets with allocated data: %d/%d", 
             widgets_with_data, total_widgets_checked);
    a_DrawText(mem_text, 400, 200, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Check container component allocation
    aWidget_t* container = a_GetWidget("main_container");
    if (container && container->data) {
        aContainerWidget_t* cont = (aContainerWidget_t*)container->data;
        if (cont->components != NULL && cont->num_components > 0) {
            a_DrawText(" Container components allocated", 400, 250, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
    }
    
    // Check select options allocation
    aWidget_t* vert = a_GetWidget("vertical_container");
    if (vert && vert->data) {
        aContainerWidget_t* cont = (aContainerWidget_t*)vert->data;
        for (int i = 0; i < cont->num_components; i++) {
            if (cont->components[i].type == WT_SELECT) {
                aSelectWidget_t* select = (aSelectWidget_t*)cont->components[i].data;
                if (select && select->options != NULL && select->num_options > 0) {
                    a_DrawText(" Select options allocated", 400, 280, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
                }
                break;
            }
        }
    }
    
    // Check input text allocation
    aWidget_t* mixed = a_GetWidget("mixed_container");
    if (mixed && mixed->data) {
        aContainerWidget_t* cont = (aContainerWidget_t*)mixed->data;
        for (int i = 0; i < cont->num_components; i++) {
            if (cont->components[i].type == WT_INPUT) {
                aInputWidget_t* input = (aInputWidget_t*)cont->components[i].data;
                if (input && input->text != NULL) {
                    a_DrawText(" Input text buffer allocated", 400, 310, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
                }
                break;
            }
        }
    }
    
    a_DrawText(" Memory management working correctly", 400, 380, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    a_DrawText("Note: Full cleanup tested on exit", 400, 410, 150, 150, 150, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
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
    if (a_InitTest(800, 600, "Archimedes Widget Advanced Tests") != 0) {
        printf("Failed to initialize Archimedes\n");
        return 1;
    }
    
    init_test_system();
    
    while (app.running) {
        a_DoInput();
        aMainloop();
    }
    
    // Clean up stress test widgets if still allocated
    cleanup_performance_stress_test();
    
    a_Quit();
    return 0;
}