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
  dLinkedList* surfaceHead;
} App;

typedef struct _audioClip {
  char filename[MAX_FILENAME_LENGTH];
  SDL_AudioSpec spec;
  uint32_t length;
  uint8_t* buffer;
} AudioClip;

/*
---------------------------------------------------------------
---                          Audio                          ---
---------------------------------------------------------------
*/

extern void aInitAudio( void );
extern void aLoadSounds(char *filename, AudioClip *clip);
extern void aPlaySoundEffect(AudioClip *clip);

/*
---------------------------------------------------------------
---                         Colors                          ---
---------------------------------------------------------------
*/

extern Color black;
extern Color blue;
extern Color green;
extern Color cyan;
extern Color red;
extern Color magenta;
extern Color yellow;
extern Color white;
extern Color shit0;
extern Color shit1;
extern Color shit2;
extern Color shit3;

extern Color gray9;
extern Color gray8;
extern Color gray7;
extern Color gray6;
extern Color gray5;
extern Color gray4;
extern Color gray3;
extern Color gray2;
extern Color gray1;
extern Color gray0;

/*
---------------------------------------------------------------
---                          Draw                           ---
---------------------------------------------------------------
*/

extern void aPrepareScene( void );
extern void aPresentScene( void );

extern void aDrawPoint( int x, int y, Color color );
extern void aDrawLine( int x1, int y1, int x2, int y2, Color color );
extern void aDrawHoriLine( int x1, int x2, int y, Color color );
extern void aDrawVertLine( int y1, int y2, int x, Color color );
extern void aDrawCircle( int posX, int posY, int radius, Color color );
extern void aDrawFilledCircle( int posX, int posY, int radius, Color color );
extern void aDrawTriangle( int x0, int y0, int x1, int y1, int x2, int y2, Color color );
extern void aFillTriangle( int x1, int y1, int x2, int y2, int x3, int y3, Color color );
extern void aDrawRect( SDL_Rect* src, int value );

extern void aBlit( SDL_Surface* surf, int x, int y );
extern void aBlitRect( SDL_Surface* surf, SDL_Rect src, int x, int y );

/*
---------------------------------------------------------------
---                          Image                          ---
---------------------------------------------------------------
*/

extern SDL_Surface* aImageLoad( char *filename );
extern int aScreenshot( const char *filename, SDL_Renderer *renderer );

/*
---------------------------------------------------------------
---                       Initialize                        ---
---------------------------------------------------------------
*/

extern App app;

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
