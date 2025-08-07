#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#ifndef __ARCHIMEDES_H__
#define __ARCHIMEDES_H__

/*
---------------------------------------------------------------
---                         Defines                         ---
---------------------------------------------------------------
*/

#define FPS 60.0
#define LOGIC_RATE ( FPS / 1000 )

#define MAX_KEYBOARD_KEYS 350
#define LOG_LEVEL_COUNT   6

#define SCREEN_WIDTH_16_9_1  1600
#define SCREEN_HEIGHT_16_9_1 900
#define SCREEN_WIDTH_16_9_0  1280
#define SCREEN_HEIGHT_16_9_0 720

#define SCREEN_WIDTH_5_4  1280
#define SCREEN_HEIGHT_5_4 1024

#define SCREEN_WIDTH_4_3_2  1024
#define SCREEN_HEIGHT_4_3_2 768
#define SCREEN_WIDTH_4_3_1  800
#define SCREEN_HEIGHT_4_3_1 600
#define SCREEN_WIDTH_4_3_0  640
#define SCREEN_HEIGHT_4_3_0 480

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

#define FONT_SIZE 32 
#define FONT_TEXTURE_SIZE 512
#define MAX_GLYPHS 400
#define MAX_GLYPH_SIZE 8
#define MAX_WORD_LENGTH 32
#define MAX_LINE_LENGTH 1024

// Text system error codes
#define ARCH_TEXT_SUCCESS 0
#define ARCH_TEXT_ERROR_NULL_POINTER -1
#define ARCH_TEXT_ERROR_INVALID_FONT -2
#define ARCH_TEXT_ERROR_BUFFER_OVERFLOW -3
#define ARCH_TEXT_ERROR_INVALID_UTF8 -4

#ifndef __DAEDALUS_H__

#define MAX_LINE_LENGTH        1024
#define MAX_FILENAME_LENGTH    256
#define MAX_NAME_LENGTH        32
#define MAX_DESCRIPTION_LENGTH 256
#define MAX_INPUT_LENGTH       16
#define PI 3.14159265

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define STRCPY( dest, src ) strncpy( dest, src, sizeof( dest ) ); dest[sizeof(dest) - 1] = '\0'
#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'
#define RANDF(lower, upper) (((float)rand() / (float)(RAND_MAX)) * (upper - lower)) + lower
#define MAP( value, start0, start1, end0, end1 ) ( ( value - start0 ) * ( ( end1 - end0 ) / ( start1 - start0 ) ) + end0 )
#define LOG( msg ) printf( "%s | File: %s, Line: %d\n", msg, __FILE__, __LINE__ )

#endif

enum
{
  FONT_CODE_PAGE_437,
  FONT_ENTER_COMMAND,
  FONT_LINUX,
  FONT_GAME,
  FONT_MAX
};

/*
---------------------------------------------------------------
---                         Structs                         ---
---------------------------------------------------------------
*/

typedef struct
{
  int x, y;
  int w, h;
} aRect_t;

typedef struct
{
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
} aColor_t;

typedef struct
{
  char error_msg[MAX_LINE_LENGTH];
  int  error_type;
} aError_t;

typedef struct _widget_t
{
  int type;
  char name[MAX_FILENAME_LENGTH];
  aRect_t rect;
  char label[MAX_FILENAME_LENGTH];
  int boxed;
  int hidden;
  int padding;
  int flex;
  aColor_t fg;
  aColor_t bg;
  struct _widget_t* next;
  struct _widget_t* prev;
  void (*action)( void );
  void (*data);
} aWidget_t;

typedef struct
{
  aRect_t rect;
  int spacing;
  int num_components;
  aWidget_t* components;
} aContainerWidget_t;

typedef struct
{
  int num_options;
  char** options;
  aRect_t rect;
  int value;
  char* text_name[MAX_NAME_LENGTH];
} aSelectWidget_t;

typedef struct
{
  aRect_t rect;
  int value;
  int step;
  int wait_on_change;
} aSliderWidget_t;

typedef struct
{
  aRect_t rect;
  int max_length;
  char* text;
} aInputWidget_t;

typedef struct
{
  int x, y;
  int value;
} aControlWidget_t;

typedef struct
{
  int x;
  int y;
  uint8_t pressed;
  uint8_t button;
  uint8_t state;
  uint8_t clicks;
  int8_t  wheel;
} aMouse_t;

typedef struct _aImageCacheNode_t
{
  SDL_Surface* surf;
  int ID;
  char filename[MAX_FILENAME_LENGTH];
  struct _aImageCacheNode_t* next;
} aImageCacheNode_t;

typedef struct
{
  aImageCacheNode_t* head;
} aImageCache_t;

typedef struct
{
  unsigned int currentTime; //Delta time
  unsigned int lastTime;

  unsigned int frameStart; //Frame time to calculate fps
  unsigned int frameTime; //Length of each frame
  unsigned int lastFrameCounterClear; //This keeps count of how long it has been since the last time frames was set to 0
  int frames; //actual count of how many frames happen over one second
} aDeltaTime_t;

typedef struct
{
  void (*logic)( float elapsedTime );
  void (*draw)( float elapsedTime );
  void (*onExit)( void );
} aDelegate_t;

typedef struct
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_AudioDeviceID deviceID;
  aDelegate_t delegate;
  aDeltaTime_t time;
  aColor_t background;
  aImageCache_t* img_cache;
  int keyboard[MAX_KEYBOARD_KEYS];
  aWidget_t* active_widget;
  double font_scale;
  int font_type;
  SDL_Rect glyphs[FONT_MAX][MAX_GLYPHS];
  TTF_Font* fonts[FONT_MAX];
  SDL_Texture* font_textures[FONT_MAX];
  aMouse_t mouse;
  int running;
  char input_text[MAX_INPUT_LENGTH];
  int last_key_pressed;
} aApp_t;

typedef struct
{
  char filename[MAX_FILENAME_LENGTH];
  SDL_AudioSpec spec;
  uint32_t length;
  uint8_t* buffer;
} aAudioClip_t;

typedef struct _textures{
  char name[MAX_FILENAME_LENGTH];
  SDL_Texture* texture;
  struct _textures* next;
} aTexture_t;

/*
---------------------------------------------------------------
---                          Audio                          ---
---------------------------------------------------------------
*/

int a_InitAudio( void );
void a_LoadSounds( const char *filename, aAudioClip_t *clip );
void a_PlaySoundEffect( aAudioClip_t *clip );

/*
---------------------------------------------------------------
---                      Delta Time                         ---
---------------------------------------------------------------
*/

float a_GetDeltaTime( void );

/**
 * @defgroup drawing Drawing System
 * @brief 2D drawing and rendering functions for the Archimedes engine
 * 
 * The drawing system provides comprehensive 2D graphics capabilities including:
 * - Scene management and render cycles
 * - Primitive drawing (points, lines, shapes)
 * - Filled and outlined shapes
 * - Surface and texture blitting
 * - Color management with predefined constants
 * 
 * All drawing functions use SDL2 as the underlying graphics backend and follow
 * a consistent pattern of temporarily setting render colors and restoring them.
 * 
 * @{
 */

/**
 * @brief Prepare the scene for rendering
 * 
 * Initializes the render target and clears the screen with the current background color.
 * This function must be called at the beginning of each frame before any drawing operations.
 * 
 * @note Sets the render draw color to white (255,255,255,255) for subsequent operations
 * @note Must be paired with a_PresentScene() to display the rendered frame
 * @see a_PresentScene()
 * @see app.background
 */
void a_PrepareScene( void );

/**
 * @brief Present the rendered scene to the screen
 * 
 * Displays all drawing operations performed since a_PrepareScene() was called.
 * This completes the render cycle and should be called once per frame after all
 * drawing operations are complete.
 * 
 * @note Uses double-buffered rendering: swaps front and back buffers
 * @note Call this after all drawing operations in a frame
 * @see a_PrepareScene()
 */
void a_PresentScene( void );
/**
 * @brief Draw a single point at specified coordinates
 * 
 * Renders a single pixel at the given screen coordinates using the specified color.
 * The point is clipped automatically if coordinates fall outside screen bounds.
 * 
 * @param x Horizontal position in pixels
 * @param y Vertical position in pixels
 * @param color RGBA color structure for the point
 * 
 * @note Point is drawn as a single pixel
 * @note Coordinates outside screen bounds are clipped by SDL
 * @note Temporarily changes render color, restores to white afterward
 * @see aColor_t
 */
void a_DrawPoint( const int x, const int y, const aColor_t color );
/**
 * @brief Draw a line between two points
 * 
 * Renders a straight line from the starting point to the ending point using
 * the specified color. Uses SDL's line drawing algorithm for optimal performance.
 * 
 * @param x1 Starting X coordinate
 * @param y1 Starting Y coordinate
 * @param x2 Ending X coordinate
 * @param y2 Ending Y coordinate
 * @param color RGBA color structure for the line
 * 
 * @note Handles lines at any angle including vertical and horizontal
 * @note Uses Bresenham's line algorithm internally via SDL
 * @note Temporarily changes render color, restores to white afterward
 * @see a_DrawHorizontalLine()
 * @see a_DrawVerticalLine()
 */
void a_DrawLine( const int x1, const int y1, const int x2, const int y2, const aColor_t color );
/**
 * @brief Draw a horizontal line
 * 
 * Renders a horizontal line between two X coordinates at a fixed Y position.
 * This is optimized for drawing horizontal lines and handles coordinate ordering automatically.
 * 
 * @param x1 Starting X coordinate
 * @param x2 Ending X coordinate
 * @param y Y coordinate (same for both endpoints)
 * @param color RGBA color structure for the line
 * 
 * @note Optimized for horizontal lines
 * @note Order of x1 and x2 doesn't matter (automatically handles swapping)
 * @note Temporarily changes render color, restores to white afterward
 * @see a_DrawLine()
 * @see a_DrawVerticalLine()
 */
void a_DrawHorizontalLine( const int x1, const int x2, const int y, const aColor_t color );
/**
 * @brief Draw a vertical line
 * 
 * Renders a vertical line between two Y coordinates at a fixed X position.
 * This is optimized for drawing vertical lines and handles coordinate ordering automatically.
 * 
 * @param y1 Starting Y coordinate
 * @param y2 Ending Y coordinate
 * @param x X coordinate (same for both endpoints)
 * @param color RGBA color structure for the line
 * 
 * @note Optimized for vertical lines
 * @note Order of y1 and y2 doesn't matter (automatically handles swapping)
 * @note Temporarily changes render color, restores to white afterward
 * @see a_DrawLine()
 * @see a_DrawHorizontalLine()
 */
void a_DrawVerticalLine( const int y1, const int y2, const int x, const aColor_t color );
/**
 * @brief Draw a circle outline
 * 
 * Renders the outline of a circle at the specified center position with the given radius.
 * Uses Bresenham's circle algorithm for efficient pixel-perfect circle drawing.
 * 
 * @param posX Center X coordinate
 * @param posY Center Y coordinate
 * @param radius Circle radius in pixels
 * @param color RGBA color structure for the circle outline
 * 
 * @note Draws only the perimeter, not filled
 * @note Uses Bresenham's circle algorithm for efficiency
 * @note Radius of 0 draws a single point at center
 * @note Negative radius is treated as 0
 * @see a_DrawFilledCircle()
 */
void a_DrawCircle( const int posX, const int posY, const int radius, const aColor_t color );
/**
 * @brief Draw a filled circle
 * 
 * Renders a completely filled circle at the specified center position with the given radius.
 * Uses an optimized scan-line filling algorithm for smooth, solid circles.
 * 
 * @param posX Center X coordinate
 * @param posY Center Y coordinate
 * @param radius Circle radius in pixels
 * @param color RGBA color structure for the filled circle
 * 
 * @note Fills the entire circle area with color
 * @note Uses optimized scan-line filling algorithm
 * @note Radius of 0 draws a single point at center
 * @note Negative radius is treated as 0
 * @see a_DrawCircle()
 */
void a_DrawFilledCircle( const int posX, const int posY, const int radius, const aColor_t color );
/**
 * @brief Draw a triangle outline
 * 
 * Renders the outline of a triangle by drawing three lines connecting the specified vertices.
 * The triangle can be specified with vertices in any order (clockwise or counter-clockwise).
 * 
 * @param x0 First vertex X coordinate
 * @param y0 First vertex Y coordinate
 * @param x1 Second vertex X coordinate
 * @param y1 Second vertex Y coordinate
 * @param x2 Third vertex X coordinate
 * @param y2 Third vertex Y coordinate
 * @param color RGBA color structure for the triangle outline
 * 
 * @note Draws three lines connecting the vertices
 * @note Vertices can be specified in any order (clockwise or counter-clockwise)
 * @note Degenerate triangles (collinear points) draw as lines
 * @see a_DrawFilledTriangle()
 */
void a_DrawTriangle( const int x0, const int y0, const int x1, const int y1, const int x2,
                     const int y2, const aColor_t color );
/**
 * @brief Draw a filled triangle
 * 
 * Renders a completely filled triangle using the three specified vertices.
 * The implementation uses barycentric coordinate filling for accurate results.
 * 
 * @param x0 First vertex X coordinate
 * @param y0 First vertex Y coordinate
 * @param x1 Second vertex X coordinate
 * @param y1 Second vertex Y coordinate
 * @param x2 Third vertex X coordinate
 * @param y2 Third vertex Y coordinate
 * @param color RGBA color structure for the filled triangle
 * 
 * @note Fills the entire triangle area with color
 * @note Currently implementation is disabled (commented out)
 * @note Would use barycentric coordinate filling when implemented
 * @note Degenerate triangles are handled gracefully
 * @see a_DrawTriangle()
 */
void a_DrawFilledTriangle( const int x0, const int y0, const int x1, const int y1,
                           const int x2, const int y2, const aColor_t color );
/**
 * @brief Draw a rectangle outline
 * 
 * Renders the outline of a rectangle at the specified position and dimensions.
 * Uses individual RGBA color components instead of a color structure.
 * 
 * @param x Top-left X coordinate
 * @param y Top-left Y coordinate
 * @param w Width in pixels
 * @param h Height in pixels
 * @param r Red color component (0-255)
 * @param g Green color component (0-255)
 * @param b Blue color component (0-255)
 * @param a Alpha transparency (0-255, 255=opaque)
 * 
 * @note Draws only the border, not filled
 * @note Zero or negative dimensions are handled gracefully
 * @note Temporarily changes render color, restores to white afterward
 * @see a_DrawFilledRect()
 */
void a_DrawRect( const aRect_t rect, const aColor_t color );
/**
 * @brief Draw a filled rectangle
 * 
 * Renders a completely filled rectangle at the specified position and dimensions.
 * Uses individual RGBA color components and supports alpha blending.
 * 
 * @param x Top-left X coordinate
 * @param y Top-left Y coordinate
 * @param w Width in pixels
 * @param h Height in pixels
 * @param r Red color component (0-255)
 * @param g Green color component (0-255)
 * @param b Blue color component (0-255)
 * @param a Alpha transparency (0-255, 255=opaque)
 * 
 * @note Fills the entire rectangle area with color
 * @note Zero or negative dimensions are handled gracefully
 * @note Supports alpha blending when alpha < 255
 * @note Temporarily changes render color, restores to white afterward
 * @see a_DrawRect()
 */
void a_DrawFilledRect( const aRect_t rect, const aColor_t color );

/**
 * @brief Blit a surface to the screen at specified position
 * 
 * Converts an SDL surface to a texture and renders it at the specified coordinates.
 * The surface dimensions determine the size of the rendered image.
 * 
 * @param surf Source SDL surface to draw
 * @param x Destination X coordinate
 * @param y Destination Y coordinate
 * 
 * @note Converts surface to texture and renders it
 * @note Surface dimensions determine blit size
 * @note Creates and destroys texture each call (not optimized for repeated use)
 * @note NULL surface is handled gracefully without crashing
 * @note Surface remains unchanged after blitting
 * @see a_BlitSurfRect()
 * @see a_BlitTextureRect()
 */
void a_Blit( SDL_Surface* surf, const int x, const int y );

/**
 * @brief Blit a rectangular region of a surface to the screen
 * 
 * Converts a portion of an SDL surface to a texture and renders it with scaling.
 * Only the specified rectangular region from the source is drawn.
 * 
 * @param surf Source SDL surface to draw from
 * @param src Source rectangle within the surface
 * @param x Destination X coordinate
 * @param y Destination Y coordinate
 * @param scale Scaling factor for the destination size
 * 
 * @note Blits only the specified rectangular region from source
 * @note Destination size is source rectangle dimensions multiplied by scale
 * @note Frees the source surface after creating texture
 * @note NULL surface is handled gracefully without crashing
 * @note Source rectangle is clipped to surface bounds
 * @see a_Blit()
 * @see a_BlitTextureRect()
 */
void a_BlitSurfRect( SDL_Surface* surf, SDL_Rect src, const int x, const int y,
                     const int scale );
void a_BlitTextureRect( SDL_Texture* texture, SDL_Rect src, const int x,
                        const int y, const int scale, const aColor_t color );

/*
 * Update the window title text
 *
 * `title` - New title string for the window
 *
 * -- Changes the text displayed in the window's title bar
 * -- String is copied internally, original can be freed after call
 * -- Empty string results in blank title
 * -- NULL title is handled gracefully (implementation dependent)
 * -- Works on all supported platforms (Windows, Linux, macOS)
 */
void a_UpdateTitle( const char *title );

/*
---------------------------------------------------------------
---                          Image                          ---
---------------------------------------------------------------
*/

int a_InitImage( void );
SDL_Surface* a_Image( const char *filename );
int a_CleanUpImageCache( aImageCache_t* head );
int a_Screenshot( SDL_Renderer *renderer, const char *filename );

/*
---------------------------------------------------------------
---                       Initialize                        ---
---------------------------------------------------------------
*/

extern aApp_t app;

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
 * - Window and renderer creation with specified dimensions
 * - Mouse and timing state initialization
 * - Image cache setup
 * - Audio, image, texture, and font system initialization
 * - Random number generator seeding
 *
 * This function must be called before any other Archimedes functions.
 * The global `app` structure will be populated with initialized values.
 *
 * @param width Window width in pixels (must be > 0)
 * @param height Window height in pixels (must be > 0)
 * @param title Window title string (cannot be NULL)
 * @return 0 on success, negative value on failure (-1: SDL init, -2: IMG init, -3: TTF init, -4: window creation)
 */
int a_Init( const int width, const int height, const char *title );
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
 * - Cleaning up the image cache and freeing memory
 * - Shutting down TTF, IMG, and SDL subsystems
 * - Resetting the app running state
 *
 * This function is safe to call multiple times and will handle NULL pointers gracefully.
 * After calling this function, a_Init() must be called again before using any other
 * Archimedes functions.
 */
void a_Quit( void );


/*
---------------------------------------------------------------
---                          Input                          ---
---------------------------------------------------------------
*/

/**
 * @brief Processes all pending SDL events and dispatches them to appropriate handlers.
 *
 * This function continuously polls for SDL events (keyboard, mouse, quit, text input)
 * using `SDL_PollEvent` and dispatches them to internal static functions (`a_DoKeyDown`,
 * `a_DoKeyUp`, `a_DoMouseDown`, `a_DoMouseUp`, `a_DoMouseWheel`, `a_DoMouseMotion`).
 * It updates the global `app` state based on these events, managing the game's input.
 * Unknown event types are silently ignored.
 *
 * @note The `app.running` flag is set to `0` if an `SDL_QUIT` event is received.
 * @note For `SDL_TEXTINPUT` events, the input text is copied to `app.input_text`
 * if its length is valid (between 1 and `MAX_INPUT_LENGTH - 1`).
 *
 * @see a_DoKeyDown
 * @see a_DoKeyUp
 * @see a_DoMouseDown
 * @see a_DoMouseUp
 * @see a_DoMouseWheel
 * @see a_DoMouseMotion
 */
void a_DoInput( void );

/*
---------------------------------------------------------------
---                          Text                           ---
---------------------------------------------------------------
*/

enum
{
  TEXT_ALIGN_LEFT,
  TEXT_ALIGN_CENTER,
  TEXT_ALIGN_RIGHT
};

int a_GetWrappedTextHeight( const char* text, const int font_type,
                            const int max_width );

void a_CalcTextDimensions( const char* text, const int font_type,
                           int* w, int* h );

void a_DrawText( const char* text, const int x, const int y, 
                 const aColor_t bg, const aColor_t fg,
                 const int font_type, const int align, const int max_width );

SDL_Texture* a_GetTextTexture( const char* text, const int font_type );

void a_InitFonts( void );

/*
---------------------------------------------------------------
---                       Textures                          ---
---------------------------------------------------------------
*/

SDL_Texture* a_LoadTexture( const char* filename );
SDL_Texture* a_ToTexture( SDL_Surface* surf, int destroy );
void a_InitTextures( void );

/*
---------------------------------------------------------------
---                        Widgets                          ---
---------------------------------------------------------------
*/

enum
{
  WT_BUTTON,
  WT_SELECT,
  WT_SLIDER,
  WT_INPUT,
  WT_CONTROL,
  WT_CONTAINER,
};

/**
 * @brief Draws all visible widgets.
 *
 * This function iterates through the linked list of widgets, starting from `widget_head.next`,
 * and calls the appropriate drawing function for each widget based on its type.
 * Hidden widgets are skipped as they are not drawn.
 */
void a_DrawWidgets( void );

/**
 * @brief Handles the logic and interactions for all active widgets.
 *
 * This function is the main update loop for the widget system. It processes mouse clicks,
 * keyboard inputs (left, right, space, return), and manages the active widget state.
 * It also handles transitions to specific input or control modes if an appropriate
 * widget is activated.
 *
 * It checks for mouse clicks to activate widgets based on their coordinates.
 * For container widgets, it iterates through their components to find the clicked one.
 * Keyboard inputs are handled for navigating widgets (though up/down are commented out)
 * and triggering actions or entering specific widget interaction modes (input/control).
 */
void a_DoWidget( void );

/**
 * @brief Retrieves a widget by its name.
 *
 * This function searches through the linked list of widgets (starting from `widget_head.next`)
 * to find a widget with a matching name. If a widget with the given name is found,
 * a pointer to that widget is returned. If no widget is found, an SDL warning
 * message is logged, and `NULL` is returned.
 *
 * @param name The string name of the widget to retrieve.
 * @return A pointer to the `aWidget_t` if found, otherwise `NULL`.
 */
aWidget_t* a_GetWidget( const char* name );

/**
 * @brief Retrieves a container widget by its name and casts it to `aContainerWidget_t`.
 *
 * This function first calls `a_GetWidget` to locate a widget by the given name.
 * If the widget is found, it attempts to cast its `data` member to an `aContainerWidget_t*`.
 * Error messages are printed to stdout if the widget is not found or if the cast results in NULL.
 *
 * @param name The string name of the container widget to retrieve.
 * @return A pointer to the `aContainerWidget_t` if successful, otherwise `NULL`.
 */
aContainerWidget_t* a_GetContainerFromWidget( const char* name );

/**
 * @brief Initializes the widget system from a configuration file.
 *
 * This function sets up the initial state of the widget system.
 * It clears the widget head, sets the tail, loads widgets from the specified
 * filename, and initializes various global widget-related variables such as
 * `slider_delay`, `cursor_blink`, `handle_input_widget`, and `handle_control_widget`.
 *
 * @param filename The path to the file containing widget configuration data.
 */
void a_InitWidgets( const char* filename );
int a_ClearWidgetCache( aWidget_t* widget );

/*
---------------------------------------------------------------
---                         Colors                          ---
---------------------------------------------------------------
*/

extern aColor_t black;
extern aColor_t blue;
extern aColor_t green;
extern aColor_t cyan;
extern aColor_t red;
extern aColor_t magenta;
extern aColor_t yellow;
extern aColor_t white;
extern aColor_t shit0;
extern aColor_t shit1;
extern aColor_t shit2;
extern aColor_t shit3;

extern aColor_t gray9;
extern aColor_t gray8;
extern aColor_t gray7;
extern aColor_t gray6;
extern aColor_t gray5;
extern aColor_t gray4;
extern aColor_t gray3;
extern aColor_t gray2;
extern aColor_t gray1;
extern aColor_t gray0;

/*
---------------------------------------------------------------
---                         Errors                          ---
---------------------------------------------------------------
*/

typedef enum
{
  NORMAL,
  WARNING,
  FATAL,
  INFO,
  DEBUG,
  TRACE,
} aLogLevel_t;

extern char* log_level_strings[LOG_LEVEL_COUNT];

#endif
