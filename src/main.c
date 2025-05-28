#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

static void aDoLoop( float );
static void aRenderLoop( float );

void aInitGame( void )
{
  app.delegate.logic = aDoLoop;
  app.delegate.draw  = aRenderLoop;
}

static void aDoLoop( float dt )
{
  a_DoInput();
  
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    exit( 0 );
  }
}

static void aRenderLoop( float dt )
{

}

void aMainloop( void )
{
  a_PrepareScene();

  app.delegate.logic( a_GetDeltaTime() );
  app.delegate.draw( a_GetDeltaTime() );
  
  a_PresentScene();
}

int main( void )
{
  a_Init( SCREEN_WIDTH, SCREEN_HEIGHT, "Archimedes" );

  aInitGame();

  #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop( aMainloop, -1, 1 );
  #endif

  #ifndef __EMSCRIPTEN__
    while( 1 ) {
      aMainloop();
    }
  #endif

  a_Quit();

  return 0;
}
