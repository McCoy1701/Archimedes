#include <stdio.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Archimedes.h"

static void aDoLoop( float );
static void aRenderLoop( float );

SDL_Surface* surf;

void aInitGame( void )
{
  app.delegate.logic = aDoLoop;
  app.delegate.draw  = aRenderLoop;
  
  surf = a_Image( "resources/assets/bullet.png" );
  if ( surf == NULL )
  {
    printf( "Failed to load image\n" );
  }
}

static void aDoLoop( float dt )
{
  a_DoInput();
  
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    app.running = 0;
  }
}

static void aRenderLoop( float dt )
{
  a_DrawFilledRect( 100, 100, 32, 32, blue );
  a_Blit( surf, 200, 200 );
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
    emscripten_set_main_loop( aMainloop, 0, 1 );
  #endif

  #ifndef __EMSCRIPTEN__
    while( app.running ) {
      aMainloop();
    }
  #endif

  a_Quit();

  return 0;
}

