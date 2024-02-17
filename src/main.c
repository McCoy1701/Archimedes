#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

void aInitGame( void );
void aDoLoop( void );
void aRenderLoop( void );
void aGameLoop( void );
void aMainloop( void );
int main( int argc, char* argv[] );

Color black   = {  0,   0,   0, 255};
Color blue    = {  0,   0, 255, 255};
Color green   = {  0, 255,   0, 255};
Color cyan    = {  0, 255, 255, 255};
Color red     = {255,   0,   0, 255};
Color magenta = {255,   0, 255, 255};
Color yellow  = {255, 255,   0, 255};
Color white   = {255, 255, 255, 255};
Color shit1   = {128, 128, 128, 255};
Color shit2   = {128, 255, 255, 255};
Color shit3   = {128, 128, 255, 255};
Color shit4   = {  0, 255, 128, 255};

Color gray11 = {255, 255, 255, 255};
Color gray10 = {235, 235, 235, 255};
Color gray9  = {215, 215, 215, 255};
Color gray8  = {195, 195, 195, 255};
Color gray7  = {175, 175, 175, 255};
Color gray6  = {155, 155, 155, 255};
Color gray5  = {135, 135, 135, 255};
Color gray4  = {115, 115, 115, 255};
Color gray3  = { 95,  95,  95, 255};
Color gray2  = { 55,  55,  55, 255};
Color gray1  = { 35,  35,  35, 255};
Color gray0  = {  0,   0,   0, 255};

void aInitGame( void )
{
  printf( "shit\n" );
}

void aDoLoop( void )
{
  aDoInput();
  
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    exit( 0 );
  }
}

void aRenderLoop( void )
{
  aDrawLine( 100, 100, 512, 256, blue );
}

void aGameLoop( void )
{
  aDoLoop();
  aRenderLoop();
}

void aMainloop( void )
{
  aPrepareScene();

  aGameLoop();
  
  aPresentScene();
}

int main( int argc, char* argv[] )
{
  aInit();

  aInitGame();

  #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop( aMainloop, -1, 1 );
  #endif

  #ifndef __EMSCRIPTEN__
    while( 1 ) {
      aMainloop();
    }
  #endif

  aQuit();

  return 0;
}