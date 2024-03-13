#ifndef __ARCHIMEDES_H__
#define __ARCHIMEDES_H__

#include <SDL2/SDL.h>
#include <Daedalus.h>

/*
---------------------------------------------------------------
---                         Defines                         ---
---------------------------------------------------------------
*/

#define FPS 60

#define MAX_KEYBOARD_KEYS   350

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
  unsigned int lastTime;
  unsigned int currentTime;
} aDeltaTime_t;

typedef struct _aDelegate_t
{
  void (*logic)(void);
  void (*draw)(void);
} aDelegate_t;

typedef struct _aApp_t
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_AudioDeviceID deviceID;
  aDelegate_t delegate;
  int keyboard[MAX_KEYBOARD_KEYS];
  aDeltaTime_t time;
  dLinkedList_t* surfaceHead;
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

extern void a_init_audio( void );
extern void a_load_sounds(char *filename, aAudioClip_t *clip);
extern void a_play_sound_effect(aAudioClip_t *clip);

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

extern float a_get_delta_time( void );

/*
---------------------------------------------------------------
---                          Draw                           ---
---------------------------------------------------------------
*/

extern void a_prepare_scene( void );
extern void a_present_scene( void );

extern void a_draw_point( int x, int y, aColor_t color );
extern void a_draw_line( int x1, int y1, int x2, int y2, aColor_t color );
extern void a_draw_horizontal_line( int x1, int x2, int y, aColor_t color );
extern void a_draw_vertical_line( int y1, int y2, int x, aColor_t color );
extern void a_draw_circle( int posX, int posY, int radius, aColor_t color );
extern void a_draw_filled_circle( int posX, int posY, int radius, aColor_t color );
extern void a_draw_triangle( int x0, int y0, int x1, int y1, int x2, int y2, aColor_t color );
extern void a_draw_filled_triangle( int x0, int y0, int x1, int y1, int x2, int y2, aColor_t color );
extern void a_draw_rect( SDL_Rect* src, int value );

extern void a_blit( SDL_Surface* surf, int x, int y );
extern void a_blit_rect( SDL_Surface* surf, SDL_Rect src, int x, int y );

/*
---------------------------------------------------------------
---                          Image                          ---
---------------------------------------------------------------
*/

extern SDL_Surface* a_image_load( char *filename );
extern int a_screenshot( SDL_Renderer *renderer, const char *filename );

/*
---------------------------------------------------------------
---                       Initialize                        ---
---------------------------------------------------------------
*/

extern aApp_t app;

extern void a_init( void );
extern void a_quit( void );


/*
---------------------------------------------------------------
---                          Input                          ---
---------------------------------------------------------------
*/

extern void a_do_input( void );
extern void a_do_key_down( SDL_KeyboardEvent *event );
extern void a_do_key_up( SDL_KeyboardEvent *event );

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

extern void a_init_font( void );
extern void a_draw_text( int x, int y, int r, int g, int b, int align, int scale, char *format, ... );


#endif
