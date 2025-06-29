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
  char error_msg[MAX_LINE_LENGTH];
  int  error_type;
} aError_t;

typedef struct _widget_t
{
  int type;
  char name[MAX_FILENAME_LENGTH];
  int x, y;
  int w, h;
  char label[MAX_FILENAME_LENGTH];
  int boxed;
  int hidden;
  int padding;
  int flex;
  uint8_t fg[4];
  uint8_t bg[4];
  struct _widget_t* next;
  struct _widget_t* prev;
  void (*action)( void );
  void (*data);
} aWidget_t;

typedef struct
{
  int x, y;
  int w, h;
  int spacing;
  int num_components;
  aWidget_t* components;
} aContainerWidget_t;

typedef struct
{
  int num_options;
  char** options;
  int x, y;
  int w, h;
  int value;
  char* text_name[MAX_NAME_LENGTH];
} aSelectWidget_t;

typedef struct
{
  int x, y;
  int w, h;
  int value;
  int step;
  int wait_on_change;
} aSliderWidget_t;

typedef struct
{
  int x, y;
  int w, h;
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
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
} aColor_t;

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

/*
---------------------------------------------------------------
---                          Draw                           ---
---------------------------------------------------------------
*/
/*
 * Prepare the scene for rendering
 *
 * -- Clears the screen with the current background color
 * -- Sets render draw color to white (255,255,255,255) for subsequent operations
 * -- Must be called before drawing operations in each frame
 * -- Call a_PresentScene() after drawing to display the rendered frame
 */
void a_PrepareScene( void );
/*
 * Present the rendered scene to the screen
 *
 * -- Displays all drawing operations performed since a_PrepareScene()
 * -- Completes the render cycle started by a_PrepareScene()
 * -- Should be called once per frame after all drawing operations
 * -- Double-buffered rendering: swaps front and back buffers
 */
void a_PresentScene( void );
/*
 * Draw a single point at specified coordinates
 *
 * `x` - Horizontal position in pixels
 * `y` - Vertical position in pixels
 * `color` - RGBA color for the point
 *
 * -- Point is drawn as a single pixel
 * -- Coordinates outside screen bounds are clipped
 * -- Temporarily changes render color, restores to white afterward
 */
void a_DrawPoint( const int x, const int y, const aColor_t color );
/*
 * Draw a line between two points
 *
 * `x1` - Starting X coordinate
 * `y1` - Starting Y coordinate
 * `x2` - Ending X coordinate
 * `y2` - Ending Y coordinate
 * `color` - RGBA color for the line
 *
 * -- Handles lines at any angle including vertical and horizontal
 * -- Temporarily changes render color, restores to white afterward
 */
void a_DrawLine( const int x1, const int y1, const int x2, const int y2, const aColor_t color );
/*
 * Draw a horizontal line
 *
 * `x1` - Starting X coordinate
 * `x2` - Ending X coordinate
 * `y` - Y coordinate (same for both endpoints)
 * `color` - RGBA color for the line
 *
 * -- Optimized for horizontal lines
 * -- Order of x1 and x2 doesn't matter (automatically handles swapping)
 * -- Temporarily changes render color, restores to white afterward
 */
void a_DrawHorizontalLine( const int x1, const int x2, const int y, const aColor_t color );
/*
 * Draw a circle outline
 *
 * `posX` - Center X coordinate
 * `posY` - Center Y coordinate
 * `radius` - Circle radius in pixels
 * `color` - RGBA color for the circle outline
 *
 * -- Draws only the perimeter, not filled
 * -- Uses Bresenham's circle algorithm for efficiency
 * -- Radius of 0 draws a single point at center
 * -- Negative radius is treated as 0
 */
void a_DrawVerticalLine( const int y1, const int y2, const int x, const aColor_t color );
/*
 * Draw a circle
 *
 * `posX` - Center X coordinate
 * `posY` - Center Y coordinate
 * `radius` - Circle radius in pixels
 * `color` - RGBA color for the circle
 *
 * -- Fills the entire circle area with color
 * -- Radius of 0 draws a single point at center
 * -- Negative radius is treated as 0
 */
void a_DrawCircle( const int posX, const int posY, const int radius, const aColor_t color );
/*
 * Draw a filled circle
 *
 * `posX` - Center X coordinate
 * `posY` - Center Y coordinate
 * `radius` - Circle radius in pixels
 * `color` - RGBA color for the filled circle
 *
 * -- Fills the entire circle area with color
 * -- Uses optimized scan-line filling algorithm
 * -- Radius of 0 draws a single point at center
 * -- Negative radius is treated as 0
 */
void a_DrawFilledCircle( const int posX, const int posY, const int radius, const aColor_t color );
/*
 * Draw a triangle outline
 *
 * `x0` - First vertex X coordinate
 * `y0` - First vertex Y coordinate
 * `x1` - Second vertex X coordinate
 * `y1` - Second vertex Y coordinate
 * `x2` - Third vertex X coordinate
 * `y2` - Third vertex Y coordinate
 * `color` - RGBA color for the triangle outline
 *
 * -- Draws three lines connecting the vertices
 * -- Vertices can be specified in any order (clockwise or counter-clockwise)
 * -- Degenerate triangles (collinear points) draw as lines
 */
void a_DrawTriangle( const int x0, const int y0, const int x1, const int y1, const int x2,
                     const int y2, const aColor_t color );
/*
 * Draw a filled triangle
 *
 * `x0` - First vertex X coordinate
 * `y0` - First vertex Y coordinate
 * `x1` - Second vertex X coordinate
 * `y1` - Second vertex Y coordinate
 * `x2` - Third vertex X coordinate
 * `y2` - Third vertex Y coordinate
 * `color` - RGBA color for the filled triangle
 *
 * -- Fills the entire triangle area with color
 * -- Currently implementation is disabled (commented out)
 * -- Would use barycentric coordinate filling when implemented
 * -- Degenerate triangles are handled gracefully
 */
void a_DrawFilledTriangle( const int x0, const int y0, const int x1, const int y1,
                           const int x2, const int y2, const aColor_t color );
/*
 * Draw a rectangle outline
 *
 * `x` - Top-left X coordinate
 * `y` - Top-left Y coordinate
 * `w` - Width in pixels
 * `h` - Height in pixels
 * `r` - Red color component (0-255)
 * `g` - Green color component (0-255)
 * `b` - Blue color component (0-255)
 * `a` - Alpha transparency (0-255, 255=opaque)
 *
 * -- Draws only the border, not filled
 * -- Zero or negative dimensions are handled gracefully
 * -- Temporarily changes render color, restores to white afterward
 */
void a_DrawRect( const int x, const int y, const int w, const int h, const int r,
                 const int g, const int b, const int a );
/*
 * Draw a filled rectangle
 *
 * `x` - Top-left X coordinate
 * `y` - Top-left Y coordinate
 * `w` - Width in pixels
 * `h` - Height in pixels
 * `r` - Red color component (0-255)
 * `g` - Green color component (0-255)
 * `b` - Blue color component (0-255)
 * `a` - Alpha transparency (0-255, 255=opaque)
 *
 * -- Fills the entire rectangle area with color
 * -- Zero or negative dimensions are handled gracefully
 * -- Supports alpha blending when alpha < 255
 * -- Temporarily changes render color, restores to white afterward
 */
void a_DrawFilledRect( const int x, const int y, const int w, const int h, const int r,
                       const int g, const int b, const int a );
/*
 * Blit a surface to the screen at specified position
 *
 * `surf` - Source SDL surface to draw
 * `x` - Destination X coordinate
 * `y` - Destination Y coordinate
 *
 * -- Converts surface to texture and renders it
 * -- Surface dimensions determine blit size
 * -- Creates and destroys texture each call (not optimized for repeated use)
 * -- NULL surface is handled gracefully without crashing
 * -- Surface remains unchanged after blitting
 */
void a_Blit( SDL_Surface* surf, const int x, const int y );
/*
 * Blit a rectangular region of a surface to the screen
 *
 * `surf` - Source SDL surface to draw from
 * `src` - Source rectangle within the surface
 * `x` - Destination X coordinate
 * `y` - Destination Y coordinate
 *
 * -- Blits only the specified rectangular region from source
 * -- Destination size matches source rectangle dimensions
 * -- Frees the source surface after creating texture
 * -- NULL surface is handled gracefully without crashing
 * -- Source rectangle is clipped to surface bounds
 */
void a_BlitRect( SDL_Surface* surf, SDL_Rect src, const int x, const int y );
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

int a_Init( const int width, const int height, const char *title );
void a_Quit( void );


/*
---------------------------------------------------------------
---                          Input                          ---
---------------------------------------------------------------
*/
/*
 * Process all pending SDL input events and update global input state
 *
 * -- Main input processing function that should be called once per frame
 * -- Polls all SDL events and dispatches them to appropriate handlers
 * -- Updates global app.keyboard[], app.mouse, and app.input_text state
 * -- Handles SDL_QUIT events by setting app.running = 0
 * -- Processes keyboard key down/up, mouse button/wheel, and text input events
 * -- Must be called after SDL initialization and before accessing input state
 * -- Thread-safe only if called from the same thread that initialized SDL
 * -- Returns early if SDL event polling fails (defensive programming)
 * -- Handles malformed or invalid events gracefully without crashing
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

int a_GetWrappedTextHeight( char* text, int font_type, int max_width );
void a_CalcTextDimensions( char* text, int font_type, int* w, int* h );
void a_DrawText( char* text, int x, int y, int r, int g, int b, int font_type, int align, int max_width );
SDL_Texture* a_GetTextTexture( char* text, int font_type );
void a_InitFonts( void );

/*
---------------------------------------------------------------
---                       Textures                          ---
---------------------------------------------------------------
*/

SDL_Texture* a_LoadTexture( char* filename );
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

void a_DrawWidgets( void );
void a_DoWidget( void );
aWidget_t* a_GetWidget( char* name );
aContainerWidget_t* a_GetContainerFromWidget( char* name );
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
