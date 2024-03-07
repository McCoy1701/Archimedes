#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

static void aDoLoop( void );
static void aRenderLoop( void );

void aInitGame( void )
{
  app.delegate.logic = aDoLoop;
  app.delegate.draw  = aRenderLoop;
}

static void aDoLoop( void )
{
  a_do_input();
  
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    exit( 0 );
  }
}

static void aRenderLoop( void )
{

}

void aMainloop( void )
{
  a_prepare_scene();

  app.delegate.logic();
  app.delegate.draw();
  
  a_present_scene();
}

int main( void )
{
  a_init();

  aInitGame();

  #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop( aMainloop, -1, 1 );
  #endif

  #ifndef __EMSCRIPTEN__
    while( 1 ) {
      aMainloop();
    }
  #endif

  a_quit();

  return 0;
}