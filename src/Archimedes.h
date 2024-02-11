#ifndef __ARCHIMEDES_H__
#define __ARCHIMEDES_H__

#include <SDL2/SDL.h>

/*
---------------------------------------------------------------
---                         Defines                         ---
---------------------------------------------------------------
*/

#define FPS 60

#define MAX_KEYBOARD_KEYS   350
#define MAX_NAME_LENGTH     100
#define MAX_LINE_LENGTH     1024
#define MAX_FILENAME_LENGTH 256

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

/*
---------------------------------------------------------------
---                         Structs                         ---
---------------------------------------------------------------
*/

typedef struct _color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} Color;

typedef struct _delegate {
  void (*logic)(void);
  void (*draw)(void);
} Delegate;

typedef struct _app {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_AudioDeviceID deviceID;
  Delegate delegate;
  int keyboard[MAX_KEYBOARD_KEYS];
  Uint32 lastTime;
  Uint32 currentTime;
} App;

typedef struct _audioClip {
  char filename[MAX_NAME_LENGTH];
  SDL_AudioSpec spec;
  uint32_t length;
  uint8_t* buffer;
} AudioClip;

/*
---------------------------------------------------------------
---                       Initialize                        ---
---------------------------------------------------------------
*/

extern App app;

void aInit( void );
void aQuit( void );

/*
---------------------------------------------------------------
---                          Audio                          ---
---------------------------------------------------------------
*/

void aInitAudio( void );
void aLoadSounds(char *filename, AudioClip *clip);
void aPlaySoundEffect(AudioClip *clip);

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

static char drawTextBuffer[MAX_LINE_LENGTH];
void aInitFont( void );
void aDrawText( int x, int y, int r, int g, int b, int align, int scale, char *format, ... );

/*
---------------------------------------------------------------
---                          Input                          ---
---------------------------------------------------------------
*/

void aDoInput( void );
void aDoKeyDown( SDL_KeyboardEvent *event );
void aDoKeyUp( SDL_KeyboardEvent *event );

/*
---------------------------------------------------------------
---                          Draw                           ---
---------------------------------------------------------------
*/

void aPrepareScene(void);
void aPresentScene(void);

void aDrawPoint( int x, int y, Color color );
void aDrawLine( int x1, int y1, int x2, int y2, Color color );
void aDrawHoriLine( int x1, int x2, int y );
void aDrawVertLine( int y1, int y2, int x );
void aDrawCircle( int x, int y, int radius );
void aDrawTriangle( int x0, int y0, int x1, int y1, int x2, int y2, Color color );
void aFillTriangle( int x1, int y1, int x2, int y2, int x3, int y3, Color color );
void aDrawRect( SDL_Rect* src, int value );

void aBlit( SDL_Surface* surf, int x, int y );
void aBlitRect( SDL_Surface* surf, SDL_Rect src, int x, int y );

#endif
