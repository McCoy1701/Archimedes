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

typedef struct _aTriangle
{
  dVec4 points[3];
} aTriangle;

typedef struct _aMesh
{
  aTriangle triangles[12];
} aMesh;

typedef struct _aColor
{
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
} aColor;

typedef struct _aDelegate
{
  void (*logic)(void);
  void (*draw)(void);
} aDelegate;

typedef struct _aApp
{
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_AudioDeviceID deviceID;
  aDelegate delegate;
  int keyboard[MAX_KEYBOARD_KEYS];
  Uint32 lastTime;
  Uint32 currentTime;
  dLinkedList* surfaceHead;
} aApp;

typedef struct _aAudioClip
{
  char filename[MAX_FILENAME_LENGTH];
  SDL_AudioSpec spec;
  uint32_t length;
  uint8_t* buffer;
} aAudioClip;

/*
---------------------------------------------------------------
---                          Audio                          ---
---------------------------------------------------------------
*/

extern void aInitAudio( void );
extern void aLoadSounds(char *filename, aAudioClip *clip);
extern void aPlaySoundEffect(aAudioClip *clip);

/*
---------------------------------------------------------------
---                         Colors                          ---
---------------------------------------------------------------
*/

extern aColor black;
extern aColor blue;
extern aColor green;
extern aColor cyan;
extern aColor red;
extern aColor magenta;
extern aColor yellow;
extern aColor white;
extern aColor shit0;
extern aColor shit1;
extern aColor shit2;
extern aColor shit3;

extern aColor gray9;
extern aColor gray8;
extern aColor gray7;
extern aColor gray6;
extern aColor gray5;
extern aColor gray4;
extern aColor gray3;
extern aColor gray2;
extern aColor gray1;
extern aColor gray0;

/*
---------------------------------------------------------------
---                          Draw                           ---
---------------------------------------------------------------
*/

extern void aPrepareScene( void );
extern void aPresentScene( void );

extern void aDrawPoint( int x, int y, aColor color );
extern void aDrawLine( int x1, int y1, int x2, int y2, aColor color );
extern void aDrawHoriLine( int x1, int x2, int y, aColor color );
extern void aDrawVertLine( int y1, int y2, int x, aColor color );
extern void aDrawCircle( int posX, int posY, int radius, aColor color );
extern void aDrawFilledCircle( int posX, int posY, int radius, aColor color );
extern void aDrawTriangle( int x0, int y0, int x1, int y1, int x2, int y2, aColor color );
extern void aFillTriangle( int x1, int y1, int x2, int y2, int x3, int y3, aColor color );
extern void aDrawRect( SDL_Rect* src, int value );

extern void aBlit( SDL_Surface* surf, int x, int y );
extern void aBlitRect( SDL_Surface* surf, SDL_Rect src, int x, int y );

/*
---------------------------------------------------------------
---                          Image                          ---
---------------------------------------------------------------
*/

extern SDL_Surface* aImageLoad( char *filename );
extern int aScreenshot( SDL_Renderer *renderer, const char *filename );

/*
---------------------------------------------------------------
---                       Initialize                        ---
---------------------------------------------------------------
*/

extern aApp app;

extern void aInit( void );
extern void aQuit( void );


/*
---------------------------------------------------------------
---                          Input                          ---
---------------------------------------------------------------
*/

extern void aDoInput( void );
extern void aDoKeyDown( SDL_KeyboardEvent *event );
extern void aDoKeyUp( SDL_KeyboardEvent *event );

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

extern void aInitFont( void );
extern void aDrawText( int x, int y, int r, int g, int b, int align, int scale, char *format, ... );


#endif
