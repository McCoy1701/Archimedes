#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"
#include "test.h"


static void aDoLoop( float );
static void aRenderLoop( float );

SDL_Surface* surf;
aImageCache_t* test_cache;

void aInitGame( void )
{
  app.delegate.logic = aDoLoop;
  app.delegate.draw  = aRenderLoop;

/*  test_cache = ( aImageCache_t* )malloc( sizeof( aImageCache_t ) );
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
  surf = a_Image( "resources/assets/bullet.png" );
  if ( surf == NULL )
  {
    printf( "Failed to load image\n" );
  }
  at_TestImageCache( app.img_cache );*/
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
  int x = 100;
  int y = 100;

  //a_Blit(surf, x, y);
  a_DrawText("Hello, World!", x, y, 255, 255, 255, FONT_CODE_PAGE_437, TEXT_ALIGN_CENTER, 0 );
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

  /*at_TestImageCache( test_cache );
  at_TestImageCache( app.img_cache );
  a_CleanUpImageCache( test_cache );
  a_CleanUpImageCache( app.img_cache );
  at_TestImageCache( test_cache );
  at_TestImageCache( app.img_cache );*/
  a_Quit();

  return 0;
}

