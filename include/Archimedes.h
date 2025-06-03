//#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>
//#include <Daedalus.h>

#ifndef __ARCHIMEDES_H__
#define __ARCHIMEDES_H__

/*
---------------------------------------------------------------
---                         Defines                         ---
---------------------------------------------------------------
*/

#define FPS 60

#define MAX_KEYBOARD_KEYS   350

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

#define GLYPH_WIDTH  7
#define GLYPH_HEIGHT 9

/*
---------------------------------------------------------------
---                         Structs                         ---
---------------------------------------------------------------
*/

typedef struct _aColor_t
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
} Mouse_t;

typedef struct _aTriangle_t
{
  dVec3_t points[3];
  aColor_t color;
} aTriangle_t;

typedef struct _aMesh_t
{
  aTriangle_t *triangles;
  int numberOfTriangles;
} aMesh_t;

typedef struct _aDeltaTime_t
{
  unsigned int currentTime; //Delta time
  unsigned int lastTime;

  unsigned int frameStart; //Frame time to calculate fps
  unsigned int frameTime; //Length of each frame
  unsigned int lastFrameCounterClear; //This keeps count of how long it has been since the last time frames was set to 0
  int frames; //actual count of how many frames happen over one second
} aDeltaTime_t;

typedef struct _aDelegate_t
{
  void (*logic)( float elapsedTime );
  void (*draw)( float elapsedTime );
  void (*onExit)( void );
} aDelegate_t;

typedef struct _aApp_t
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_AudioDeviceID deviceID;
  aDelegate_t delegate;
  aDeltaTime_t time;
  aColor_t background;
  dLinkedList_t* surfaceHead;
  int keyboard[MAX_KEYBOARD_KEYS];
  Mouse_t mouse;
  int running;
  TTF_Font* g_Font;
} aApp_t;

typedef struct _aAudioClip_t
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

extern void a_InitAudio( void );
extern void a_LoadSounds(char *filename, aAudioClip_t *clip);
extern void a_PlaySoundEffect(aAudioClip_t *clip);

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
---                      Delta Time                         ---
---------------------------------------------------------------
*/

extern float a_GetDeltaTime( void );

/*
---------------------------------------------------------------
---                          Draw                           ---
---------------------------------------------------------------
*/

extern void a_PrepareScene( void );
extern void a_PresentScene( void );

extern void a_DrawPoint( int x, int y, aColor_t color );
extern void a_DrawLine( int x1, int y1, int x2, int y2, aColor_t color );
extern void a_DrawHorizontalLine( int x1, int x2, int y, aColor_t color );
extern void a_DrawVerticalLine( int y1, int y2, int x, aColor_t color );
extern void a_DrawCircle( int posX, int posY, int radius, aColor_t color );
extern void a_DrawFilledCircle( int posX, int posY, int radius, aColor_t color );
extern void a_DrawTriangle( int x0, int y0, int x1, int y1, int x2, int y2, aColor_t color );
extern void a_DrawFilledTriangle( int x0, int y0, int x1, int y1, int x2, int y2, aColor_t color );
extern void a_DrawRect(int x, int y, int w, int h, aColor_t color );
extern void a_DrawFilledRect( int x, int y, int w, int h, aColor_t color );

extern void a_Blit( SDL_Surface* surf, int x, int y );
extern void a_BlitRect( SDL_Surface* surf, SDL_Rect src, int x, int y );

extern void a_UpdateTitle( const char *title );

/*
---------------------------------------------------------------
---                          Image                          ---
---------------------------------------------------------------
*/

extern SDL_Surface* a_ImageLoad( char *filename );
extern int a_Screenshot( SDL_Renderer *renderer, const char *filename );

/*
---------------------------------------------------------------
---                       Initialize                        ---
---------------------------------------------------------------
*/

extern aApp_t app;

extern void a_Init( const int width, const int height, const char *title );
extern void a_Quit( void );


/*
---------------------------------------------------------------
---                          Input                          ---
---------------------------------------------------------------
*/

extern void a_DoInput( void );

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

extern SDL_Texture *fontTexture;

extern void a_InitFont( void );
extern void a_DrawText( int x, int y, int r, int g, int b, int align, int scale, char *format, ... );


#endif
