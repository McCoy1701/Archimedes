#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

#include "Archimedes.h"

SDL_Surface* aImageLoad( char *filename )
{
  SDL_Surface *surf;

  surf = ( SDL_Surface* )dGetDataInLinkedListByName( app.surfaceHead, filename );

  if ( surf == NULL )
  {
    SDL_LogMessage( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename );
    surf = IMG_Load( filename );

    if ( surf == NULL )
    {
      printf( "Failed to load image: %s, %s\n", filename, SDL_GetError() );
      return NULL;
    }
    
    dPushBack( app.surfaceHead, surf, filename, sizeof( SDL_Surface ) );
  }

  return surf;
}

int aScreenshot( const char *filename, SDL_Renderer *renderer )
{
  SDL_Rect aViewport;
  SDL_Surface *aSurface = NULL;

  SDL_RenderGetViewport( renderer, &aViewport );

  aSurface = SDL_CreateRGBSurface( 0, aViewport.w, aViewport.h, 32, 0, 0, 0, 0 );

  if ( aSurface == NULL )
  {
    printf( "Failed to create surface %s\n", SDL_GetError() );
    return 0;
  }

  if ( SDL_RenderReadPixels( renderer, NULL, aSurface->format->format, aSurface->pixels, aSurface->pitch ) != 0 )
  {
    printf( "Failed to read pixels from renderer: %s\n", SDL_GetError() );
    SDL_FreeSurface( aSurface );
    return 0;
  }

  if ( IMG_SavePNG( aSurface, filename ) != 0 )
  {
    printf( "Failed to save surface as png: %s, %s", filename, SDL_GetError() );
    SDL_FreeSurface( aSurface );
    return 0;
  }
  
  SDL_FreeSurface( aSurface );
  return 1;
}
