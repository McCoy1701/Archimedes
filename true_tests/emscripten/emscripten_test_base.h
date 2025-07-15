/**
 * @file emscripten_test_base.h
 * @brief Header for Emscripten test base infrastructure
 * 
 * This header provides the interface for creating web-based tests using
 * the Archimedes engine with Emscripten. It defines the test scene structure
 * and provides functions for web-optimized test execution.
 * 
 * @author Archimedes Team
 * @date 2025
 */

#ifndef EMSCRIPTEN_TEST_BASE_H
#define EMSCRIPTEN_TEST_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Type Definitions
// ============================================================================

/**
 * @brief Structure defining a web test scene
 * 
 * Each test scene represents a single test case or group of related tests
 * that can be navigated through using arrow keys in the web interface.
 */
typedef struct {
    const char* name;               ///< Display name for the test scene
    const char* description;        ///< Detailed description of what this scene tests
    float display_duration;         ///< Seconds to display in auto-advance mode
    void (*init_scene)(void);       ///< Function to initialize the scene
    void (*logic)(float dt);        ///< Function to handle scene logic
    void (*draw)(float dt);         ///< Function to draw the scene
} WebTestScene_t;

// ============================================================================
// Public API Functions
// ============================================================================

/**
 * @brief Initializes the web test system
 * 
 * Sets up the web-based test environment with the provided scenes and
 * configures the HTML interface for interactive testing.
 * 
 * @param scenes Array of test scenes to execute
 * @param scene_count Number of scenes in the array
 * @param title Test suite title (displayed in web UI)
 * @param description Test suite description (displayed in web UI)
 * @return 0 on success, -1 on failure
 */
int web_test_init(WebTestScene_t* scenes, int scene_count, const char* title, const char* description);

/**
 * @brief Gets the current scene index
 * @return Current scene index (0-based)
 */
int web_test_get_current_scene(void);

/**
 * @brief Gets the total number of scenes
 * @return Total scene count
 */
int web_test_get_total_scenes(void);

/**
 * @brief Checks if web test system is initialized
 * @return 1 if initialized, 0 otherwise
 */
int web_test_is_initialized(void);

/**
 * @brief Gets the web-optimized input logic function
 * 
 * This function handles arrow key navigation and other web-specific inputs.
 * It should be assigned to app.delegate.logic or called from your main logic loop.
 * 
 * @return Pointer to web input logic function
 */
void (*web_test_get_input_logic(void))(float);

/**
 * @brief Gets the web UI overlay drawing function
 * 
 * This function draws the common UI elements like progress bars, scene info,
 * and navigation hints. It should be called from your draw functions.
 * 
 * @return Pointer to web UI drawing function
 */
void (*web_test_get_ui_overlay(void))(float);

/**
 * @brief Sets the current scene test result
 * 
 * Call this function to indicate whether the current test scene passed or failed.
 * This affects the test statistics displayed in the web UI.
 * 
 * @param passed 1 if test passed, 0 if failed
 */
void web_test_set_result(int passed);

/**
 * @brief Updates the current test description
 * 
 * Dynamically updates the description shown in the web UI for the current scene.
 * Useful for providing real-time feedback about what the test is doing.
 * 
 * @param description New description text
 */
void web_test_update_description(const char* description);

// ============================================================================
// JavaScript Integration Functions
// ============================================================================

#ifdef __EMSCRIPTEN__
/**
 * @brief Called by JavaScript to advance to next scene
 * 
 * This function is exposed to JavaScript and called when the user clicks
 * the right arrow or uses touch/swipe gestures to advance.
 */
void emscripten_next_scene(void);

/**
 * @brief Called by JavaScript to go to previous scene
 * 
 * This function is exposed to JavaScript and called when the user clicks
 * the left arrow or uses touch/swipe gestures to go back.
 */
void emscripten_previous_scene(void);

/**
 * @brief Called by JavaScript to restart current scene
 * 
 * This function is exposed to JavaScript and called when the user clicks
 * the up arrow or uses touch/swipe gestures to restart.
 */
void emscripten_restart_scene(void);

/**
 * @brief Called by JavaScript to toggle auto-advance
 * 
 * This function is exposed to JavaScript and called when the user clicks
 * the down arrow or uses touch/swipe gestures to toggle auto-advance mode.
 */
void emscripten_toggle_auto_advance(void);
#endif

// ============================================================================
// Utility Macros
// ============================================================================

/**
 * @brief Macro to define a test scene with default settings
 * 
 * Usage:
 * @code
 * WEB_TEST_SCENE("My Test", "Tests something important", 5.0f, init_func, logic_func, draw_func)
 * @endcode
 */
#define WEB_TEST_SCENE(name, desc, duration, init_fn, logic_fn, draw_fn) \
    { name, desc, duration, init_fn, logic_fn, draw_fn }

/**
 * @brief Macro to define a quick test scene (3 seconds)
 * 
 * Usage:
 * @code
 * WEB_QUICK_TEST("Quick Test", "A quick test", init_func, logic_func, draw_func)
 * @endcode
 */
#define WEB_QUICK_TEST(name, desc, init_fn, logic_fn, draw_fn) \
    WEB_TEST_SCENE(name, desc, 3.0f, init_fn, logic_fn, draw_fn)

/**
 * @brief Macro to define a long test scene (8 seconds)
 * 
 * Usage:
 * @code
 * WEB_LONG_TEST("Long Test", "A detailed test", init_func, logic_func, draw_func)
 * @endcode
 */
#define WEB_LONG_TEST(name, desc, init_fn, logic_fn, draw_fn) \
    WEB_TEST_SCENE(name, desc, 8.0f, init_fn, logic_fn, draw_fn)

/**
 * @brief Macro to mark a test as passed
 * 
 * Usage:
 * @code
 * WEB_TEST_PASS();
 * @endcode
 */
#define WEB_TEST_PASS() web_test_set_result(1)

/**
 * @brief Macro to mark a test as failed
 * 
 * Usage:
 * @code
 * WEB_TEST_FAIL();
 * @endcode
 */
#define WEB_TEST_FAIL() web_test_set_result(0)

/**
 * @brief Macro to update test description
 * 
 * Usage:
 * @code
 * WEB_TEST_UPDATE("Currently testing feature X...");
 * @endcode
 */
#define WEB_TEST_UPDATE(desc) web_test_update_description(desc)

#ifdef __cplusplus
}
#endif

#endif // EMSCRIPTEN_TEST_BASE_H