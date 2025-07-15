// aInitializeTest.c - Test-friendly initialization that uses smaller fonts

#include <stdlib.h>
#include <time.h>
#include "Archimedes.h"

// Define the global app instance (normally in aInitialize.c)
aApp_t app;

// Define log level strings (normally in aInitialize.c)
char* log_level_strings[LOG_LEVEL_COUNT] = {
    "NORMAL",
    "WARNING", 
    "FATAL",
    "INFO",
    "DEBUG",
    "TRACE",
};

// External test font initialization function
extern void a_InitTestFonts(void);

/**
 * @brief Initializes Archimedes for testing with smaller fonts.
 *
 * This is a test-friendly version of a_Init that uses smaller font sizes
 * to avoid glyph atlas overflow. It's identical to a_Init except it calls
 * a_InitTestFonts() instead of a_InitFonts().
 *
 * @param width Window width in pixels (must be > 0)
 * @param height Window height in pixels (must be > 0) 
 * @param title Window title string (cannot be NULL)
 * @return 0 on success, negative value on failure
 */
int a_InitTest(const int width, const int height, const char *title) {
    // Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        return -1;
    }
    
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Quit();
        return -2;
    }
    
    if (TTF_Init() < 0) {
        IMG_Quit();
        SDL_Quit();
        return -3;
    }
    
    // Create window and renderer
    if (SDL_CreateWindowAndRenderer(width, height, 0, &app.window, &app.renderer) < 0) {
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return -4;
    }
    
    // Set window title after window creation
    SDL_SetWindowTitle(app.window, title);

    // Initialize mouse state
    app.mouse = (aMouse_t){ .x = 0, .y = 0, .state = 0, .pressed = 0, .button = 0, .wheel = 0, .clicks = 0 };

    // Initialize timing system
    app.time.lastTime = 0;
    app.time.currentTime = 0;
    app.time.frameStart = 0;
    app.time.frameTime = 0;
    app.time.lastFrameCounterClear = 0;
    app.time.frames = 0;

    // Initialize image cache
    app.img_cache = NULL;

    // Set app to running state
    app.running = 1;

    // Initialize subsystems
    a_InitAudio();
    a_InitImage();
    a_InitTextures();
    a_InitTestFonts(); // Use test fonts instead of regular fonts

    // Seed random number generator
    srand(time(NULL));

    return 0;
}

/**
 * @brief Cleans up all Archimedes resources and shuts down SDL.
 *
 * This is the same as the regular a_Quit() function. Included here
 * so that test initialization can be self-contained.
 */
void a_Quit(void) {
    // Call optional exit delegate
    if (app.delegate.onExit) {
        app.delegate.onExit();
    }
    
    // Destroy SDL resources
    if (app.renderer) {
        SDL_DestroyRenderer(app.renderer);
        app.renderer = NULL;
    }
    
    if (app.window) {
        SDL_DestroyWindow(app.window);
        app.window = NULL;
    }
    
    // Clean up image cache
    if (app.img_cache) {
        a_CleanUpImageCache(app.img_cache);
        free(app.img_cache);
        app.img_cache = NULL;
    }
    
    // Shutdown SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    // Reset app state
    app.running = 0;
}