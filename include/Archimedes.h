/*
 *   Archimedes is a lightweight, cross-platform C game engine
 * built on SDL2 that provides essential 2D graphics, audio,
 * input handling, and UI widget functionality.
 *
 * Copyright (c) 2025 Jacob Kellum <jkellum819@gmail.com>
 *                    Mathew Storm <smattymat@gmail.com>
 ************************************************************************
 */

#ifndef __ARCHIMEDES_H__
#define __ARCHIMEDES_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

/*
---------------------------------------------------------------
---                         Defines                         ---
---------------------------------------------------------------
*/

#define FPS_CAP 60.0
#define LOGIC_RATE ( FPS_CAP / 1000 )

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

#define MAGIC_NUMBER "CAFEBABE"
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

enum
{
  AUF_Invalid,
  AUF_False,
  AUF_True,
  AUF_NULL,
  AUF_Number,
  AUF_String,
  AUF_Array,
  AUF_Object,
  AUF_Raw
};

/*
---------------------------------------------------------------
---                         Structs                         ---
---------------------------------------------------------------
*/

typedef struct
{
  float x, y;
  float w, h;
} aRectf_t;

typedef struct
{
  int x, y;
  int w, h;
} aRecti_t;

typedef struct
{
  float x, y;
} aPoint2f_t;

typedef struct
{
  int x, y;
} aPoint2i_t;

typedef struct
{
  float x, y, z;
} aPoint3f_t;

typedef struct
{
  int x, y, z;
} aPoint3i_t;

typedef struct
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} aColor_t;

typedef struct
{
  uint32_t start_ticks;
  uint32_t paused_ticks;
  uint8_t paused;
  uint8_t started;
} aTimer_t;

typedef struct
{
  char error_msg[MAX_LINE_LENGTH];
  int  error_type;
} aError_t;

typedef struct _widget_t
{
  int type;
  char name[MAX_FILENAME_LENGTH];
  aRectf_t rect;
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
  aRectf_t rect;
  int spacing;
  int num_components;
  aWidget_t* components;
} aContainerWidget_t;

typedef struct
{
  int num_options;
  char** options;
  aRectf_t rect;
  int value;
  char* text_name[MAX_NAME_LENGTH];
} aSelectWidget_t;

typedef struct
{
  aRectf_t rect;
  int value;
  int step;
  int wait_on_change;
} aSliderWidget_t;

typedef struct
{
  aRectf_t rect;
  int max_length;
  char* text;
} aInputWidget_t;

typedef struct
{
  int x, y;
  int value;
} aControlWidget_t;

typedef struct {
  char magic_number[8];
  uint8_t version;
  uint16_t num_widgets;
  char filename[MAX_FILENAME_LENGTH];
} aWidgetFileHeader_t;

typedef struct _aAUF_Node_t {
  struct _aAUF_Node_t* next;
  struct _aAUF_Node_t* prev;

  struct _aAUF_Node_t* child;

  int type;

  char* value_string;
  int value_int;
  double value_double;
  char* string;

} aAUF_Node_t;

typedef struct _aAUF_t
{
  aAUF_Node_t* head;
  aAUF_Node_t* tail;
  int size;

} aAUF_t;

typedef struct
{
  uint8_t frame_cap;
} aOptions_t;

typedef struct
{
  int x;
  int y;
  uint8_t pressed;
  uint8_t button;
  uint8_t state;
  uint8_t clicks;
  uint8_t motion;
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
  uint32_t current_time; //Delta time
  uint32_t last_time;

  int frames;
  aTimer_t* FPS_timer;
  float avg_FPS;
} aDeltaTime_t;

typedef struct
{
  void (*logic)( float delta_time );
  void (*draw)( float delta_time );
  void (*onExit)( void );
} aDelegate_t;

typedef struct
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_AudioDeviceID deviceID;
  aDelegate_t delegate;
  aOptions_t options;
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
  aRectf_t g_viewport;
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
void a_GetFPS( void );

/*
---------------------------------------------------------------
---                        Draw                             ---
---------------------------------------------------------------
*/

/**
 * @brief Prepare the scene for rendering
 * 
 * Initializes the render target and clears the screen with the current background color.
 * This function must be called at the beginning of each frame before any drawing operations.
 * 
 * @note Sets the render draw color to white (255,255,255,255) for subsequent operations
 * @note Must be paired with a_PresentScene() to display the rendered frame
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
 */
void a_PresentScene( void );

/**
 * @brief Draw a single point at specified coordinates
 * 
 * Renders a single pixel at the given screen coordinates using the specified color.
 * 
 * @param x Horizontal position in pixels
 * @param y Vertical position in pixels
 * @param color RGBA color structure for the point
 * 
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
 */
void a_DrawLine( const int x1, const int y1, const int x2, const int y2, const aColor_t color );

/**
 * @brief Draw a horizontal line
 * 
 * Renders a horizontal line between two X coordinates at a fixed Y position.
 * 
 * @param x1 Starting X coordinate
 * @param x2 Ending X coordinate
 * @param y Y coordinate (same for both endpoints)
 * @param color RGBA color structure for the line
 * 
 * @note Optimized for horizontal lines
 * @note Order of x1 and x2 doesn't matter (automatically handles swapping)
 */
void a_DrawHorizontalLine( const int x1, const int x2, const int y, const aColor_t color );

/**
 * @brief Draw a vertical line
 * 
 * Renders a vertical line between two Y coordinates at a fixed X position.
 * 
 * @param y1 Starting Y coordinate
 * @param y2 Ending Y coordinate
 * @param x X coordinate (same for both endpoints)
 * @param color RGBA color structure for the line
 * 
 * @note Order of y1 and y2 doesn't matter (automatically handles swapping)
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
 */
void a_DrawFilledCircle( const int posX, const int posY, const int radius, const aColor_t color );

/**
 * @brief Draw a triangle outline
 * 
 * Renders the outline of a triangle by drawing three lines connecting the specified vertices.
 * 
 * @param x0 First vertex X coordinate
 * @param y0 First vertex Y coordinate
 * @param x1 Second vertex X coordinate
 * @param y1 Second vertex Y coordinate
 * @param x2 Third vertex X coordinate
 * @param y2 Third vertex Y coordinate
 * @param color RGBA color structure for the triangle outline
 * 
 * @note Vertices can be specified in any order (clockwise or counter-clockwise)
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
 * @note Currently implementation is disabled
 */
void a_DrawFilledTriangle( const int x0, const int y0, const int x1, const int y1,
                           const int x2, const int y2, const aColor_t color );

/**
 * @brief Draw a rectangle outline
 * 
 * Renders the outline of a rectangle at the specified position and dimensions.
 * 
 * @param rect the rectangle to be drawn
 * @param color (RGBA)
 * 
 */
void a_DrawRect( const aRectf_t rect, const aColor_t color );

/**
 * @brief Draw a filled rectangle
 * 
 * Renders a completely filled rectangle at the specified position and dimensions.
 * 
 * @param rect the rectangle to be drawn
 * @param color (RGBA)
 * 
 */
void a_DrawFilledRect( const aRectf_t rect, const aColor_t color );

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
 * @note NULL surface is handled gracefully without crashing
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
 * @note NULL surface is handled gracefully without crashing
 */
void a_BlitSurfRect( SDL_Surface* surf, SDL_Rect src, const int x, const int y,
                     const int scale );
void a_BlitTextureRect( SDL_Texture* texture, SDL_Rect src, const int x,
                        const int y, const int scale, const aColor_t color );

/**
 * @brief Update the window title text
 *
 * Changes the text displayed in the window's title bar
 * 
 * @param title Source of new Title
 * 
 * @note String is copied internally, original can be freed after call
 */
void a_UpdateTitle( const char *title );

void a_SetPixel( SDL_Surface *surface, int x, int y, aColor_t c );

/*
---------------------------------------------------------------
---                          Image                          ---
---------------------------------------------------------------
*/

int a_InitImage( void );
SDL_Surface* a_Image( const char *filename );
int a_CleanUpImageCache( void );
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

/**
 * @brief Calculates the height of wrapped text within a maximum width.
 *
 * This function computes how tall a text block will be when word-wrapped
 * to fit within the specified maximum width. Useful for determining the
 * space needed for multi-line text in UI elements.
 *
 * @param text The text string to measure (UTF-8 encoded)
 * @param font_type The font type to use for measurement
 * @param max_width Maximum width in pixels before wrapping
 * @return Total height in pixels of the wrapped text
 */
int a_GetWrappedTextHeight( const char* text, const int font_type,
                            const int max_width );

/**
 * @brief Calculates the pixel dimensions of a text string.
 *
 * This function computes the width and height of a text string when rendered
 * with the specified font type. It takes into account the current font scale
 * and handles UTF-8 encoded text properly.
 *
 * @param text The text string to measure (UTF-8 encoded)
 * @param font_type The font type to use (FONT_ENTER_COMMAND, FONT_LINUX, etc.)
 * @param w Pointer to store the calculated width in pixels
 * @param h Pointer to store the calculated height in pixels
 */
void a_CalcTextDimensions( const char* text, const int font_type,
                           float* w, float* h );

/**
 * @brief Draws text at the specified position with color and alignment.
 *
 * This function renders text to the screen using the glyph atlas system for
 * efficient drawing. It supports text alignment, color tinting, and optional
 * word wrapping. The text is rendered using pre-cached glyphs from the font
 * texture atlas.
 *
 * @param text The text string to draw (UTF-8 encoded)
 * @param x X coordinate for text position
 * @param y Y coordinate for text position
 * @param bg background color
 * @param fg foreground color
 * @param font_type The font type to use (FONT_ENTER_COMMAND, FONT_LINUX, etc.)
 * @param align Text alignment (TEXT_ALIGN_LEFT, TEXT_ALIGN_CENTER, TEXT_ALIGN_RIGHT)
 * @param max_width Maximum width for text wrapping (0 = no wrapping)
 */
void a_DrawText( const char* text, const int x, const int y, 
                 const aColor_t bg, const aColor_t fg,
                 const int font_type, const int align, const int max_width );

/**
 * @brief Creates a texture from a text string using TTF rendering.
 *
 * This function renders text using the SDL_ttf library directly, creating
 * a texture that can be used for custom text rendering. The text is
 * rendered with white color using blended (anti-aliased) mode.
 *
 * @param text The text string to render (UTF-8 encoded)
 * @param font_type The font type to use for rendering
 * @return SDL_Texture pointer containing the rendered text, or NULL on failure
 */
SDL_Texture* a_GetTextTexture( const char* text, const int font_type );

/**
 * @brief Initializes the font system with TTF fonts.
 *
 * This function loads the required TTF fonts and creates glyph atlases for
 * efficient text rendering. 
 *
 * The function expects font files at:
 * - resources/fonts/EnterCommand.ttf (48pt)
 * - resources/fonts/JetBrains.ttf (32pt)
 * - resources/fonts/CodePage437.png (32pt)
 * - resources/fonts/font.png (32pt)
 *
 * If font loading fails, the program will exit with an error message.
 */
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
  WT_UNKNOWN,
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
 * filename.
 *
 * @param filename The path to the file containing widget configuration data.
 */
void a_InitWidgets( const char* filename );
int a_FreeWidgetCache( void );

/*
---------------------------------------------------------------
---                      Widget Parser                      ---
---------------------------------------------------------------
*/

aAUF_t* a_AUFParser( const char* filename );
int a_SaveAUF( aWidget_t* widget_head, const char* filename );
int a_FreeLine( char** line, const int nl_count );

aAUF_t* a_AUFCreation( void );
aAUF_Node_t* a_AUFNodeCreation( void );
int a_AUFAddNode( aAUF_t* root, aAUF_Node_t* node );
int a_AUFNodeAddNode( aAUF_Node_t* root, aAUF_Node_t* node );
int a_AUFNodeAddChild( aAUF_Node_t* root, aAUF_Node_t* node );
int a_AUFNodeFree( aAUF_Node_t* head );
int a_AUFFree( aAUF_t* root );
aAUF_Node_t* a_AUFGetObjectItem( aAUF_Node_t* node, char* object_str );
void a_PrintAUFTree( aAUF_Node_t* node, int depth );

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

/*
---------------------------------------------------------------
---                         Utils                           ---
---------------------------------------------------------------
*/

char* a_STR_NDUP( const char* str, size_t len );
char* a_ParseString( char delimiter, char* str, int str_len );
char* a_ReadFile( const char* filename, int* file_size );
char* a_ParseStringDoubleDelim( char delimiter1, char delimiter2, char* str,
                                int str_len );
int a_CountNewLines( const char* file_string, const int file_size );
char** a_ParseLinesInFile( const char* file_string, const int file_size,
                           const int nl_count );

/*
---------------------------------------------------------------
---                      Viewport                           ---
---------------------------------------------------------------
*/


aPoint2f_t a_CalculateScaleOfViewport( void );
uint8_t a_IsRectVisibleInViewport( aRectf_t rect );

void a_DrawPointToViewport( aPoint3f_t p, aColor_t color );
void a_DrawRectToViewport( aRectf_t rect, aColor_t color );

/*
---------------------------------------------------------------
---                         Defines                         ---
---------------------------------------------------------------
*/

aTimer_t* a_CreateTimer( void );
void a_FreeTimer( aTimer_t* timer );
void a_StartTimer( aTimer_t* timer );
void a_StopTimer( aTimer_t* timer );
void a_PauseTimer( aTimer_t* timer );
void a_UnpauseTimer( aTimer_t* timer );
uint32_t a_GetTicks( aTimer_t* timer );
uint8_t a_IsStarted( aTimer_t* timer );
uint8_t a_IsPaused( aTimer_t* timer );

/*
---------------------------------------------------------------
---                     Input Enums                         ---
---------------------------------------------------------------
*/

enum
{
  //From USB HID Usage Tables
  //Keyboard/Keypad Page (0x07) and SDL2
  A_UNKOWN = 0,
  A_A = SDL_SCANCODE_A,
  A_B = 5,
  A_C = 6,
  A_D = 7,
  A_E = 8,
  A_F = 9,
  A_G = 10,
  A_H = 11,
  A_I = 12,
  A_J = 13,
  A_K = 14,
  A_L = 15,
  A_M = 16,
  A_N = 17,
  A_O = 18,
  A_P = 19,
  A_Q = 20,
  A_R = 21,
  A_S = 22,
  A_T = 23,
  A_U = 24,
  A_V = 25,
  A_W = 26,
  A_Y = 27,
  A_X = 28,
  A_Z = 29,
  A_1 = 30,
  A_2 = 31,
  A_3 = 32,
  A_4 = 33,
  A_5 = 34,
  A_6 = 35,
  A_7 = 36,
  A_8 = 37,
  A_9 = 38,
  A_0 = 39,
  A_RETURN = 40,
  A_ESCAPE = 41,
  A_DELETE = 42,
  A_TAB = 43,
  A_SPACEBAR = 44,
  A_MINUS = 45,
  A_EQUALS = 46,
  A_LEFTBRACKET = 47,
  A_RIGHTBRACKET = 48,
  A_BACKSLASH = 49,
  A_NONUSHASH = 50,
  A_SEMICOLON = 51,
  A_APOSTROPHE = 52,
  A_GRAVE = 53,
  A_COMMA = 54,
  A_PERIOD = 55,
  A_FORWARDSLASH = 56,
  A_CAPSLOCK = 57,
  A_F1 = 58,
  A_F2 = 59,
  A_F3 = 60,
  A_F4 = 61,
  A_F5 = 62,
  A_F6 = 63,
  A_F7 = 64,
  A_F8 = 65,
  A_F9 = 66,
  A_F10 = 67,
  A_F11 = 68,
  A_F12 = 69,
  A_PRINTSCREEN = 70,
  A_SCROLLLOCK = 71,
  A_PAUSE = 72,
  A_INSERT = 73,
  A_HOME = 74,
  A_PAGEUP = 75,
  A_DELETEFORWARD = 76,
  A_END = 77,
  A_PAGEDOWN = 78,
  A_RIGHT = 79,
  A_LEFT = 80,
  A_DOWN = 81,
  A_UP = 82,
  A_KP_NUMLOCK = 83,
  A_KP_DIVIDE = 84,
  A_KP_MULTIPLY = 85,
  A_KP_MINUS = 86,
  A_KP_PLUS = 87,
  A_KP_ENTER = 88,
  A_KP_1 = 89,
  A_KP_2 = 90,
  A_KP_3 = 91,
  A_KP_4 = 92,
  A_KP_5 = 93,
  A_KP_6 = 94,
  A_KP_7 = 95,
  A_KP_8 = 96,
  A_KP_9 = 97,
  A_KP_0 = 98,
  A_KP_PERIOD = 99,
  A_NONUSBACKSLASH = 100,
  A_APPLICATION = 101,
  A_POWER = 102,
  A_KP_EQUALS = 103,
  A_F13 = 104,
  A_F14 = 105,
  A_F15 = 106,
  A_F16 = 107,
  A_F17 = 108,
  A_F18 = 109,
  A_F19 = 110,
  A_F20 = 111,
  A_F21 = 112,
  A_F22 = 113,
  A_F23 = 114,
  A_F24 = 115,
  A_EXECUTE = 116,
  A_HELP = 117,
  A_MENU = 118,
  A_SELECT = 119,
  A_STOP = 120,
  A_AGAIN = 121,
  A_UNDO = 122,
  A_CUT = 123,
  A_COPY = 124,
  A_PASTE = 125,
  A_FIND = 126,
  A_MUTE = 127,
  A_VOLUMEUP = 128,
  A_VOLUMEDOWN = 129,
  A_LOCKINGCAPSLOCK = 130,
  A_LOCKINGNUMLOCK = 131,
  A_LOCKINGSCROLLLOCK = 132,
  A_KP_COMMA = 133,
  A_KP_EQUALSSIGN = 134,
  A_INTERNATIONAL1 = 135,
  A_INTERNATIONAL2 = 136,
  A_INTERNATIONAL3 = 137,
  A_INTERNATIONAL4 = 138,
  A_INTERNATIONAL5 = 139,
  A_INTERNATIONAL6 = 140,
  A_INTERNATIONAL7 = 141,
  A_INTERNATIONAL8 = 142,
  A_INTERNATIONAL9 = 143,
  A_LANG1 = 144,
  A_LANG2 = 145,
  A_LANG3 = 146,
  A_LANG4 = 147,
  A_LANG5 = 148,
  A_LANG6 = 149,
  A_LANG7 = 150,
  A_LANG8 = 151,
  A_LANG9 = 152,
  A_ALTERASE = 153,
  A_SYSREQ = 154,
  A_CANCEL = 155,
  A_CLEAR = 156,
  A_PRIOR = 157,
  A_RETURN2 = 158,
  A_SEPARATOR = 159,
  A_OUT = 160,
  A_OPER = 161,
  A_CLEARAGAIN = 162,
  A_CRSEL = 163,
  A_EXSEL = 164,
  A_KP_00 = 176,
  A_KP_000 = 177,
  A_THOUSANDSSPEARATOR = 178,
  A_DECIMALSPEARATOR = 179,
  A_CURRENCYUNIT = 180,
  A_CURRENCYSUBUNIT = 181,
  A_KP_LEFTPAREN = 182,
  A_KP_RIGHTPAREN = 183,
  A_KP_LEFTBRACE = 184,
  A_KP_RIGHTBRACE = 185,
  A_KP_TAB = 186,
  A_KP_BACKSPACE = 187,
  A_KP_A = 188,
  A_KP_B = 189,
  A_KP_C = 190,
  A_KP_D = 191,
  A_KP_E = 192,
  A_KP_F = 193,
  A_KP_XOR = 194,
  A_KP_POWER = 195,
  A_KP_PRECENT = 196,
  A_KP_LESSTHAN = 197,
  A_KP_GREATERTHAN = 198,
  A_KP_AMPERSAND = 199,
  A_KP_DBLAPERSAND = 200,
  A_KP_VERTBAR = 201,
  A_KP_DBLVERTBAR = 202,
  A_KP_COLON = 203,
  A_KP_HASH = 204,
  A_KP_SPACE = 205,
  A_KP_AT = 206,
  A_KP_EXCLAM = 207,
  A_KP_MEMSTORE = 208,
  A_KP_MEMRECALL = 209,
  A_KP_MEMCLEAR = 210,
  A_KP_MEMADD = 211,
  A_KP_MEMSUB = 212,
  A_KP_MEMMULTIPLY = 213,
  A_KP_MEMDIVIDE = 214,
  A_KP_PLUSMINUS = 215,
  A_KP_CLEAR = 216,
  A_KP_CLEARENTRY = 217,
  A_KP_BINARY = 218,
  A_KP_OCTAL = 219,
  A_KP_DECIMAL = 220,
  A_KP_HEXADECIMAL = 221,
  A_LCTRL = 224,
  A_LSHIFT = 225,
  A_LALT = 226,
  A_LGUI = 227,
  A_RCTRL = 228,
  A_RSHIFT = 229,
  A_RALT = 230,
  A_RGUI = 231,
  A_KEYS = 512
};

#endif

