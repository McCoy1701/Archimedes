#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

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
  aDrawText( 200, 200, 255, 255, 255, TEXT_CENTER, 0, "FUCKING BITCH" );
}

void aInitGame( void )
{
  app.delegate.logic = aDoLoop;
  app.delegate.draw = aRenderLoop;
}

void aMainloop( void )
{
  aPrepareScene();

  app.delegate.logic();
  app.delegate.draw();
  
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