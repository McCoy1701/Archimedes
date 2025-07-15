#include <stdlib.h>
#include <time.h>

#include "Archimedes.h"

aApp_t app;

/**
 * @brief Initializes the Archimedes framework and SDL subsystems.
 *
 * This function sets up the complete Archimedes environment including SDL video,
 * audio, and timer subsystems, creates a window and renderer, initializes all
 * app state variables, and sets up the audio, image, texture, and font systems.
 * The random number generator is also seeded with the current time.
 *
 * The initialization sequence includes:
 * - SDL video, audio, and timer subsystem initialization
 * - PNG image format support initialization
 * - TTF font support initialization
 * - Window and renderer creation
 * - Mouse and timing state initialization
 * - Image cache setup
 * - Audio, image, texture, and font system initialization
 * - Random number generator seeding
 *
 * @param width Window width in pixels (must be > 0)
 * @param height Window height in pixels (must be > 0)
 * @param title Window title string (cannot be NULL)
 * @return 0 on success, negative value on failure
 */
int a_Init( const int width, const int height, const char *title )
{
  // Initialize SDL subsystems
  if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER ) < 0) {
    return -1;
  }
  
  if (IMG_Init( IMG_INIT_PNG ) == 0) {
    SDL_Quit();
    return -2;
  }
  
  if (TTF_Init() < 0) {
    IMG_Quit();
    SDL_Quit();
    return -3;
  }
  
  // Create window and renderer
  if (SDL_CreateWindowAndRenderer( width, height, 0, &app.window, &app.renderer ) < 0) {
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return -4;
  }
  
  // Set window title after window creation
  SDL_SetWindowTitle( app.window, title );

  // Initialize mouse state
  app.mouse = (aMouse_t){ .x = 0, .y = 0, .state = 0, .pressed = 0, .button = 0, .wheel = 0, .clicks = 0 };

  // Initialize timing system
  app.time.lastTime    = 0;
  app.time.currentTime = 0;
  app.time.frameStart = 0;
  app.time.frameTime  = 0;
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
  a_InitFonts();

  // Seed random number generator
  srand(time(NULL));

  return 0;
}

/**
 * @brief Cleans up all Archimedes resources and shuts down SDL.
 *
 * This function performs complete cleanup of the Archimedes framework by
 * calling the optional onExit delegate callback, destroying the renderer
 * and window, cleaning up the image cache, and shutting down SDL subsystems.
 * Should be called before program termination to ensure proper resource cleanup.
 *
 * The cleanup sequence includes:
 * - Calling the optional onExit delegate callback if set
 * - Destroying the SDL renderer
 * - Destroying the SDL window
 * - Cleaning up the image cache
 * - Freeing image cache memory
 * - Shutting down SDL subsystems
 */
void a_Quit( void )
{
  // Call optional exit delegate
  if ( app.delegate.onExit ) {
    app.delegate.onExit();
  }
  
  // Destroy SDL resources
  if ( app.renderer ) {
    SDL_DestroyRenderer( app.renderer );
    app.renderer = NULL;
  }
  
  if ( app.window ) {
    SDL_DestroyWindow( app.window );
    app.window = NULL;
  }
  
  // Clean up image cache
  if ( app.img_cache ) {
    a_CleanUpImageCache( app.img_cache );
    free( app.img_cache );
    app.img_cache = NULL;
  }
  
  // Shutdown SDL subsystems
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  
  // Reset app state
  app.running = 0;
}


char* log_level_strings[LOG_LEVEL_COUNT] =
{
  "NORMAL",
  "WARNING",
  "FATAL",
  "INFO",
  "DEBUG",
  "TRACE",
};

