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

  a_InitWidgets( "resources/widgets/options.json" );
  
  app.active_widget = a_GetWidget( "name" );
}

static void aDoLoop( float dt )
{
  a_DoInput();
  if ( app.mouse.wheel == 1 )
  {
    printf( "scroll up\n" );
    app.mouse.wheel = 0;
  }

  if ( app.mouse.wheel == -1 )
  {
    printf( "scroll down\n" );
    app.mouse.wheel = 0;
  }

  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    app.running = 0;
  }

  a_DoWidget();
}

static void aRenderLoop( float dt )
{
  a_DrawText("Center aligned text.", SCREEN_WIDTH / 2, 25, 255, 255, 255, FONT_ENTER_COMMAND, TEXT_ALIGN_CENTER, 0 );
  //a_DrawText("The Quick Brown Fox Jumps Over The Lazy Dog", SCREEN_WIDTH / 2, 50, 255, 255, 255, FONT_KTM, TEXT_ALIGN_CENTER, SCREEN_WIDTH/2 );
  a_DrawFilledRect( 100, 100, 32, 32, 0, 0, 255, 255 );
  a_Blit( surf, 200, 200 );

  a_DrawWidgets();
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

