#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Archimedes.h"

static void we_DoLoop( float );
static void we_RenderLoop( float );

void Init_WidgetEditor( void )
{
  app.delegate.logic = we_DoLoop;
  app.delegate.draw  = we_RenderLoop;

}

static void we_DoLoop( float dt )
{

}

static void we_RenderLoop( float dt )
{

}

void aMainloop( void )
{
  float dt = a_GetDeltaTime();
  a_GetFPS();
  a_PrepareScene();
  
  app.delegate.logic( dt );
  app.delegate.draw( dt );
  
  a_PresentScene();
  app.time.frames++;
  
  if ( app.options.frame_cap )
  {
    int frame_tick = SDL_GetTicks();
    if ( frame_tick < LOGIC_RATE )
    {
      SDL_Delay( LOGIC_RATE - frame_tick );
    }
  }
}

int main( void )
{
  a_Init( SCREEN_WIDTH, SCREEN_HEIGHT, "Archimedes: Widget Editor" );

  Init_WidgetEditor();

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

