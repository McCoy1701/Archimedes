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
