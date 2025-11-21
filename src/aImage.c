/* 
 * @file src/aImage.c
 
 * This file manages the loading, caching, and retrieval of all SDL_Surface
 * image assets to prevent redundant disk access. It ensures images are loaded
 * only once and handles memory cleanup upon shutdown. It also provides utility
 * functions, such as a viewport screenshot capture
 *
 * Copyright (c) 2025 Jacob Kellum <jkellum819@gmail.com>
 *                    Mathew Storm <smattymat@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

static int a_CacheImage( aImageCache_t* head, SDL_Surface* surface,
                          const char* filename, const int ID );
//static SDL_Surface* a_GetImageFromCacheByID( aImageCache_t* head, const int ID );
static SDL_Surface* a_GetImageFromCacheByFilename( aImageCache_t* head, const char* filename );

int a_InitImage( void )
{
  app.img_cache = ( aImageCache_t* )malloc( sizeof( aImageCache_t ) );
  if ( app.img_cache == NULL )
  {
    aError_t new_error;
    new_error.error_type = FATAL;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to allocate memory for cache",
              log_level_strings[new_error.error_type] );
    LOG( new_error.error_msg );

    return 1;
  }

  app.img_cache->head = NULL;
  return 0;
}

SDL_Surface* a_Image( const char *filename )
{
  SDL_Surface *surf;

  surf = a_GetImageFromCacheByFilename( app.img_cache, filename );

  if ( surf == NULL )
  {
    //SDL_LogMessage( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename );
    surf = IMG_Load( filename );

    if ( surf == NULL )
    {
      aError_t new_error;
      new_error.error_type = FATAL;
      snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to load image: %s, %s",
               log_level_strings[new_error.error_type], filename, SDL_GetError() );
      LOG( new_error.error_msg );

      return NULL;
    }

    a_CacheImage( app.img_cache, surf, filename, 0 );
  }

  return surf;
}

static int a_CacheImage( aImageCache_t* head, SDL_Surface* surface, const char* filename, const int ID )
{
  aImageCacheNode_t* new_bucket = ( aImageCacheNode_t* )malloc( sizeof( aImageCacheNode_t ) );
  if ( new_bucket == NULL )
  {
    aError_t new_error;
    new_error.error_type = FATAL;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to allocate memory for a new bucket",
             log_level_strings[new_error.error_type] );
    LOG( new_error.error_msg );

    return 1;
  }

  new_bucket->surf = surface;
  STRNCPY( new_bucket->filename, filename, MAX_FILENAME_LENGTH );
  new_bucket->ID = ID;
  new_bucket->next = NULL;

  if ( head->head != NULL )
  {

    aImageCacheNode_t *current = head->head;
    while ( current->next != NULL )
    {
      current = current->next;
    }

    current = new_bucket;
  }

  else
  {
    head->head = new_bucket;
  }

  return 0;
}

/*static SDL_Surface* a_GetImageFromCacheByID( aImageCache_t* head, const int ID )
{
  aImageCacheNode_t* current;

  for ( current = head->head; current != NULL; current = current->next )
  {
    if ( current->ID == ID )
    {
      return current->surf;
    }
  }

  return NULL;
}*/

static SDL_Surface* a_GetImageFromCacheByFilename( aImageCache_t* head, const char* filename )
{
  aImageCacheNode_t* current;

  for ( current = head->head; current != NULL; current = current->next )
  {
    if ( strcmp( current->filename, filename ) == 0 )
    {
      return current->surf;
    }
  }

  return NULL;
}

int a_CleanUpImageCache( void )
{
  if ( app.img_cache == NULL )
      // FAIL: img_cache is NULL
  {
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: img_cache is NULL",
             log_level_strings[new_error.error_type] );
    LOG( new_error.error_msg );
    return 1;
  }

  if ( app.img_cache->head == NULL )
  {
      // FAIL: img_cache->head is NULL
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: img_cache->head is NULL",
             log_level_strings[new_error.error_type] );
    LOG( new_error.error_msg );
    return 0;
  }

  else
  {
    aImageCacheNode_t* current = app.img_cache->head;
    aImageCacheNode_t* next = NULL;

    while ( current != NULL )
    {
      next = current->next;
      if ( current->surf != NULL )
      {
        SDL_FreeSurface( current->surf );
        current->surf = NULL;

      }

      free( current );
      current = next;
    }

    app.img_cache->head = NULL;
  }

  return 0;
}

int a_Screenshot( SDL_Renderer *renderer, const char *filename )
{
  SDL_Rect aViewport;
  SDL_Surface *aSurface = NULL;

  SDL_RenderGetViewport( renderer, &aViewport );

  aSurface = SDL_CreateRGBSurface( 0, aViewport.w, aViewport.h, 32, 0, 0, 0, 0 );

  if ( aSurface == NULL )
  {
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to create surface %s",
             log_level_strings[new_error.error_type], SDL_GetError() );
    LOG( new_error.error_msg );

    return 0;
  }

  if ( SDL_RenderReadPixels( renderer, NULL, aSurface->format->format, aSurface->pixels, aSurface->pitch ) != 0 )
  {
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to read pixels from renderer: %s",
             log_level_strings[new_error.error_type], SDL_GetError() );
    LOG( new_error.error_msg );

    SDL_FreeSurface( aSurface );
    return 0;
  }

  if ( IMG_SavePNG( aSurface, filename ) != 0 )
  {
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to save surface as png: %s, %s",
             log_level_strings[new_error.error_type], filename, SDL_GetError() );
    LOG( new_error.error_msg );

    SDL_FreeSurface( aSurface );
    return 0;
  }

  SDL_FreeSurface( aSurface );
  return 1;
}

