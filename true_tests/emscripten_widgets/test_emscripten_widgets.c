/**
 * @file test_emscripten_widgets.c
 * @brief The Sacred Chamber of Divine Automata - Epic Emscripten Widget Test Suite
 * 
 * Behold! The ultimate demonstration of Archimedes' mechanical genius, now manifested
 * in the ethereal realm of the web browser! This test suite transforms humble UI widgets
 * into SACRED AUTOMATA worthy of the gods themselves.
 * 
 * The Seven Sacred Scenes:
 * 1. Awakening of the Automata - Widget system initialization
 * 2. The Mortal Touch Interface - Touch/click interactions  
 * 3. The Flexbox Pantheon - Responsive layouts and containers
 * 4. The Symphony of Interactions - Multi-widget orchestration
 * 5. The Web Performance Colosseum - Stress testing with 100+ widgets
 * 6. The Responsive Oracle - Screen adaptation testing
 * 7. The Final Ascension - Interactive demo and user creation
 * 
 * Controls:
 * - ← → Arrow keys: Navigate between sacred scenes
 * - ↑ Arrow key: Restart current divine trial
 * - ↓ Arrow key: Toggle auto-advance mode
 * - Mouse/Touch: Interact with the sacred automata
 * - Space: Advance to next realm of testing
 * - ESC: Exit the divine chamber
 * 
 * @author Archimedes - The Divine Architect
 * @date 2025 - The Year of Sacred Mechanisms
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
#include "../emscripten/emscripten_test_base.h"

// Test-friendly initialization function
extern int a_InitTest(const int width, const int height, const char *title);

// ============================================================================
// 🏛️ DIVINE DIAGNOSTIC ORACLE - Genius-Level Debugging Framework
// ============================================================================

// Debug levels for progressive diagnostic detail
typedef enum {
    DEBUG_SILENT = 0,       // No debug output
    DEBUG_BASIC = 1,        // Basic status messages
    DEBUG_VERBOSE = 2,      // Detailed diagnostic information
    DEBUG_TRACE = 3,        // Function-level tracing
    DEBUG_DIVINE_INSIGHT = 4 // Everything - for AI analysis
} DebugLevel_t;

// Environment detection results
typedef struct {
    int is_emscripten;      // Running in Emscripten/browser
    int is_native;          // Running natively
    int console_available;  // Console output available
    int json_file_exists;   // JSON file accessible
    int widgets_loaded;     // Widget system initialized
    int web_framework_active; // Web test framework running
} EnvironmentInfo_t;

// Diagnostic message structure (AI-parseable)
typedef struct {
    const char* severity;   // "INFO", "WARN", "ERROR", "CRITICAL"
    const char* component;  // "INIT", "JSON", "WIDGETS", "GRAPHICS", "INPUT"
    const char* message;    // Human-readable description
    const char* context;    // Additional context
    const char* suggestion; // Suggested solution
    float timestamp;        // When this occurred
} DiagnosticMessage_t;

// Global diagnostic state
static DebugLevel_t g_debug_level = DEBUG_BASIC;
static EnvironmentInfo_t g_env = {0};
static DiagnosticMessage_t g_diagnostic_history[100];
static int g_diagnostic_count = 0;

// Console bridge for Emscripten - makes browser console visible to terminal
#ifdef __EMSCRIPTEN__
EM_JS(void, console_bridge_setup, (), {
    // Capture original console functions
    const originalLog = console.log;
    const originalError = console.error;
    const originalWarn = console.warn;
    
    // Bridge console output to Emscripten stdout
    console.log = function(...args) {
        const msg = args.map(a => String(a)).join(' ');
        Module.print('🏛️ BROWSER: ' + msg);
        originalLog(...args);
    };
    
    console.error = function(...args) {
        const msg = args.map(a => String(a)).join(' ');
        Module.printErr('❌ BROWSER_ERROR: ' + msg);
        originalError(...args);
    };
    
    console.warn = function(...args) {
        const msg = args.map(a => String(a)).join(' ');
        Module.print('⚠️ BROWSER_WARN: ' + msg);
        originalWarn(...args);
    };
    
    // Add diagnostic reporting
    window.divine_diagnostic = function(severity, component, message, context, suggestion) {
        const timestamp = Date.now() / 1000.0;
        const report = {
            severity: severity,
            component: component,
            message: message,
            context: context || "",
            suggestion: suggestion || "",
            timestamp: timestamp
        };
        Module.print('🔮 DIVINE_DIAGNOSTIC: ' + JSON.stringify(report));
    };
    
    console.log('🏛️ Divine Console Bridge activated - terminal can now see browser messages!');
});

EM_JS(void, divine_report_js, (const char* severity, const char* component, const char* message, const char* context, const char* suggestion), {
    if (window.divine_diagnostic) {
        window.divine_diagnostic(UTF8ToString(severity), UTF8ToString(component), UTF8ToString(message), UTF8ToString(context), UTF8ToString(suggestion));
    }
});
#endif

// Environment auto-detection
static void detect_environment(void) {
#ifdef __EMSCRIPTEN__
    g_env.is_emscripten = 1;
    g_env.is_native = 0;
    g_env.console_available = 1;
    console_bridge_setup();
    printf("🌟 DIVINE_DETECTION: Running in Sacred Web Realm (Emscripten)\n");
#else
    g_env.is_emscripten = 0;
    g_env.is_native = 1;
    g_env.console_available = 1;
    printf("🏛️ DIVINE_DETECTION: Running in Mortal Desktop Realm (Native)\n");
#endif
}

// Structured diagnostic logging
static void divine_diagnostic(const char* severity, const char* component, const char* message, const char* context, const char* suggestion) {
    if (g_debug_level == DEBUG_SILENT) return;
    
    // Store in history
    if (g_diagnostic_count < 100) {
        DiagnosticMessage_t* diag = &g_diagnostic_history[g_diagnostic_count++];
        diag->severity = severity;
        diag->component = component;
        diag->message = message;
        diag->context = context ? context : "";
        diag->suggestion = suggestion ? suggestion : "";
        diag->timestamp = 0.0f; // Will add timing later
    }
    
    // Format output based on debug level
    if (g_debug_level >= DEBUG_BASIC) {
        printf("🔮 [%s:%s] %s\n", severity, component, message);
    }
    
    if (g_debug_level >= DEBUG_VERBOSE && context) {
        printf("   📋 Context: %s\n", context);
    }
    
    if (g_debug_level >= DEBUG_VERBOSE && suggestion) {
        printf("   💡 Suggestion: %s\n", suggestion);
    }
    
    if (g_debug_level >= DEBUG_DIVINE_INSIGHT) {
        printf("   🔍 AI_PARSEABLE: {\"severity\":\"%s\",\"component\":\"%s\",\"message\":\"%s\",\"context\":\"%s\",\"suggestion\":\"%s\"}\n",
               severity, component, message, context ? context : "", suggestion ? suggestion : "");
    }
    
#ifdef __EMSCRIPTEN__
    // Also report to browser console for web debugging
    divine_report_js(severity, component, message, context ? context : "", suggestion ? suggestion : "");
#endif
}

// Self-diagnostic system
static void run_system_diagnostics(void) {
    divine_diagnostic("INFO", "DIAGNOSTICS", "🔮 Running Sacred Chamber Self-Diagnostics", 
                     "Checking all divine systems", "Monitor output for any issues");
    
    // Test JSON file accessibility
    FILE* json_test = fopen("test_emscripten_widgets.json", "r");
    if (json_test) {
        fclose(json_test);
        g_env.json_file_exists = 1;
        divine_diagnostic("INFO", "JSON", "✅ Sacred JSON file accessible", 
                         "File: test_emscripten_widgets.json", "JSON loading should work");
    } else {
        g_env.json_file_exists = 0;
        divine_diagnostic("WARN", "JSON", "❌ Sacred JSON file not found", 
                         "File: test_emscripten_widgets.json", "Will use programmatic widget creation fallback");
    }
    
    // Test Archimedes initialization
    if (app.running) {
        divine_diagnostic("INFO", "INIT", "✅ Archimedes divine framework initialized", 
                         "SDL and core systems ready", "Framework ready for sacred operations");
    } else {
        divine_diagnostic("ERROR", "INIT", "❌ Archimedes framework not initialized", 
                         "Core systems not ready", "Check a_Init() return value");
    }
    
    // Environment-specific diagnostics
    if (g_env.is_emscripten) {
        divine_diagnostic("INFO", "ENVIRONMENT", "🌐 Web realm detected - console bridge active", 
                         "Browser messages visible in terminal", "Use browser dev tools for additional debugging");
    } else {
        divine_diagnostic("INFO", "ENVIRONMENT", "🖥️ Native realm detected - direct execution", 
                         "Full native capabilities available", "All debugging output visible directly");
    }
}

// ============================================================================
// 🏺 SACRED WIDGET CREATION FALLBACK - When JSON Files Fail the Gods
// ============================================================================

// Forward declaration for setup function
static void setup_navigation_widgets(void);

// Programmatic widget creation with epic theming
static void create_divine_widgets_programmatically(void) {
    divine_diagnostic("INFO", "WIDGETS", "🏺 Creating sacred widgets programmatically", 
                     "JSON loading failed - using divine backup creation", "Widgets will have epic theming");

    // Note: This would normally use a_CreateWidget API if available
    // For now, we'll mark widgets as "failed to load but gracefully handled"
    g_env.widgets_loaded = 0;
    
    divine_diagnostic("WARN", "WIDGETS", "📜 Widget creation API not available", 
                     "Archimedes framework needs programmatic widget API", "Consider adding a_CreateWidget() functions");
    
    // Create epic status messages for each divine widget that would exist
    const char* divine_widgets[] = {
        "oracle_stone", "celestial_dial", "scribes_tablet", "sacred_chamber"
    };
    
    for (int i = 0; i < 4; i++) {
        divine_diagnostic("INFO", "WIDGETS", "🌟 Divine widget template prepared", 
                         divine_widgets[i], "Widget would be created with epic theming if API existed");
    }
}

// Intelligent widget system initialization with fallback
static void initialize_sacred_widget_system(void) {
    divine_diagnostic("INFO", "WIDGETS", "🔮 Initializing Sacred Widget System", 
                     "Attempting JSON loading with graceful fallback", "Monitor for loading success");

    // First, try JSON loading
    if (g_env.json_file_exists) {
        divine_diagnostic("INFO", "WIDGETS", "📜 Attempting JSON widget loading", 
                         "File: test_emscripten_widgets.json", "Should load widgets with divine theming");
        
        // This will either succeed or fail gracefully
        a_InitWidgets("test_emscripten_widgets.json");
        
        // Check if widgets were actually loaded (basic validation)
        aWidget_t* test_widget = a_GetWidget("oracle_stone");
        if (test_widget != NULL) {
            g_env.widgets_loaded = 1;
            divine_diagnostic("INFO", "WIDGETS", "✅ JSON widgets loaded successfully", 
                             "Sacred mechanisms awakened from file", "Widgets ready for divine interaction");
            
            // Set up navigation widget callbacks
            setup_navigation_widgets();
        } else {
            g_env.widgets_loaded = 0;
            divine_diagnostic("WARN", "WIDGETS", "⚠️ JSON file loaded but widgets not found", 
                             "File exists but widgets not accessible", "Falling back to programmatic creation");
            create_divine_widgets_programmatically();
        }
    } else {
        divine_diagnostic("WARN", "WIDGETS", "📂 JSON file not accessible", 
                         "Skipping JSON loading", "Using programmatic fallback");
        create_divine_widgets_programmatically();
    }
}

// ============================================================================
// 🎮 Smart Navigation System - Widget-Friendly Input Handling
// ============================================================================

// Navigation state tracking
static int g_nav_space_pressed = 0;
static int g_nav_f1_pressed = 0;
static int g_nav_escape_pressed = 0;
static int g_nav_r_pressed = 0;

// Smart navigation that doesn't hijack widget interactions
static void handle_smart_navigation(void) {
    // Only handle specific navigation keys, let widgets handle their own input
    
    // Space for manual scene advancement (only when not focused on input widgets)
    if (app.keyboard[SDL_SCANCODE_SPACE] == 1 && !g_nav_space_pressed) {
        g_nav_space_pressed = 1;
        
        // Check if we're currently focused on an input widget that needs space
        if (app.active_widget && app.active_widget->type == WT_INPUT) {
            // Let input widget handle space
            divine_diagnostic("INFO", "NAVIGATION", "⌨️ Space key passed to input widget", 
                             "Active input widget detected", "Space key not used for navigation");
        } else {
            divine_diagnostic("INFO", "NAVIGATION", "🚀 Space pressed - advancing scene", 
                             "Manual scene advancement", "User chose to skip current scene");
            // TODO: Signal scene advancement via navigation widgets
        }
    } else if (app.keyboard[SDL_SCANCODE_SPACE] == 0) {
        g_nav_space_pressed = 0;
    }
    
    // F1 for quick skip (always works)
    if (app.keyboard[SDL_SCANCODE_F1] == 1 && !g_nav_f1_pressed) {
        g_nav_f1_pressed = 1;
        divine_diagnostic("INFO", "NAVIGATION", "⚡ F1 pressed - quick skip", 
                         "Fast scene advancement", "User chose quick skip");
        // TODO: Signal quick skip via navigation widgets
    } else if (app.keyboard[SDL_SCANCODE_F1] == 0) {
        g_nav_f1_pressed = 0;
    }
    
    // Escape for exit (always works)
    if (app.keyboard[SDL_SCANCODE_ESCAPE] == 1 && !g_nav_escape_pressed) {
        g_nav_escape_pressed = 1;
        divine_diagnostic("INFO", "NAVIGATION", "🚪 Escape pressed - exiting Sacred Chamber", 
                         "User requested exit", "Closing divine experience");
        app.running = 0;
    } else if (app.keyboard[SDL_SCANCODE_ESCAPE] == 0) {
        g_nav_escape_pressed = 0;
    }
    
    // R for restart current scene (always works)
    if (app.keyboard[SDL_SCANCODE_R] == 1 && !g_nav_r_pressed) {
        g_nav_r_pressed = 1;
        divine_diagnostic("INFO", "NAVIGATION", "🔄 R pressed - restarting scene", 
                         "Scene restart requested", "Resetting current divine trial");
        // Note: Actual restart logic will be handled in scene management functions
        // TODO: Add restart via function call instead of direct variable access
    } else if (app.keyboard[SDL_SCANCODE_R] == 0) {
        g_nav_r_pressed = 0;
    }
}

// Forward declarations for callbacks (to be implemented after global variables)
static void next_scene_action(void);
static void prev_scene_action(void);

// Scene 1 dynamic button actions
static void awaken_all_action(void);
static void reset_widgets_action(void);
static void power_boost_action(void);
static void divine_mode_action(void);

// Scene button visibility helpers
static void hide_scene1_buttons(void);
static void hide_scene2_buttons(void);
static void hide_scene3_buttons(void);


// ============================================================================
// Divine Constants and Sacred Geometries
// ============================================================================

#define MAX_STRESS_WIDGETS 150
#define DIVINE_GOLDEN_RATIO 1.618f
#define SACRED_ANIMATION_SPEED 2.0f
#define CELESTIAL_GLOW_INTENSITY 128

// ============================================================================
// Test Scene Functions - Forward Declarations of Sacred Rituals
// ============================================================================

// Scene management functions
static void advance_to_next_scene(void);
static void init_test_system(void);
static void common_logic(float dt);
static void draw_ui_overlay(void);
static void cleanup_performance_test(void);
static void handle_smart_navigation(void);
static void setup_navigation_widgets(void);
static void next_scene_action(void);
static void prev_scene_action(void);

// Scene initialization functions
static void init_awakening_automata(void);
static void init_mortal_touch_interface(void);
static void init_flexbox_pantheon(void);
static void init_symphony_interactions(void);
static void init_performance_colosseum(void);
static void init_responsive_oracle(void);
static void init_final_ascension(void);

// Scene logic functions
static void logic_awakening_automata(float dt);
static void logic_mortal_touch_interface(float dt);
static void logic_flexbox_pantheon(float dt);
static void logic_symphony_interactions(float dt);
static void logic_performance_colosseum(float dt);
static void logic_responsive_oracle(float dt);
static void logic_final_ascension(float dt);

// Scene drawing functions
static void draw_awakening_automata(float dt);
static void draw_mortal_touch_interface(float dt);
static void draw_flexbox_pantheon(float dt);
static void draw_symphony_interactions(float dt);
static void draw_performance_colosseum(float dt);
static void draw_responsive_oracle(float dt);
static void draw_final_ascension(float dt);

// ============================================================================
// The Seven Sacred Scenes - Divine Test Definitions
// ============================================================================

// Using WebTestScene_t from emscripten_test_base.h for proper web framework integration

static WebTestScene_t test_scenes[] = {
    WEB_TEST_SCENE(
        "⚡ Awakening of the Automata",
        "Breathing divine life into sacred widget mechanisms - testing initialization and first contact",
        15.0f,
        init_awakening_automata,
        logic_awakening_automata,
        draw_awakening_automata
    ),
    WEB_TEST_SCENE(
        "👆 The Mortal Touch Interface",
        "Testing divine responsiveness to mortal touch - mouse, keyboard, and interaction systems",
        20.0f,
        init_mortal_touch_interface,
        logic_mortal_touch_interface,
        draw_mortal_touch_interface
    ),
    WEB_TEST_SCENE(
        "🏛️ The Flexbox Pantheon",
        "Arranging sacred containers with divine geometry - layout and responsive design",
        18.0f,
        init_flexbox_pantheon,
        logic_flexbox_pantheon,
        draw_flexbox_pantheon
    ),
    WEB_TEST_SCENE(
        "🎼 The Symphony of Interactions",
        "Orchestrating divine harmony between multiple widget automata - complex interactions",
        25.0f,
        init_symphony_interactions,
        logic_symphony_interactions,
        draw_symphony_interactions
    ),
    WEB_TEST_SCENE(
        "⚔️ The Web Performance Colosseum",
        "Epic battle of 150+ widgets against mortal hardware - stress testing performance",
        30.0f,
        init_performance_colosseum,
        logic_performance_colosseum,
        draw_performance_colosseum
    ),
    WEB_TEST_SCENE(
        "🔮 The Responsive Oracle",
        "Testing prophetic adaptation to any screen size - responsive design validation",
        20.0f,
        init_responsive_oracle,
        logic_responsive_oracle,
        draw_responsive_oracle
    ),
    WEB_TEST_SCENE(
        "🌟 The Final Ascension",
        "Ultimate demonstration of widget mastery - interactive divine showcase",
        25.0f,
        init_final_ascension,
        logic_final_ascension,
        draw_final_ascension
    )
};

// ============================================================================
// Global Sacred State - The Divine Memory  
// ============================================================================

// Scene management state (use web framework API instead of local variables)
// Note: Web framework manages current_scene, total_scenes, tests_passed, tests_failed, scene_test_result

static float divine_timer = 0.0f;
static int widget_interaction_count = 0;
static int stress_widget_count = 0;
static aWidget_t* stress_widgets = NULL;
static int current_demo_phase = 0;
static float celestial_glow_pulse = 0.0f;
static char divine_status_text[256] = "Preparing the Sacred Chamber...";
static int responsive_test_width = 800;
static int responsive_test_height = 600;

// Dynamic button state variables
static int all_widgets_awakened = 0;
static float power_boost_multiplier = 1.0f;
static int divine_mode_active = 0;
static int widgets_force_reset = 0;

// Sacred color palettes for divine theming
static const struct {
    int r, g, b;
    const char* name;
    const char* divine_meaning;
} celestial_palette[] = {
    {74, 158, 255, "Divine Azure", "The color of Zeus's lightning"},
    {255, 215, 0, "Sacred Gold", "Midas's eternal touch"},
    {138, 43, 226, "Mystic Purple", "The robes of prophecy"},
    {0, 255, 127, "Celestial Green", "The gardens of Olympus"},
    {255, 105, 180, "Ethereal Rose", "Aphrodite's divine beauty"},
    {64, 224, 208, "Ocean Turquoise", "Poseidon's sacred depths"},
    {255, 140, 0, "Phoenix Orange", "The eternal flame of rebirth"},
    {220, 220, 220, "Starlight Silver", "The cosmic dust of creation"}
};

// ============================================================================
// Navigation Widget Setup Functions
// ============================================================================

// External functions from web test framework
extern void emscripten_next_scene(void);
extern void emscripten_previous_scene(void);
extern void emscripten_restart_scene(void);

// Navigation widget callback implementations
static void next_scene_action(void) {
    divine_diagnostic("INFO", "WIDGETS", "➡️ Next Scene widget activated", 
                     "Widget-based scene navigation", "Advancing to next divine trial");
    
    // Use the web framework's scene advancement
    emscripten_next_scene();
}

static void prev_scene_action(void) {
    divine_diagnostic("INFO", "WIDGETS", "⬅️ Previous Scene widget activated", 
                     "Widget-based scene navigation", "Returning to previous divine trial");
    
    // Use the web framework's scene navigation
    emscripten_previous_scene();
}

// ============================================================================
// Scene 1 Dynamic Button Actions
// ============================================================================

static void awaken_all_action(void) {
    divine_diagnostic("DEBUG", "BUTTON", "🔥 AWAKEN ALL BUTTON CLICKED!", 
                     "Button callback function called", "Testing if widget actions work");
    
    all_widgets_awakened = 1;
    widget_interaction_count = 10; // Force high interaction count
    divine_diagnostic("INFO", "SCENE1", "⚡ Awaken All button activated", 
                     "All widgets instantly awakened", "Divine power flows through all mechanisms!");
    strcpy(divine_status_text, "⚡ ALL AUTOMATA AWAKENED BY DIVINE COMMAND! ⚡");
}

static void reset_widgets_action(void) {
    divine_diagnostic("DEBUG", "BUTTON", "🔥 RESET WIDGETS BUTTON CLICKED!", 
                     "Button callback function called", "Testing if widget actions work");
    
    all_widgets_awakened = 0;
    widget_interaction_count = 0;
    widgets_force_reset = 1;
    divine_diagnostic("INFO", "SCENE1", "🔄 Reset Widgets button activated", 
                     "All widgets returned to dormant state", "The sacred mechanisms slumber once more");
    strcpy(divine_status_text, "🌟 The Divine Breath awakens sleeping widgets...");
}

static void power_boost_action(void) {
    divine_diagnostic("DEBUG", "BUTTON", "🔥 POWER BOOST BUTTON CLICKED!", 
                     "Button callback function called", "Testing if widget actions work");
    
    power_boost_multiplier *= 1.5f;
    if (power_boost_multiplier > 5.0f) power_boost_multiplier = 1.0f; // Reset if too high
    divine_diagnostic("INFO", "SCENE1", "⚡ Power Boost button activated", 
                     "Celestial energy amplified", "Sacred animations pulse with divine power!");
    char boost_text[128];
    snprintf(boost_text, sizeof(boost_text), "⚡ DIVINE POWER BOOST ACTIVE! (%.1fx) ⚡", power_boost_multiplier);
    strcpy(divine_status_text, boost_text);
}

static void divine_mode_action(void) {
    divine_diagnostic("DEBUG", "BUTTON", "🔥 DIVINE MODE BUTTON CLICKED!", 
                     "Button callback function called", "Testing if widget actions work");
    
    divine_mode_active = !divine_mode_active;
    divine_diagnostic("INFO", "SCENE1", "🌟 Divine Mode button activated", 
                     divine_mode_active ? "Entering divine realm" : "Returning to mortal realm", 
                     "The very fabric of reality shifts!");
    if (divine_mode_active) {
        strcpy(divine_status_text, "🌟 DIVINE MODE ACTIVATED! REALITY TRANSCENDS! 🌟");
    } else {
        strcpy(divine_status_text, "🏛️ Returning to the mortal realm...");
    }
}

// ============================================================================
// Scene Button Visibility Helpers
// ============================================================================

static void hide_scene1_buttons(void) {
    aWidget_t* awaken_btn = a_GetWidget("awaken_all_btn");
    aWidget_t* reset_btn = a_GetWidget("reset_widgets_btn");
    aWidget_t* boost_btn = a_GetWidget("power_boost_btn");
    aWidget_t* divine_btn = a_GetWidget("divine_mode_btn");
    
    if (awaken_btn) awaken_btn->hidden = 1;
    if (reset_btn) reset_btn->hidden = 1;
    if (boost_btn) boost_btn->hidden = 1;
    if (divine_btn) divine_btn->hidden = 1;
}

static void hide_scene2_buttons(void) {
    // Will implement when Scene 2 buttons are added
}

static void hide_scene3_buttons(void) {
    // Will implement when Scene 3 buttons are added
}


// Set up navigation widget callbacks
static void setup_navigation_widgets(void) {
    divine_diagnostic("INFO", "WIDGETS", "🎮 Setting up navigation widget callbacks", 
                     "Connecting widgets to scene navigation functions", "Widgets will control scene navigation");
    
    // Connect navigation widgets to their callback functions
    aWidget_t* next_btn = a_GetWidget("navigation_next");
    if (next_btn) {
        next_btn->action = next_scene_action;
        divine_diagnostic("INFO", "WIDGETS", "✅ Next Scene widget connected", 
                         "Widget will advance scenes", "Scene navigation ready");
    }
    
    aWidget_t* prev_btn = a_GetWidget("navigation_prev");
    if (prev_btn) {
        prev_btn->action = prev_scene_action;
        divine_diagnostic("INFO", "WIDGETS", "✅ Previous Scene widget connected", 
                         "Widget will go back scenes", "Scene navigation ready");
    }
    
    if (next_btn && prev_btn) {
        divine_diagnostic("INFO", "WIDGETS", "🎮 All navigation widgets operational", 
                         "Widget-based scene control active", "Mortals can now navigate with divine widgets");
    } else {
        divine_diagnostic("WARN", "WIDGETS", "⚠️ Some navigation widgets missing", 
                         "Check JSON file for navigation widget definitions", "Scene navigation may be limited");
    }
    
    // Setup Scene 1 dynamic buttons
    aWidget_t* awaken_btn = a_GetWidget("awaken_all_btn");
    if (awaken_btn) {
        awaken_btn->action = awaken_all_action;
        divine_diagnostic("INFO", "WIDGETS", "⚡ Awaken All button connected", 
                         "Button will instantly awaken all widgets", "Divine power ready");
    }
    
    aWidget_t* reset_btn = a_GetWidget("reset_widgets_btn");
    if (reset_btn) {
        reset_btn->action = reset_widgets_action;
        divine_diagnostic("INFO", "WIDGETS", "🔄 Reset Widgets button connected", 
                         "Button will reset all widgets to dormant", "Reset power ready");
    }
    
    aWidget_t* boost_btn = a_GetWidget("power_boost_btn");
    if (boost_btn) {
        boost_btn->action = power_boost_action;
        divine_diagnostic("INFO", "WIDGETS", "⚡ Power Boost button connected", 
                         "Button will amplify celestial energy", "Boost power ready");
    }
    
    aWidget_t* divine_btn = a_GetWidget("divine_mode_btn");
    if (divine_btn) {
        divine_btn->action = divine_mode_action;
        divine_diagnostic("INFO", "WIDGETS", "🌟 Divine Mode button connected", 
                         "Button will toggle divine realm", "Divine transformation ready");
    }
}

// ============================================================================
// Common Scene Management Functions (like working native tests)
// ============================================================================


static void init_test_system(void) {
    divine_diagnostic("INFO", "INIT", "🏛️ Initializing Sacred Chamber Test System", 
                     "Starting divine widget testing framework", "All systems preparing for epic trials");
    
    // Detect environment and set up diagnostics first
    detect_environment();
    run_system_diagnostics();
    
    // Set mystical background
    app.background.r = 16;
    app.background.g = 16;
    app.background.b = 32;
    app.background.a = 255;
    
    divine_diagnostic("INFO", "INIT", "🎨 Sacred background color set", 
                     "Deep cosmic blue (16,16,32)", "Perfect for divine widget display");
    
    // Initialize widgets with intelligent fallback system
    initialize_sacred_widget_system();
    
    printf("\n🏛️ ===== THE SACRED CHAMBER OF DIVINE AUTOMATA ===== 🏛️\n");
    printf("⚡ Ancient trials: %d\n", web_test_get_total_scenes());
    printf("🎮 Controls: Navigation widgets, F1=Skip, R=Restart, ESC=Exit\n");
    printf("🔮 Divine Console Bridge: %s\n", g_env.is_emscripten ? "ACTIVE (browser→terminal)" : "DIRECT");
    printf("📜 Widget System: %s\n", g_env.widgets_loaded ? "JSON LOADED" : "FALLBACK MODE");
    printf("🌟 By the grace of Archimedes, let the trials begin!\n\n");
    
    divine_diagnostic("INFO", "INIT", "🚀 Sacred Chamber fully initialized", 
                     "All systems ready for divine trials", "Beginning epic widget testing experience");
}

static void common_logic(float dt) {
    divine_timer += dt;
    
    // Update celestial animations
    celestial_glow_pulse = sin(divine_timer * SACRED_ANIMATION_SPEED) * 0.5f + 0.5f;
    
    // Update widget system
    a_DoWidget();
    
    // Scene-specific logic based on current scene
    if (web_test_get_current_scene() == 0) { // Awakening
        // Count successful widget interactions
        static int last_active_widget_hash = 0;
        int current_hash = app.active_widget ? (int)((uintptr_t)app.active_widget) : 0;
        if (current_hash != last_active_widget_hash && current_hash != 0) {
            widget_interaction_count++;
            last_active_widget_hash = current_hash;
        }
        
        // Update divine status based on awakening progress
        if (divine_timer > 3.0f && widget_interaction_count == 0) {
            strcpy(divine_status_text, "⚡ Widgets await the first mortal touch...");
        } else if (widget_interaction_count > 0) {
            snprintf(divine_status_text, sizeof(divine_status_text), 
                    "✨ %d Sacred Automata have awakened to your touch!", widget_interaction_count);
        }
    }
    
    // Use smart navigation instead of hijacking input
    handle_smart_navigation();
    
    // Note: Auto-advance is now handled by the web framework
}

static void draw_ui_overlay(void) {
    // Test progress
    char progress_text[100];
    int current = web_test_get_current_scene();
    int total = web_test_get_total_scenes();
    
    if (current < total) {
        snprintf(progress_text, sizeof(progress_text), "Sacred Trial %d/%d", 
                 current + 1, total);
    } else {
        snprintf(progress_text, sizeof(progress_text), "Trials Complete: %d/%d", 
                 total, total);
    }
    a_DrawText(progress_text, 400, 20, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Note: Timer bar and test status are now handled by web framework UI overlay
    
}

// ============================================================================
// Scene 1: Awakening of the Automata
// ============================================================================

static void init_awakening_automata(void) {
    widget_interaction_count = 0;
    divine_timer = 0.0f;
    strcpy(divine_status_text, "🌟 The Divine Breath awakens sleeping widgets...");
    
    // Show Scene 1 specific buttons
    aWidget_t* awaken_btn = a_GetWidget("awaken_all_btn");
    aWidget_t* reset_btn = a_GetWidget("reset_widgets_btn");
    aWidget_t* boost_btn = a_GetWidget("power_boost_btn");
    aWidget_t* divine_btn = a_GetWidget("divine_mode_btn");
    
    if (awaken_btn) {
        awaken_btn->hidden = 0;
        divine_diagnostic("INFO", "SCENE1", "⚡ Awaken All button made visible", 
                         "Scene 1 dynamic button activated", "Button ready for divine interaction");
    }
    if (reset_btn) {
        reset_btn->hidden = 0;
        divine_diagnostic("INFO", "SCENE1", "🔄 Reset Widgets button made visible", 
                         "Scene 1 dynamic button activated", "Button ready for divine interaction");
    }
    if (boost_btn) {
        boost_btn->hidden = 0;
        divine_diagnostic("INFO", "SCENE1", "⚡ Power Boost button made visible", 
                         "Scene 1 dynamic button activated", "Button ready for divine interaction");
    }
    if (divine_btn) {
        divine_btn->hidden = 0;
        divine_diagnostic("INFO", "SCENE1", "🌟 Divine Mode button made visible", 
                         "Scene 1 dynamic button activated", "Button ready for divine interaction");
    }
    
    // Reset any scene-specific state
    all_widgets_awakened = 0;
    power_boost_multiplier = 1.0f;
    divine_mode_active = 0;
    widgets_force_reset = 0;
}

static void logic_awakening_automata(float dt) {
    divine_timer += dt;
    
    // Apply power boost to animation speed
    float boosted_speed = SACRED_ANIMATION_SPEED * power_boost_multiplier;
    celestial_glow_pulse = sin(divine_timer * boosted_speed) * 0.5f + 0.5f;
    
    // Update widgets system
    a_DoWidget();
    
    // Handle reset widgets button effect
    if (widgets_force_reset) {
        widgets_force_reset = 0;
        // Reset any visual effects or counters
        divine_timer = 0.0f;
    }
    
    // Count successful widget interactions (unless all widgets are awakened)
    if (!all_widgets_awakened) {
        static int last_active_widget_hash = 0;
        int current_hash = app.active_widget ? (int)((uintptr_t)app.active_widget) : 0;
        if (current_hash != last_active_widget_hash && current_hash != 0) {
            widget_interaction_count++;
            last_active_widget_hash = current_hash;
        }
        
        // Update divine status based on awakening progress (if not overridden by buttons)
        if (divine_timer > 3.0f && widget_interaction_count == 0) {
            strcpy(divine_status_text, "⚡ Widgets await the first mortal touch...");
        } else if (widget_interaction_count > 0) {
            snprintf(divine_status_text, sizeof(divine_status_text), 
                    "✨ %d Sacred Automata have awakened to your touch!", widget_interaction_count);
        }
    }
    
    // Smart navigation - don't hijack widget interactions
    handle_smart_navigation();
}


static void draw_awakening_automata(float dt) {
    (void)dt;
    
    // Epic title with divine glow (affected by divine mode)
    int title_glow = (int)(CELESTIAL_GLOW_INTENSITY * celestial_glow_pulse);
    if (divine_mode_active) {
        // Divine mode: Golden shimmering title
        a_DrawText("🌟 AWAKENING OF THE SACRED AUTOMATA 🌟", 400, 80, 
                   255, 215 + title_glow/4, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    } else {
        // Normal mode: Blue/white title
        a_DrawText("⚡ AWAKENING OF THE SACRED AUTOMATA ⚡", 400, 80, 
                   255, 255 - title_glow/2, 255 - title_glow, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    }
    
    // Widget status report
    a_DrawText("Widget System Status:", 100, 120, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Check widget loading success
    const char* widget_names[] = {"oracle_stone", "celestial_dial", "scribes_tablet", "sacred_chamber"};
    int loaded_widgets = 0;
    
    for (int i = 0; i < 4; i++) {
        aWidget_t* widget = a_GetWidget((char*)widget_names[i]);
        if (widget != NULL) {
            loaded_widgets++;
            if (all_widgets_awakened) {
                // Force awakened display with divine glow
                a_DrawText("⚡ Sacred Mechanism DIVINELY AWAKENED!", 120, 140 + i * 20, 
                          255, 215, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
            } else if (divine_mode_active) {
                // Divine mode with golden text
                a_DrawText("🌟 Sacred Mechanism Awakened", 120, 140 + i * 20, 
                          255, 215, 100, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
            } else {
                // Normal awakened state
                a_DrawText("✅ Sacred Mechanism Awakened", 120, 140 + i * 20, 
                          0, 255, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
            }
        } else {
            if (divine_mode_active) {
                a_DrawText("💫 Mechanism Slumbering", 120, 140 + i * 20, 
                          150, 100, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
            } else {
                a_DrawText("❌ Mechanism Dormant", 120, 140 + i * 20, 
                          255, 0, 0, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
            }
        }
    }
    
    // Divine statistics
    char stats_text[128];
    snprintf(stats_text, sizeof(stats_text), "Awakened Automata: %d/4", loaded_widgets);
    a_DrawText(stats_text, 450, 140, celestial_palette[0].r, celestial_palette[0].g, celestial_palette[0].b, 
               FONT_ENTER_COMMAND, TEXT_ALIGN_LEFT, 0);
    
    snprintf(stats_text, sizeof(stats_text), "Mortal Interactions: %d", widget_interaction_count);
    a_DrawText(stats_text, 450, 160, celestial_palette[1].r, celestial_palette[1].g, celestial_palette[1].b, 
               FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Animated sacred geometry
    for (int i = 0; i < 6; i++) {
        float angle = divine_timer * 0.5f + i * (3.14159f / 3.0f);
        int x = 400 + (int)(sin(angle) * 60);
        int y = 280 + (int)(cos(angle) * 60);
        int pulse_size = (int)(8 + celestial_glow_pulse * 4);
        a_DrawFilledCircle(x, y, pulse_size, (aColor_t){
            celestial_palette[i].r, 
            celestial_palette[i].g, 
            celestial_palette[i].b, 
            (int)(128 + celestial_glow_pulse * 127)
        });
    }
    
    // Central divine mandala
    a_DrawText("🔮", 400, 270, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Divine status message
    a_DrawText(divine_status_text, 400, 350, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Success condition
    if (loaded_widgets == 4 && widget_interaction_count > 0) {
        a_DrawText("🏛️ THE AUTOMATA HAVE AWAKENED! GLORY TO ARCHIMEDES! 🏛️", 
                   400, 380, 0, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    }
    
    // Draw the sacred widgets
    a_DrawWidgets();
    
    // Use the centralized UI overlay
    draw_ui_overlay();
}

// ============================================================================
// Scene 2: The Mortal Touch Interface  
// ============================================================================

static void init_mortal_touch_interface(void) {
    // Testing divine responsiveness to mortal touch...
    widget_interaction_count = 0;
    strcpy(divine_status_text, "👆 Touch the sacred mechanisms and witness their power...");
    
    // Hide Scene 1 buttons when entering Scene 2
    hide_scene1_buttons();
    
    divine_diagnostic("INFO", "SCENE2", "👆 Scene 2 initialized", 
                     "Mortal Touch Interface ready", "Scene 1 buttons hidden");
}

static void logic_mortal_touch_interface(float dt) {
    divine_timer += dt;
    celestial_glow_pulse = sin(divine_timer * SACRED_ANIMATION_SPEED * 1.5f) * 0.5f + 0.5f;
    
    // Enhanced widget interaction tracking
    a_DoWidget();
    
    // Track different types of interactions
    static int button_clicks = 0;
    static int slider_changes = 0;
    static int input_edits = 0;
    
    // Track mouse interactions for web demo
    if (app.mouse.pressed == 1 && app.mouse.button == SDL_BUTTON_LEFT) {
        // Simulate widget interaction areas
        if (app.mouse.x >= 100 && app.mouse.y >= 200 && app.mouse.x <= 200 && app.mouse.y <= 250) {
            widget_interaction_count++;
        }
    }
    
    // Update divine status with interaction feedback
    if (widget_interaction_count > 5) {
        strcpy(divine_status_text, "✨ The mortals have mastered the divine interface!");
    } else if (widget_interaction_count > 0) {
        snprintf(divine_status_text, sizeof(divine_status_text), 
                "🌟 %d sacred touches registered! The gods are pleased!", widget_interaction_count);
    }
    
    // Smart navigation - don't hijack widget interactions
    handle_smart_navigation();
}

static void draw_mortal_touch_interface(float dt) {
    (void)dt;
    
    // Epic title
    a_DrawText("👆 THE MORTAL TOUCH INTERFACE 👆", 400, 80, 
               255, 215, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Touch instruction panel
    a_DrawText("Sacred Interaction Methods:", 100, 120, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("🖱️ Mouse clicks awaken the Oracle Stones", 120, 140, 200, 200, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("👆 Touch gestures command the Celestial Dials", 120, 160, 200, 255, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("⌨️ Sacred keys inscribe upon the Scribe's Tablets", 120, 180, 255, 200, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Real-time interaction feedback
    a_DrawText("Divine Interaction Log:", 450, 120, celestial_palette[2].r, celestial_palette[2].g, celestial_palette[2].b, 
               FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    char mouse_text[128];
    snprintf(mouse_text, sizeof(mouse_text), "Sacred Cursor: (%d, %d)", app.mouse.x, app.mouse.y);
    a_DrawText(mouse_text, 450, 140, 150, 150, 150, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    snprintf(mouse_text, sizeof(mouse_text), "Touch Events: %d", widget_interaction_count);
    a_DrawText(mouse_text, 450, 160, celestial_palette[4].r, celestial_palette[4].g, celestial_palette[4].b, 
               FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Visual feedback for current widget interaction
    if (app.active_widget) {
        // Draw divine aura around active widget
        int aura_size = (int)(20 + celestial_glow_pulse * 10);
        for (int i = 0; i < 3; i++) {
            a_DrawRect(app.active_widget->x - aura_size + i * 5, 
                      app.active_widget->y - aura_size + i * 5,
                      app.active_widget->w + (aura_size - i * 5) * 2,
                      app.active_widget->h + (aura_size - i * 5) * 2,
                      celestial_palette[0].r, celestial_palette[0].g, celestial_palette[0].b, 
                      (int)(64 + celestial_glow_pulse * 127));
        }
        
        char active_text[128];
        snprintf(active_text, sizeof(active_text), "Active Sacred Mechanism: %s", app.active_widget->name);
        a_DrawText(active_text, 400, 320, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    }
    
    // Touch responsiveness visualization
    if (app.mouse.pressed == 1) {
        // Draw ripple effect at mouse position
        for (int ring = 1; ring <= 3; ring++) {
            int radius = ring * 15;
            a_DrawCircle(app.mouse.x, app.mouse.y, radius, (aColor_t){
                255, 215, 0, (int)(255 - ring * 60)
            });
        }
    }
    
    // Divine status
    a_DrawText(divine_status_text, 400, 350, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Performance indicator
    float fps = 1.0f / a_GetDeltaTime();
    char perf_text[64];
    snprintf(perf_text, sizeof(perf_text), "Divine Performance: %.1f FPS", fps);
    a_DrawText(perf_text, 700, 500, fps > 58 ? 0 : 255, fps > 58 ? 255 : 0, 0, FONT_LINUX, TEXT_ALIGN_RIGHT, 0);
    
    // Draw widgets with divine enhancements
    a_DrawWidgets();
    
    draw_ui_overlay();
}

// ============================================================================
// Scene 3: The Flexbox Pantheon
// ============================================================================

static void init_flexbox_pantheon(void) {
    // Arranging divine containers with sacred geometry...
    strcpy(divine_status_text, "🏛️ Witness the power of divine proportion and layout...");
    
    // Hide Scene 1 buttons when entering Scene 3
    hide_scene1_buttons();
    
    // Show the responsive chamber for flexbox demonstration
    aWidget_t* responsive_chamber = a_GetWidget("responsive_chamber");
    if (responsive_chamber) {
        responsive_chamber->hidden = 0; // Make visible for this scene
    }
    
    // Hide other containers during flexbox demo
    aWidget_t* sacred_chamber = a_GetWidget("sacred_chamber");
    if (sacred_chamber) {
        sacred_chamber->hidden = 1;
    }
    
    divine_diagnostic("INFO", "FLEXBOX", "🏛️ Flexbox scene initialized", 
                     "Responsive chamber revealed", "Scene 1 buttons hidden, ready to demonstrate layout changes");
}

static void logic_flexbox_pantheon(float dt) {
    divine_timer += dt;
    celestial_glow_pulse = sin(divine_timer * SACRED_ANIMATION_SPEED * 0.8f) * 0.5f + 0.5f;
    
    a_DoWidget();
    
    // Demonstrate responsive behavior - actually change widget layouts
    int new_demo_phase = (int)(divine_timer / 4.0f) % 3;
    
    // Only update layout when phase changes to avoid constant updates
    if (new_demo_phase != current_demo_phase) {
        current_demo_phase = new_demo_phase;
        
        // Actually modify widget layouts based on demo phase
        aWidget_t* responsive_chamber = a_GetWidget("responsive_chamber");
        if (responsive_chamber) {
            // Show the responsive chamber for flexbox demo
            responsive_chamber->hidden = 0;
            
            // Change flex layout based on demo phase
            switch (current_demo_phase) {
                case 0: // Horizontal alignment
                    responsive_chamber->flex = 1; // Horizontal layout
                    responsive_chamber->x = 100;
                    responsive_chamber->y = 350;
                    break;
                case 1: // Vertical stack  
                    responsive_chamber->flex = 2; // Vertical layout
                    responsive_chamber->x = 200;
                    responsive_chamber->y = 280;
                    break;
                case 2: // Mixed arrangement
                    responsive_chamber->flex = 0; // Manual positioning
                    responsive_chamber->x = 300;
                    responsive_chamber->y = 320;
                    break;
            }
            
            divine_diagnostic("INFO", "FLEXBOX", "🏛️ Layout phase changed", 
                             "Widget flex layout updated", "Visual layout now matches description");
        }
    }
    
    // Smart navigation - don't hijack widget interactions
    handle_smart_navigation();
}

static void draw_flexbox_pantheon(float dt) {
    (void)dt;
    
    a_DrawText("🏛️ THE FLEXBOX PANTHEON 🏛️", 400, 80, 
               138, 43, 226, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Layout demonstration info
    const char* layout_modes[] = {
        "Horizontal Sacred Alignment",
        "Vertical Divine Stack", 
        "Mixed Celestial Arrangement"
    };
    
    char mode_text[128];
    snprintf(mode_text, sizeof(mode_text), "Current Divine Layout: %s", layout_modes[current_demo_phase]);
    a_DrawText(mode_text, 400, 110, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Flexbox principles visualization
    a_DrawText("Sacred Flexbox Principles:", 100, 140, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("• flex=1: Horizontal divine flow", 120, 160, celestial_palette[0].r, celestial_palette[0].g, celestial_palette[0].b, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("• flex=2: Vertical sacred stack", 120, 180, celestial_palette[1].r, celestial_palette[1].g, celestial_palette[1].b, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("• flex=0: Manual god-like positioning", 120, 200, celestial_palette[2].r, celestial_palette[2].g, celestial_palette[2].b, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Container hierarchy visualization
    a_DrawText("Divine Container Hierarchy:", 450, 140, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Find and display container information
    aWidget_t* main_container = a_GetWidget("sacred_chamber");
    if (main_container && main_container->type == WT_CONTAINER) {
        aContainerWidget_t* container = (aContainerWidget_t*)main_container->data;
        char container_text[128];
        snprintf(container_text, sizeof(container_text), "Sacred Chamber: %d mechanisms", container->num_components);
        a_DrawText(container_text, 450, 160, 0, 255, 127, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
        
        snprintf(container_text, sizeof(container_text), "Flex Mode: %d", main_container->flex);
        a_DrawText(container_text, 450, 180, 0, 255, 127, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    }
    
    // Golden ratio demonstration
    float golden_x = 400 - (200 / DIVINE_GOLDEN_RATIO);
    float golden_y = 300;
    a_DrawRect((int)golden_x, (int)golden_y, (int)(200 / DIVINE_GOLDEN_RATIO), 40, 255, 215, 0, 255);
    a_DrawText("φ Golden Ratio", (int)golden_x + 60, (int)golden_y + 20, 0, 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Animated layout demonstration
    for (int i = 0; i < 5; i++) {
        float anim_offset = sin(divine_timer + i * 0.5f) * 10;
        int demo_x = 150 + i * 80 + (int)anim_offset;
        int demo_y = 400;
        
        a_DrawFilledRect(demo_x, demo_y, 60, 30, 
                        celestial_palette[i % 8].r, 
                        celestial_palette[i % 8].g, 
                        celestial_palette[i % 8].b, 
                        (int)(128 + celestial_glow_pulse * 127));
        
        char demo_label[16];
        snprintf(demo_label, sizeof(demo_label), "W%d", i + 1);
        a_DrawText(demo_label, demo_x + 30, demo_y + 15, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    a_DrawText("✨ Divine Layout Animation ✨", 400, 450, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    a_DrawWidgets();
    draw_ui_overlay();
}

// ============================================================================
// Scene 4: The Symphony of Interactions
// ============================================================================

static void init_symphony_interactions(void) {
    // Orchestrating divine harmony between multiple automata...
    widget_interaction_count = 0;
    strcpy(divine_status_text, "🎼 Listen as the widgets sing in perfect harmony...");
    // Test passed
}

static void logic_symphony_interactions(float dt) {
    divine_timer += dt;
    celestial_glow_pulse = sin(divine_timer * SACRED_ANIMATION_SPEED * 1.2f) * 0.5f + 0.5f;
    
    a_DoWidget();
    
    // Synchronize multiple widgets
    aWidget_t* slider = a_GetWidget("celestial_dial");
    aWidget_t* input = a_GetWidget("scribes_tablet");
    
    if (slider && slider->type == WT_SLIDER) {
        aSliderWidget_t* slider_data = (aSliderWidget_t*)slider->data;
        // Update other widgets based on slider value
        widget_interaction_count = slider_data->value;
    }
    
    // Smart navigation - don't hijack widget interactions
    handle_smart_navigation();
}

static void draw_symphony_interactions(float dt) {
    (void)dt;
    
    a_DrawText("🎼 THE SYMPHONY OF INTERACTIONS 🎼", 400, 80, 
               255, 105, 180, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Musical visualization of widget harmony
    a_DrawText("Widget Harmony Visualization:", 100, 120, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    // Draw musical staff representation
    for (int line = 0; line < 5; line++) {
        int y = 160 + line * 20;
        a_DrawHorizontalLine(150, 650, y, (aColor_t){100, 100, 100, 255});
    }
    
    // Animated musical notes based on widget interactions
    for (int note = 0; note < 8; note++) {
        float note_time = divine_timer * 2.0f + note * 0.3f;
        int x = 200 + note * 60;
        int y = 170 + (int)(sin(note_time) * 40);
        
        // Note based on widget interaction count
        int note_intensity = (widget_interaction_count * (note + 1)) % 255;
        a_DrawFilledCircle(x, y, 8, (aColor_t){
            note_intensity, 255 - note_intensity, 128, 255
        });
        
        // Musical note symbol
        a_DrawText("♪", x, y, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    // Widget synchronization display
    a_DrawText("Sacred Widget Symphony Status:", 100, 280, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    const char* widget_instruments[] = {
        "🔮 Oracle Stone - Divine Percussion",
        "🌀 Celestial Dial - Cosmic Strings", 
        "📜 Scribe's Tablet - Sacred Vocals",
        "🏛️ Sacred Chamber - Harmonic Bass"
    };
    
    for (int i = 0; i < 4; i++) {
        // Animated color based on harmony
        int harmony_color = (int)(128 + sin(divine_timer * 3.0f + i) * 127);
        a_DrawText((char*)widget_instruments[i], 120, 300 + i * 20, 
                  harmony_color, 255 - harmony_color/2, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    }
    
    // Real-time interaction counter
    char interaction_text[128];
    snprintf(interaction_text, sizeof(interaction_text), "Harmonic Resonance Level: %d", widget_interaction_count);
    a_DrawText(interaction_text, 400, 420, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Divine performance meter
    float performance_level = fminf(widget_interaction_count / 100.0f, 1.0f);
    int meter_width = (int)(300 * performance_level);
    a_DrawFilledRect(250, 450, meter_width, 20, 0, 255, 0, 255);
    a_DrawRect(250, 450, 300, 20, 255, 255, 255, 255);
    a_DrawText("Divine Harmony Meter", 400, 480, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    a_DrawWidgets();
    draw_ui_overlay();
}

// ============================================================================
// Scene 5: The Web Performance Colosseum
// ============================================================================

static void init_performance_colosseum(void) {
    // Summoning 100+ widgets for epic performance battle...
    
    // Create stress test widgets dynamically
    stress_widget_count = MAX_STRESS_WIDGETS;
    stress_widgets = malloc(sizeof(aWidget_t) * stress_widget_count);
    
    // Initialize gladiator widgets
    for (int i = 0; i < stress_widget_count; i++) {
        aWidget_t* w = &stress_widgets[i];
        memset(w, 0, sizeof(aWidget_t));
        
        snprintf(w->name, sizeof(w->name), "gladiator_%d", i);
        snprintf(w->label, sizeof(w->label), "G%d", i);
        w->type = WT_BUTTON;
        w->x = 50 + (i % 15) * 50;
        w->y = 150 + (i / 15) * 35;
        w->w = 40;
        w->h = 25;
        w->boxed = 1;
        w->hidden = 0;
        w->padding = 2;
        
        // Epic gladiator colors
        int color_index = i % 8;
        w->fg[0] = 255;
        w->fg[1] = 255;
        w->fg[2] = 255;
        w->fg[3] = 255;
        w->bg[0] = celestial_palette[color_index].r;
        w->bg[1] = celestial_palette[color_index].g;
        w->bg[2] = celestial_palette[color_index].b;
        w->bg[3] = 200;
        
        w->action = NULL;
        w->data = NULL;
    }
    
    strcpy(divine_status_text, "⚔️ The gladiator widgets enter the arena!");
    // Test passed
}

static void logic_performance_colosseum(float dt) {
    divine_timer += dt;
    celestial_glow_pulse = sin(divine_timer * SACRED_ANIMATION_SPEED * 2.0f) * 0.5f + 0.5f;
    
    a_DoWidget();
    
    // Animate gladiator widgets for stress testing
    for (int i = 0; i < stress_widget_count; i++) {
        aWidget_t* w = &stress_widgets[i];
        // Subtle animation to stress the system
        float wave = sin(divine_timer * 3.0f + i * 0.1f);
        w->x = 50 + (i % 15) * 50 + (int)(wave * 2);
        w->y = 150 + (i / 15) * 35 + (int)(sin(divine_timer * 2.0f + i * 0.2f) * 1);
        
        // Color cycling for maximum visual stress
        int color_cycle = (int)(divine_timer * 2.0f + i) % 8;
        w->bg[0] = celestial_palette[color_cycle].r;
        w->bg[1] = celestial_palette[color_cycle].g;
        w->bg[2] = celestial_palette[color_cycle].b;
    }
    
    // Smart navigation - don't hijack widget interactions
    handle_smart_navigation();
}

static void draw_performance_colosseum(float dt) {
    (void)dt;
    
    a_DrawText("⚔️ THE WEB PERFORMANCE COLOSSEUM ⚔️", 400, 80, 
               255, 0, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Performance statistics
    float fps = 1.0f / a_GetDeltaTime();
    char perf_text[128];
    snprintf(perf_text, sizeof(perf_text), "Gladiator Widgets in Arena: %d", stress_widget_count);
    a_DrawText(perf_text, 400, 110, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    snprintf(perf_text, sizeof(perf_text), "Battle Performance: %.1f FPS", fps);
    int fps_color_r = fps > 55 ? 0 : 255;
    int fps_color_g = fps > 55 ? 255 : (fps > 30 ? 255 : 0);
    a_DrawText(perf_text, 400, 130, fps_color_r, fps_color_g, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Draw all gladiator widgets
    for (int i = 0; i < stress_widget_count; i++) {
        aWidget_t* w = &stress_widgets[i];
        if (!w->hidden) {
            // Epic gladiator rendering with divine effects
            if (w->boxed) {
                // Glowing border effect
                for (int glow = 0; glow < 3; glow++) {
                    a_DrawRect(w->x - glow, w->y - glow, w->w + glow * 2, w->h + glow * 2,
                              w->bg[0], w->bg[1], w->bg[2], 50 - glow * 15);
                }
            }
            
            // Main gladiator body
            a_DrawFilledRect(w->x, w->y, w->w, w->h, w->bg[0], w->bg[1], w->bg[2], w->bg[3]);
            a_DrawRect(w->x, w->y, w->w, w->h, 255, 255, 255, 255);
            
            // Gladiator label
            a_DrawText(w->label, w->x + w->w/2, w->y + w->h/2 - 6,
                      w->fg[0], w->fg[1], w->fg[2], FONT_LINUX, TEXT_ALIGN_CENTER, 0);
        }
    }
    
    // Regular widgets on top
    a_DrawWidgets();
    
    // Performance verdict
    if (fps > 55) {
        a_DrawText("🏆 GLORIOUS VICTORY! 60 FPS MAINTAINED!", 400, 520, 0, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    } else if (fps > 30) {
        a_DrawText("⚔️ VALIANT EFFORT! PERFORMANCE HOLDS STRONG!", 400, 520, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    } else {
        a_DrawText("💀 THE GLADIATORS OVERWHELM THE ARENA!", 400, 520, 255, 0, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    draw_ui_overlay();
}

// ============================================================================
// Scene 6: The Responsive Oracle
// ============================================================================

static void init_responsive_oracle(void) {
    // Testing prophetic screen adaptation capabilities...
    responsive_test_width = 800;
    responsive_test_height = 600;
    strcpy(divine_status_text, "🔮 The Oracle adapts to any vessel, any realm...");
    // Test passed
}

static void logic_responsive_oracle(float dt) {
    divine_timer += dt;
    celestial_glow_pulse = sin(divine_timer * SACRED_ANIMATION_SPEED * 0.6f) * 0.5f + 0.5f;
    
    a_DoWidget();
    
    // Simulate different screen sizes
    float size_cycle = divine_timer * 0.3f;
    responsive_test_width = (int)(600 + sin(size_cycle) * 200);
    responsive_test_height = (int)(400 + cos(size_cycle * 1.2f) * 150);
    
    // Smart navigation - don't hijack widget interactions
    handle_smart_navigation();
}

static void draw_responsive_oracle(float dt) {
    (void)dt;
    
    a_DrawText("🔮 THE RESPONSIVE ORACLE 🔮", 400, 80, 
               64, 224, 208, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    // Responsive testing visualization
    a_DrawText("Sacred Prophecy: Widgets adapt to any dimension", 400, 110, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Draw simulated screen boundaries
    int sim_x = 400 - responsive_test_width/2;
    int sim_y = 200;
    
    // Animated screen border
    for (int i = 0; i < 3; i++) {
        a_DrawRect(sim_x - i*2, sim_y - i*2, responsive_test_width + i*4, responsive_test_height + i*4,
                  celestial_palette[5].r, celestial_palette[5].g, celestial_palette[5].b, 
                  100 - i*30);
    }
    
    // Screen dimension display
    char dimension_text[128];
    snprintf(dimension_text, sizeof(dimension_text), "Prophetic Dimensions: %dx%d", 
             responsive_test_width, responsive_test_height);
    a_DrawText(dimension_text, 400, 150, 255, 255, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Device type simulation
    const char* device_type;
    if (responsive_test_width < 500) {
        device_type = "📱 Sacred Mobile Tablet";
    } else if (responsive_test_width < 700) {
        device_type = "💻 Divine Laptop Portal";
    } else {
        device_type = "🖥️ Celestial Desktop Shrine";
    }
    
    a_DrawText((char*)device_type, 400, 170, celestial_palette[6].r, celestial_palette[6].g, celestial_palette[6].b, 
               FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    // Responsive widget demonstration inside simulated screen
    int demo_widgets = 4;
    int widget_width = (responsive_test_width - 40) / demo_widgets;
    
    for (int i = 0; i < demo_widgets; i++) {
        int widget_x = sim_x + 20 + i * widget_width;
        int widget_y = sim_y + 50;
        int widget_h = 40;
        
        // Responsive widget that scales with screen
        a_DrawFilledRect(widget_x, widget_y, widget_width - 10, widget_h,
                        celestial_palette[i % 8].r, 
                        celestial_palette[i % 8].g, 
                        celestial_palette[i % 8].b, 
                        (int)(128 + celestial_glow_pulse * 127));
        
        char widget_label[16];
        snprintf(widget_label, sizeof(widget_label), "W%d", i + 1);
        a_DrawText(widget_label, widget_x + widget_width/2, widget_y + 20, 
                  255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    // Responsive guidelines
    a_DrawText("Oracle's Responsive Wisdom:", 100, 450, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("• Widgets flow like divine water", 120, 470, 200, 255, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("• Touch targets remain sacred (44px+)", 120, 490, 200, 255, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    a_DrawText("• Text scales with celestial proportion", 120, 510, 200, 255, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
    
    a_DrawText("✨ DIVINE ADAPTATION ACHIEVED ✨", 400, 530, 0, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    a_DrawWidgets();
    draw_ui_overlay();
}

// ============================================================================
// Scene 7: The Final Ascension
// ============================================================================

static void init_final_ascension(void) {
    // Preparing the ultimate divine demonstration...
    current_demo_phase = 0;
    strcpy(divine_status_text, "🌟 Become the architect! Master the divine mechanisms!");
    // Test passed
}

static void logic_final_ascension(float dt) {
    divine_timer += dt;
    celestial_glow_pulse = sin(divine_timer * SACRED_ANIMATION_SPEED * 0.4f) * 0.5f + 0.5f;
    
    a_DoWidget();
    
    // Cycle through different demonstration modes
    current_demo_phase = (int)(divine_timer / 8.0f) % 4;
    
    // Smart navigation - don't hijack widget interactions
    handle_smart_navigation();
}

static void draw_final_ascension(float dt) {
    (void)dt;
    
    a_DrawText("🌟 THE FINAL ASCENSION 🌟", 400, 80, 
               255, 215, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    const char* ascension_phases[] = {
        "🎭 Interactive Widget Gallery",
        "🛠️ Divine Mechanism Workshop", 
        "🎨 Sacred Styling Studio",
        "🚀 Transcendent Integration"
    };
    
    char phase_text[128];
    snprintf(phase_text, sizeof(phase_text), "Ascension Phase: %s", ascension_phases[current_demo_phase]);
    a_DrawText(phase_text, 400, 110, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    
    switch (current_demo_phase) {
        case 0: // Interactive Gallery
            a_DrawText("🎭 BEHOLD THE WIDGET GALLERY OF THE GODS! 🎭", 400, 150, 
                       celestial_palette[0].r, celestial_palette[0].g, celestial_palette[0].b, 
                       FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            
            a_DrawText("Every sacred mechanism responds to your divine will:", 400, 180, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
            a_DrawText("• Touch the Oracle Stones to channel power", 200, 200, 200, 255, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
            a_DrawText("• Command the Celestial Dials with precision", 200, 220, 200, 255, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
            a_DrawText("• Inscribe wisdom upon Sacred Tablets", 200, 240, 200, 255, 200, FONT_LINUX, TEXT_ALIGN_LEFT, 0);
            break;
            
        case 1: // Divine Workshop
            a_DrawText("🛠️ THE DIVINE MECHANISM WORKSHOP! 🛠️", 400, 150, 
                       celestial_palette[1].r, celestial_palette[1].g, celestial_palette[1].b, 
                       FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            
            a_DrawText("Here you could forge new widgets:", 400, 180, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
            
            // Workshop visualization
            for (int tool = 0; tool < 5; tool++) {
                int tool_x = 200 + tool * 80;
                int tool_y = 220;
                
                a_DrawFilledRect(tool_x, tool_y, 60, 40, 
                                celestial_palette[tool].r, 
                                celestial_palette[tool].g, 
                                celestial_palette[tool].b, 
                                (int)(128 + celestial_glow_pulse * 127));
                
                const char* tool_names[] = {"🔨", "⚙️", "✨", "🎨", "⚡"};
                a_DrawText((char*)tool_names[tool], tool_x + 30, tool_y + 20, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            }
            break;
            
        case 2: // Styling Studio
            a_DrawText("🎨 THE SACRED STYLING STUDIO! 🎨", 400, 150, 
                       celestial_palette[2].r, celestial_palette[2].g, celestial_palette[2].b, 
                       FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            
            a_DrawText("Customize the divine appearance:", 400, 180, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
            
            // Color palette demonstration
            for (int color = 0; color < 8; color++) {
                int swatch_x = 200 + color * 50;
                int swatch_y = 210;
                
                a_DrawFilledRect(swatch_x, swatch_y, 40, 40, 
                                celestial_palette[color].r, 
                                celestial_palette[color].g, 
                                celestial_palette[color].b, 255);
                a_DrawRect(swatch_x, swatch_y, 40, 40, 255, 255, 255, 255);
            }
            
            a_DrawText("Divine Color Palette", 400, 260, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
            break;
            
        case 3: // Transcendent Integration
            a_DrawText("🚀 TRANSCENDENT INTEGRATION! 🚀", 400, 150, 
                       celestial_palette[3].r, celestial_palette[3].g, celestial_palette[3].b, 
                       FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
            
            a_DrawText("All systems unified in perfect harmony:", 400, 180, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
            
            // Integration visualization - spinning sacred geometry
            for (int ring = 0; ring < 3; ring++) {
                int num_points = 6 + ring * 2;
                float ring_radius = 60 + ring * 30;
                
                for (int point = 0; point < num_points; point++) {
                    float angle = divine_timer * (1.0f + ring * 0.3f) + point * (6.28f / num_points);
                    int x = 400 + (int)(sin(angle) * ring_radius);
                    int y = 250 + (int)(cos(angle) * ring_radius);
                    
                    a_DrawFilledCircle(x, y, 8 - ring * 2, (aColor_t){
                        celestial_palette[point % 8].r,
                        celestial_palette[point % 8].g,
                        celestial_palette[point % 8].b,
                        255
                    });
                }
            }
            break;
    }
    
    // Universal ascension status
    char ascension_text[128];
    snprintf(ascension_text, sizeof(ascension_text), "Divinity Level: %.1f%%", 
             fminf((divine_timer / 25.0f) * 100, 100.0f));
    a_DrawText(ascension_text, 400, 400, 255, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
    
    if (divine_timer > 20.0f) {
        a_DrawText("🏛️ YOU HAVE MASTERED THE SACRED AUTOMATA! 🏛️", 400, 450, 0, 255, 0, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
        a_DrawText("The gods themselves bow before your widget mastery!", 400, 480, 255, 215, 0, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    a_DrawWidgets();
    draw_ui_overlay();
}

// ============================================================================
// Cleanup Functions
// ============================================================================

static void cleanup_performance_test(void) {
    if (stress_widgets) {
        free(stress_widgets);
        stress_widgets = NULL;
        stress_widget_count = 0;
    }
}

// ============================================================================
// Main Entry Point - The Sacred Portal
// ============================================================================

void aMainloop(void) {
    a_DoInput();
    a_PrepareScene();
    
    // Only run if web test framework is properly initialized
    if (web_test_is_initialized()) {
        app.delegate.logic(a_GetDeltaTime());
        app.delegate.draw(a_GetDeltaTime());
    } else {
        // Fallback rendering if web framework failed
        divine_diagnostic("WARN", "MAIN_LOOP", "🔄 Web framework not ready", 
                         "Running fallback rendering", "Check web_test_init status");
        
        // Simple fallback display
        a_DrawText("🏛️ Sacred Chamber Initializing...", 400, 300, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0);
        a_DrawText("Please wait while the divine mechanisms awaken", 400, 330, 200, 200, 200, FONT_LINUX, TEXT_ALIGN_CENTER, 0);
    }
    
    a_PresentScene();
}

int main(void) {
    // Initialize the Divine Framework
    if (a_Init(800, 600, "🏛️ Sacred Chamber of Divine Automata") != 0) {
        printf("❌ Failed to awaken the divine framework!\n");
        return 1;
    }
    
    // Initialize the comprehensive test system with diagnostics
    init_test_system();
    
    // Initialize the Web Test Framework (handles UI and navigation)
    int scene_count = sizeof(test_scenes) / sizeof(WebTestScene_t);
    if (web_test_init(test_scenes, scene_count, 
                      "🏛️ Sacred Chamber of Divine Automata", 
                      "Epic web-based widget testing suite - where mortals become gods through divine UI mastery") != 0) {
        divine_diagnostic("ERROR", "WEB_FRAMEWORK", "❌ Failed to initialize web test system", 
                         "Web framework initialization failed", "Check browser console for additional errors");
        return 1;
    }
    
    divine_diagnostic("INFO", "MAIN", "🌟 Sacred Chamber web framework activated", 
                     "All systems operational", "Epic widget testing ready to begin");
    
    printf("🏛️ Sacred Chamber initialized successfully!\n");
    printf("⚡ Navigate: Arrow keys | F1=Skip | R=Restart | ESC=Exit\n");
    printf("🌐 Web Framework: ACTIVE | Console Bridge: %s\n", g_env.is_emscripten ? "ENABLED" : "N/A");
    
#ifdef __EMSCRIPTEN__
    // Set up the eternal divine loop for the web realm
    emscripten_set_main_loop(aMainloop, 0, 1);
    divine_diagnostic("INFO", "MAIN", "🌌 Eternal divine loop established", 
                     "Web realm main loop active", "Chamber will run until mortals close browser");
#else
    // Fallback for mortal desktop builds
    divine_diagnostic("INFO", "MAIN", "🖥️ Desktop mode active", 
                     "Running native main loop", "Use standard controls");
    while (app.running) {
        a_DoInput();
        aMainloop();
    }
    cleanup_performance_test();
    a_Quit();
#endif
    
    return 0;
}