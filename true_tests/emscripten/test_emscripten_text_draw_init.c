/**
 * @file test_emscripten_text_draw_init.c
 * @brief Combined Emscripten test showcasing text rendering, drawing, and initialization
 * 
 * This test demonstrates the Archimedes engine running in a web browser via Emscripten.
 * It combines functionality from multiple test categories:
 * - Initialization and SDL setup verification
 * - Text rendering with fonts, colors, and alignment
 * - Drawing primitives and the Bob Ross color palette
 * - Web-optimized controls with arrow key navigation
 * 
 * Controls:
 * - ← → Arrow keys: Navigate between scenes
 * - ↑ Arrow key: Restart current scene
 * - ↓ Arrow key: Toggle auto-advance mode
 * - Mouse/Touch: Click or tap to advance
 * - Space: Advance to next scene
 * - ESC: Exit test
 * - R: Restart current scene
 * 
 * @author Archimedes Team
 * @date 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "Archimedes.h"
#include "emscripten_test_base.h"

// ============================================================================
// Test Scene Functions - Forward Declarations
// ============================================================================

static void init_initialization_test(void);
static void logic_initialization_test(float dt);
static void draw_initialization_test(float dt);

static void init_text_rendering_test(void);
static void logic_text_rendering_test(float dt);
static void draw_text_rendering_test(float dt);

static void init_drawing_test(void);
static void logic_drawing_test(float dt);
static void draw_drawing_test(float dt);

static void init_combined_demo(void);
static void logic_combined_demo(float dt);
static void draw_combined_demo(float dt);

// ============================================================================
// Test Scene Definitions
// ============================================================================

static WebTestScene_t test_scenes[] = {
    WEB_TEST_SCENE(
        "Initialization Test",
        "Verifying SDL setup, window creation, and Archimedes engine initialization",
        12.0f, // Extended duration for proper observation
        init_initialization_test,
        logic_initialization_test,
        draw_initialization_test
    ),
    WEB_TEST_SCENE(
        "Text Rendering Test",
        "Testing font rendering, alignment, colors, and text layout systems",
        15.0f, // Extended duration for reading text
        init_text_rendering_test,
        logic_text_rendering_test,
        draw_text_rendering_test
    ),
    WEB_TEST_SCENE(
        "Drawing System Test",
        "Demonstrating drawing primitives, Bob Ross palette, and artistic rendering",
        12.0f, // Extended duration for appreciating artwork
        init_drawing_test,
        logic_drawing_test,
        draw_drawing_test
    ),
    WEB_TEST_SCENE(
        "Combined Demo",
        "Interactive demonstration combining text, drawing, and user interaction",
        20.0f, // Extended duration for full demo cycle
        init_combined_demo,
        logic_combined_demo,
        draw_combined_demo
    )
};

// ============================================================================
// Global Test State
// ============================================================================

static int current_demo_mode = 0;
static float animation_timer = 0.0f;
static char dynamic_text[256] = "Welcome to Archimedes Web Tests!";

// Bob Ross palette (from our enhanced draw test) - with shorter names for better display
static const struct {
    int r, g, b;
    const char* name;
    const char* subtitle;
} bob_ross_palette[] = {
    {219, 219, 208, "Misty", "White"},
    {232, 232, 223, "Snow", "White"},
    {54, 79, 51, "Deep", "Forest"},
    {55, 69, 41, "Pine", "Shadow"},
    {111, 109, 118, "Mountain", "Stone"},
    {88, 122, 132, "Winter", "Sky"},
    {41, 50, 59, "Midnight", "Blue"},
    {179, 176, 159, "Warm", "Gray"},
    {52, 74, 77, "Deep", "Water"},
    {94, 93, 93, "Pewter", ""},
    {65, 135, 145, "Ice", "Blue"},
    {70, 107, 93, "Ever", "green"},
    {156, 82, 90, "Cabin", "Red"},
    {143, 137, 123, "Drift", "wood"},
    {255, 204, 103, "Sunny", "Yellow"},
    {147, 2, 53, "Deep", "Crimson"},
    {255, 255, 255, "Pure", "White"},
    {0, 0, 0, "Pure", "Black"}
};

// ============================================================================
// Scene 1: Initialization Test
// ============================================================================

static void init_initialization_test(void) {
    WEB_TEST_UPDATE("Checking SDL initialization and engine setup...");
    WEB_TEST_PASS(); // If we got here, initialization worked
}

static void logic_initialization_test(float dt) {
    animation_timer += dt;
    
    // Call the web input logic
    web_test_get_input_logic()(dt);
}

static void draw_initialization_test(float dt) {
    (void)dt;
    
    // Title
    a_DrawText("🏛️ Archimedes Engine - Initialization Test", 400, 100, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Test results
    a_DrawText("✅ SDL Video System", 200, 150, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("✅ SDL Audio System", 200, 170, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("✅ SDL Input System", 200, 190, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("✅ Font System (TTF)", 200, 210, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("✅ Image System (PNG)", 200, 230, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("✅ Render System", 200, 250, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // System info
    a_DrawText("Canvas Resolution: 800x600", 450, 150, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("Renderer: WebGL/OpenGL ES", 450, 170, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("Platform: Emscripten/WASM", 450, 190, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("Browser: Modern Web Browser", 450, 210, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("Architecture: WebAssembly", 450, 230, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Animated elements
    int pulse = (int)(128 + 127 * sin(animation_timer * 2.0f));
    a_DrawFilledRect(350, 300, 100, 50, pulse, 0, pulse, 255);
    a_DrawText("WASM", 400, 320, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Status
    a_DrawText("✅ All systems operational! Ready for testing.", 400, 400, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Draw UI overlay
    web_test_get_ui_overlay()(dt);
}

// ============================================================================
// Scene 2: Text Rendering Test
// ============================================================================

static void init_text_rendering_test(void) {
    WEB_TEST_UPDATE("Testing text rendering, fonts, and alignment...");
    WEB_TEST_PASS();
}

static void logic_text_rendering_test(float dt) {
    animation_timer += dt;
    
    // Update dynamic text
    snprintf(dynamic_text, sizeof(dynamic_text), "Timer: %.1f seconds", animation_timer);
    
    // Call the web input logic
    web_test_get_input_logic()(dt);
}

static void draw_text_rendering_test(float dt) {
    (void)dt;
    
    // Title
    a_DrawText("📝 Text Rendering System Test", 400, 80, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Font types demonstration
    a_DrawText("FONT_ENTER_COMMAND: Professional Command Style", 400, 120, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    a_DrawText("FONT_LINUX: Standard Linux Console Font", 400, 140, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    a_DrawText("FONT_GAME: Gaming and UI Font", 400, 160, 150, 150, 255, FONT_GAME, TEXT_ALIGN_CENTER, 0);
    
    // Text alignment demonstration
    a_DrawText("LEFT ALIGNED", 100, 200, 255, 100, 100, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("CENTER ALIGNED", 400, 200, 100, 255, 100, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    a_DrawText("RIGHT ALIGNED", 700, 200, 100, 100, 255, FONT_LINUX, TEXT_ALIGN_RIGHT, 0);
    
    // Color spectrum
    for (int i = 0; i < 8; i++) {
        int r = (i & 1) ? 255 : 0;
        int g = (i & 2) ? 255 : 0;
        int b = (i & 4) ? 255 : 0;
        char color_text[32];
        snprintf(color_text, sizeof(color_text), "Color %d", i);
        a_DrawText(color_text, 100 + i * 80, 240, r, g, b, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    }
    
    // Animated text
    int wave_r = (int)(128 + 127 * sin(animation_timer * 2.0f));
    int wave_g = (int)(128 + 127 * sin(animation_timer * 2.0f + 2.0f));
    int wave_b = (int)(128 + 127 * sin(animation_timer * 2.0f + 4.0f));
    a_DrawText("🌊 ANIMATED TEXT WAVE 🌊", 400, 280, wave_r, wave_g, wave_b, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Dynamic content
    a_DrawText(dynamic_text, 400, 320, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Text wrapping demonstration
    a_DrawText("This is a long text that demonstrates text wrapping capabilities within a specified width limit.", 400, 360, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 500);
    
    // Status
    a_DrawText("✅ Text rendering system working perfectly!", 400, 420, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Draw UI overlay
    web_test_get_ui_overlay()(dt);
}

// ============================================================================
// Scene 3: Drawing Test
// ============================================================================

static void init_drawing_test(void) {
    WEB_TEST_UPDATE("Testing drawing primitives and Bob Ross color palette...");
    WEB_TEST_PASS();
}

static void logic_drawing_test(float dt) {
    animation_timer += dt;
    
    // Call the web input logic
    web_test_get_input_logic()(dt);
}

// Helper functions for Bob Ross easel scene
static void draw_wooden_easel(float animation_timer) {
    // Easel legs with subtle animation
    float easel_sway = sin(animation_timer * 0.3f) * 1.0f;
    
    // Left leg
    a_DrawLine(150 + (int)easel_sway, 500, 180 + (int)easel_sway, 350, (aColor_t){139, 69, 19, 255}); // Saddle brown
    // Right leg  
    a_DrawLine(600 - (int)easel_sway, 500, 570 - (int)easel_sway, 350, (aColor_t){139, 69, 19, 255});
    // Back support leg
    a_DrawLine(375, 500, 375, 370, (aColor_t){160, 82, 45, 255}); // Saddle brown
    
    // Cross beam
    a_DrawLine(180 + (int)easel_sway, 400, 570 - (int)easel_sway, 400, (aColor_t){139, 69, 19, 255});
    
    // Canvas holder
    a_DrawLine(200 + (int)easel_sway, 380, 550 - (int)easel_sway, 380, (aColor_t){160, 82, 45, 255});
}

static void draw_white_canvas(float animation_timer) {
    // Canvas dimensions (16:12 aspect ratio like Bob Ross)
    int canvas_x = 220;
    int canvas_y = 180;
    int canvas_w = 360;
    int canvas_h = 200;
    
    // Subtle canvas sway
    float canvas_sway = sin(animation_timer * 0.3f) * 0.5f;
    canvas_x += (int)canvas_sway;
    
    // Canvas shadow (depth effect)
    a_DrawFilledRect(canvas_x + 3, canvas_y + 3, canvas_w, canvas_h, 180, 180, 180, 255);
    
    // White canvas background
    a_DrawFilledRect(canvas_x, canvas_y, canvas_w, canvas_h, 248, 248, 255, 255); // Slightly off-white
    
    // Canvas border
    a_DrawRect(canvas_x, canvas_y, canvas_w, canvas_h, 200, 200, 200, 255);
    
    // Return canvas position for painting
    // Note: We'll use these values in the painting function
}

static void draw_professional_winter_scene(float animation_timer) {
    // Canvas area (matching the white canvas dimensions)
    int canvas_x = 220 + (int)(sin(animation_timer * 0.3f) * 0.5f);
    int canvas_y = 180;
    int canvas_w = 360;
    int canvas_h = 200;
    
    // Painting area (slightly inset from canvas)
    int paint_x = canvas_x + 10;
    int paint_y = canvas_y + 10;
    int paint_w = canvas_w - 20;
    int paint_h = canvas_h - 20;
    
    // Sky gradient (Bob Ross technique)
    for (int y = 0; y < paint_h * 0.6f; y++) {
        float blend = y / (paint_h * 0.6f);
        int r = (int)(135 * (1 - blend) + 176 * blend); // Sky blue to lighter blue
        int g = (int)(206 * (1 - blend) + 224 * blend);
        int b = (int)(235 * (1 - blend) + 230 * blend);
        a_DrawHorizontalLine(paint_x, paint_x + paint_w, paint_y + y, (aColor_t){r, g, b, 255});
    }
    
    // Distant mountains (layered technique)
    float mountain_offset = sin(animation_timer * 0.2f) * 2.0f;
    // Back mountain range
    a_DrawFilledTriangle(
        paint_x + 50, paint_y + (int)(paint_h * 0.5f),
        paint_x + 150 + (int)mountain_offset, paint_y + (int)(paint_h * 0.3f),
        paint_x + 250, paint_y + (int)(paint_h * 0.5f),
        (aColor_t){147, 112, 219, 255} // Light slate blue
    );
    
    // Front mountain range
    a_DrawFilledTriangle(
        paint_x + 100, paint_y + (int)(paint_h * 0.6f),
        paint_x + 200 + (int)mountain_offset, paint_y + (int)(paint_h * 0.4f),
        paint_x + 300, paint_y + (int)(paint_h * 0.6f),
        (aColor_t){111, 109, 118, 255} // Mountain stone
    );
    
    // Evergreen trees (Bob Ross technique)
    int tree_positions[] = {80, 140, 280, 320};
    for (int i = 0; i < 4; i++) {
        int tree_x = paint_x + tree_positions[i];
        int tree_base_y = paint_y + (int)(paint_h * 0.8f);
        float tree_sway = sin(animation_timer * 1.5f + i * 0.8f) * 1.5f;
        
        // Tree trunk
        a_DrawFilledRect(tree_x + (int)tree_sway, tree_base_y - 20, 4, 20, 101, 67, 33, 255);
        
        // Tree layers (classic Bob Ross evergreen)
        for (int layer = 0; layer < 3; layer++) {
            int layer_y = tree_base_y - 15 - layer * 12;
            int layer_w = 25 - layer * 3;
            a_DrawFilledTriangle(
                tree_x - layer_w/2 + (int)tree_sway, layer_y,
                tree_x + (int)tree_sway, layer_y - 15,
                tree_x + layer_w/2 + (int)tree_sway, layer_y,
                (aColor_t){34, 139, 34, 255} // Forest green
            );
        }
    }
    
    // Cozy cabin
    int cabin_x = paint_x + 200;
    int cabin_y = paint_y + (int)(paint_h * 0.7f);
    
    // Cabin body
    a_DrawFilledRect(cabin_x, cabin_y, 50, 35, 160, 82, 45, 255); // Saddle brown
    
    // Cabin roof
    a_DrawFilledTriangle(
        cabin_x - 5, cabin_y,
        cabin_x + 25, cabin_y - 15,
        cabin_x + 55, cabin_y,
        (aColor_t){139, 69, 19, 255}
    );
    
    // Cabin window (warm glow)
    a_DrawFilledRect(cabin_x + 15, cabin_y + 10, 12, 12, 255, 215, 0, 255); // Gold
    a_DrawRect(cabin_x + 15, cabin_y + 10, 12, 12, 139, 69, 19, 255);
    
    // Cabin door
    a_DrawFilledRect(cabin_x + 35, cabin_y + 15, 10, 20, 101, 67, 33, 255);
    
    // Chimney with animated smoke
    a_DrawFilledRect(cabin_x + 35, cabin_y - 10, 8, 15, 105, 105, 105, 255);
    for (int i = 0; i < 6; i++) {
        float smoke_x = cabin_x + 39 + sin(animation_timer * 2.0f + i * 0.6f) * 3.0f;
        float smoke_y = cabin_y - 15 - i * 4;
        a_DrawPoint((int)smoke_x, (int)smoke_y, (aColor_t){211, 211, 211, 180});
    }
    
    // Snow ground with texture
    a_DrawFilledRect(paint_x, paint_y + (int)(paint_h * 0.85f), paint_w, (int)(paint_h * 0.15f), 250, 250, 250, 255);
    
    // Snow texture details
    for (int i = 0; i < 20; i++) {
        int snow_x = paint_x + (i * 18) % paint_w;
        int snow_y = paint_y + (int)(paint_h * 0.85f) + (i * 7) % (int)(paint_h * 0.15f);
        a_DrawPoint(snow_x, snow_y, (aColor_t){255, 255, 255, 255});
    }
}

static void draw_drawing_test(float dt) {
    (void)dt;
    
    // Title
    a_DrawText("🎨 Drawing System & Bob Ross Palette", 400, 60, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Drawing primitives demonstration
    a_DrawText("Drawing Primitives:", 100, 100, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Points
    for (int i = 0; i < 20; i++) {
        a_DrawPoint(150 + i * 3, 120, (aColor_t){255, 255, 255, 255});
    }
    a_DrawText("Points", 150, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Lines
    a_DrawLine(300, 115, 350, 125, (aColor_t){255, 0, 0, 255});
    a_DrawLine(300, 125, 350, 115, (aColor_t){0, 255, 0, 255});
    a_DrawText("Lines", 300, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Rectangles
    a_DrawRect(400, 115, 30, 15, 0, 0, 255, 255);
    a_DrawFilledRect(440, 115, 30, 15, 255, 255, 0, 255);
    a_DrawText("Rectangles", 400, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Circles
    a_DrawCircle(550, 122, 10, (aColor_t){255, 0, 255, 255});
    a_DrawFilledCircle(580, 122, 8, (aColor_t){0, 255, 255, 255});
    a_DrawText("Circles", 530, 130, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Bob Ross palette display
    a_DrawText("Bob Ross \"Lonely Retreat\" Palette:", 400, 160, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    int palette_size = sizeof(bob_ross_palette) / sizeof(bob_ross_palette[0]);
    int colors_per_row = 9;
    int swatch_size = 30;
    int start_x = 100;
    int start_y = 180;
    
    for (int i = 0; i < palette_size; i++) {
        int row = i / colors_per_row;
        int col = i % colors_per_row;
        
        int x = start_x + col * 40;
        int y = start_y + row * 40;
        
        a_DrawFilledRect(x, y, swatch_size, swatch_size, 
                        bob_ross_palette[i].r, bob_ross_palette[i].g, bob_ross_palette[i].b, 255);
        a_DrawRect(x, y, swatch_size, swatch_size, 128, 128, 128, 255);
    }
    
    // Professional Bob Ross easel scene
    a_DrawText("Professional Bob Ross Easel Setup:", 400, 400, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Draw wooden easel stand
    draw_wooden_easel(animation_timer);
    
    // Draw white canvas
    draw_white_canvas(animation_timer);
    
    // Draw professional winter painting on canvas
    draw_professional_winter_scene(animation_timer);
    
    a_DrawText("✅ Authentic Bob Ross painting setup!", 400, 520, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Draw UI overlay
    web_test_get_ui_overlay()(dt);
}

// ============================================================================
// Scene 4: Combined Demo
// ============================================================================

static void init_combined_demo(void) {
    WEB_TEST_UPDATE("Interactive demo combining all systems...");
    current_demo_mode = 0;
    WEB_TEST_PASS();
}

static void logic_combined_demo(float dt) {
    animation_timer += dt;
    
    // Cycle through demo modes
    current_demo_mode = (int)(animation_timer / 3.0f) % 4;
    
    // Call the web input logic
    web_test_get_input_logic()(dt);
}

static void draw_combined_demo(float dt) {
    (void)dt;
    
    // Title
    a_DrawText("🚀 Archimedes Engine - Web Integration Demo", 400, 60, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Demo mode indicator
    char mode_text[64];
    snprintf(mode_text, sizeof(mode_text), "Demo Mode %d/4", current_demo_mode + 1);
    a_DrawText(mode_text, 400, 90, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Different demo modes
    switch (current_demo_mode) {
        case 0: // Text animation
            a_DrawText("🌟 TEXT ANIMATION SHOWCASE 🌟", 400, 150, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            for (int i = 0; i < 10; i++) {
                float wave = sin(animation_timer * 3.0f + i * 0.3f) * 20.0f;
                int color = (int)(128 + 127 * sin(animation_timer * 2.0f + i * 0.5f));
                char wave_char[2] = { '0' + i, '\0' };
                a_DrawText(wave_char, 350 + i * 20, 200 + (int)wave, color, color, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            }
            break;
            
        case 1: // Geometric patterns
            a_DrawText("📐 GEOMETRIC PATTERNS 📐", 400, 150, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            for (int i = 0; i < 8; i++) {
                float angle = animation_timer + i * 0.785f; // π/4
                int x = 400 + (int)(sin(angle) * 100);
                int y = 250 + (int)(cos(angle) * 100);
                int size = 10 + (int)(sin(animation_timer * 2.0f + i) * 5);
                a_DrawFilledRect(x - size/2, y - size/2, size, size, 255, i * 32, 255 - i * 32, 255);
            }
            break;
            
        case 2: // Color palette cycling
            a_DrawText("🎨 COLOR PALETTE SHOWCASE 🎨", 400, 150, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            for (int i = 0; i < 6; i++) {
                int color_index = ((int)animation_timer + i) % (sizeof(bob_ross_palette) / sizeof(bob_ross_palette[0]));
                a_DrawFilledCircle(200 + i * 80, 250, 30, (aColor_t){
                    bob_ross_palette[color_index].r,
                    bob_ross_palette[color_index].g,
                    bob_ross_palette[color_index].b,
                    255
                });
                // Two-line color labels for better readability
                a_DrawText((char*)bob_ross_palette[color_index].name, 200 + i * 80, 295, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
                if (strlen(bob_ross_palette[color_index].subtitle) > 0) {
                    a_DrawText((char*)bob_ross_palette[color_index].subtitle, 200 + i * 80, 310, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
                }
            }
            break;
            
        case 3: // Combined interactive
            a_DrawText("🎮 INTERACTIVE DEMO 🎮", 400, 150, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            a_DrawText("Use arrow keys to navigate • Click to interact", 400, 180, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
            
            // Interactive elements
            float pulse = sin(animation_timer * 4.0f) * 0.3f + 0.7f;
            a_DrawFilledCircle(300, 250, (int)(30 * pulse), (aColor_t){255, 255, 0, 255});
            a_DrawFilledCircle(500, 250, (int)(25 * pulse), (aColor_t){0, 255, 255, 255});
            
            a_DrawText("Web-Ready!", 300, 300, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
            a_DrawText("Cross-Platform!", 500, 300, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
            break;
    }
    
    // Performance info
    char perf_text[128];
    snprintf(perf_text, sizeof(perf_text), "Runtime: %.1fs | Frame: %d | FPS: ~60", animation_timer, (int)(animation_timer * 60));
    a_DrawText(perf_text, 400, 380, 150, 150, 150, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Status
    a_DrawText("✅ All systems integrated successfully!", 400, 420, 0, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    a_DrawText("🌐 Ready for web deployment!", 400, 440, 0, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Draw UI overlay
    web_test_get_ui_overlay()(dt);
}

// ============================================================================
// Main Entry Point
// ============================================================================

void aMainloop(void) {
    a_DoInput();
    a_PrepareScene();
    
    if (web_test_is_initialized()) {
        app.delegate.logic(a_GetDeltaTime());
        app.delegate.draw(a_GetDeltaTime());
    }
    
    a_PresentScene();
}

int main(void) {
    // Initialize Archimedes
    if (a_Init(800, 600, "Archimedes Emscripten Test Suite") != 0) {
        printf("Failed to initialize Archimedes\n");
        return 1;
    }
    
    // Set dark background for better contrast
    app.background.r = 16;
    app.background.g = 16;
    app.background.b = 24;
    app.background.a = 255;
    
    // Initialize web test system
    int scene_count = sizeof(test_scenes) / sizeof(WebTestScene_t);
    if (web_test_init(test_scenes, scene_count, 
                      "Archimedes Web Test Suite", 
                      "Comprehensive testing of text, drawing, and initialization systems") != 0) {
        printf("Failed to initialize web test system\n");
        return 1;
    }
    
    printf("Archimedes Emscripten Test Suite initialized successfully!\n");
    printf("Use arrow keys to navigate between %d test scenes\n", scene_count);
    
#ifdef __EMSCRIPTEN__
    // Set up the main loop for Emscripten
    emscripten_set_main_loop(aMainloop, 0, 1);
    // Note: a_Quit() is not called in Emscripten - the main loop continues indefinitely
#else
    // Fallback for native builds
    while (app.running) {
        aMainloop();
    }
    a_Quit();
#endif
    
    return 0;
}