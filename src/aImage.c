#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

static void a_CacheImage( aImageCache_t* head, const SDL_Surface* surface,
                          const char* filename, const int ID );
static SDL_Surface* a_GetImageFromCacheByID( aImageCache_t* head, const int ID );
static SDL_Surface* a_GetImageFromCacheByFilename( aImageCache_t* head, const char* filename );

void a_ImageInit( aApp_t* app )
{

}

SDL_Surface* a_Image( const char *filename )
{
  SDL_Surface *surf;

  surf = a_GetImageFromCacheByFilename( app.surfaceHead, filename );

  if ( surf == NULL )
  {
    SDL_LogMessage( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename );
    surf = IMG_Load( filename );

    if ( surf == NULL )
    {
      printf( "Failed to load image: %s, %s\n", filename, SDL_GetError() );
      return NULL;
    }
    
    a_CacheImage( app.surfaceHead, surf, filename );
  }

  return surf;
}

static void a_CacheImage( aImageCache_t* head, const SDL_Surface* surface, const char* filename, const int ID )
{

}

static SDL_Surface* a_SetImageFromCacheByID( aImageCache_t* head, const int id )
{

}

static SDL_Surface* a_GetImageFromCacheByFilename( aImageCache_t* head, const char* filename )
{

}

int a_Screenshot( SDL_Renderer *renderer, const char *filename )
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

