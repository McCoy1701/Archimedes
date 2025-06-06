#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"
#include "test.h"


static void aDoLoop( float );
static void aRenderLoop( float );

void aInitGame( void )
{
  app.delegate.logic = aDoLoop;
  app.delegate.draw  = aRenderLoop;

  aImageCache_t* test_cache = ( aImageCache_t* )malloc( sizeof( aImageCache_t ) );
  if ( test_cache == NULL )
  {
    aError_t new_error;
    new_error.error_type = FATAL;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to allocate memory for cache",
              log_level_strings[new_error.error_type] );
    LOG( new_error.error_msg );
  }
  test_cache->head = NULL;
  
  at_LoadImageCache( test_cache );
  at_TestImageCache( test_cache );
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

  while( app.running ) {
    aMainloop();
  }

  a_Quit();

  return 0;
}

