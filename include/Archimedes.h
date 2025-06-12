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

#define FPS 60

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


#ifndef __DAEDALUS_H__

#define MAX_LINE_LENGTH     1024
#define MAX_FILENAME_LENGTH 256
#define PI 3.14159265

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'
#define RANDF(lower, upper) (((float)rand() / (float)(RAND_MAX)) * (upper - lower)) + lower
#define MAP( value, start0, start1, end0, end1 ) ( ( value - start0 ) * ( ( end1 - end0 ) / ( start1 - start0 ) ) + end0 )
#define LOG( msg ) printf( "%s | File: %s, Line: %d\n", msg, __FILE__, __LINE__ )

#endif

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

typedef struct
{
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
} aColor_t;

typedef struct
{
  int x, y;
  int w, h;
  char* text;
  SDL_Surface* text_surf;
  SDL_Texture* text_tex;
} Text_t;

typedef struct
{
  int x;
  int y;
  uint8_t pressed;
  uint8_t button;
  uint8_t state;
  uint8_t clicks;
  int8_t  wheel;
} Mouse_t;

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
  Mouse_t mouse;
  int running;
  TTF_Font* g_Font;
} aApp_t;

typedef struct
{
  char filename[MAX_FILENAME_LENGTH];
  SDL_AudioSpec spec;
  uint32_t length;
  uint8_t* buffer;
} aAudioClip_t;

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

void a_PrepareScene( void );
void a_PresentScene( void );

void a_DrawPoint( const int x, const int y, const aColor_t color );
void a_DrawLine( const int x1, const int y1, const int x2, const int y2, const aColor_t color );
void a_DrawHorizontalLine( const int x1, const int x2, const int y, const aColor_t color );
void a_DrawVerticalLine( const int y1, const int y2, const int x, const aColor_t color );
void a_DrawCircle( const int posX, const int posY, const int radius, const aColor_t color );
void a_DrawFilledCircle( const int posX, const int posY, const int radius, const aColor_t color );
void a_DrawTriangle( const int x0, const int y0, const int x1, const int y1, const int x2,
                     const int y2, const aColor_t color );
void a_DrawFilledTriangle( const int x0, const int y0, const int x1, const int y1,
                           const int x2, const int y2, const aColor_t color );
void a_DrawRect( const int x, const int y, const int w, const int h, const aColor_t color );
void a_DrawFilledRect( const int x, const int y, const int w, const int h, const aColor_t color );

void a_Blit( SDL_Surface* surf, const int x, const int y );
void a_BlitRect( SDL_Surface* surf, SDL_Rect src, const int x, const int y );

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

void a_DoInput( void );

/*
---------------------------------------------------------------
---                          Text                           ---
---------------------------------------------------------------
*/

enum {
    TEXT_LEFT,
    TEXT_CENTER,
    TEXT_RIGHT
};

int a_InitFont( void );
Text_t* a_TextConstructor( void );
void a_TextDestructor( Text_t* text );
void a_RenderText( Text_t* text, int x, int y, SDL_Rect* clip,
                   double angle, SDL_Point* center, SDL_RendererFlip flip );
int a_SetText( Text_t* text, const char* string, SDL_Color color );

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
