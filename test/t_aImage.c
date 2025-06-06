#include <stdlib.h>
#include <stdio.h>

#include "Archimedes.h"


aImageCacheNode_t* at_InitImageCacheNode( SDL_Surface* surf, const char* filename, const int ID )
{
  aImageCacheNode_t* cache_node = ( aImageCacheNode_t* )malloc( sizeof( aImageCacheNode_t ) );
  if ( cache_node == NULL )
  {
    aError_t new_error;
    new_error.error_type = FATAL;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to allocate memory for cache",
              log_level_strings[new_error.error_type] );
    LOG( new_error.error_msg );
    return NULL;
  }

  STRNCPY( cache_node->filename, filename, MAX_FILENAME_LENGTH );
  cache_node->ID = ID;
  cache_node->next = NULL;
  cache_node->surf = surf;

  return cache_node;
}

int at_LoadImageCache( aImageCache_t* cache )
{
  const char* filename = "resources/assets/enemy.png";

  for ( int i = 0; i < 10; i++ )
  {
    char name[MAX_FILENAME_LENGTH];
    snprintf( name, MAX_FILENAME_LENGTH, "Enemy: %d", i );
    SDL_Surface* surf = IMG_Load( filename );
   
    aImageCacheNode_t* new_bucket = at_InitImageCacheNode( surf, name, i );
    
    printf( "NB %s, %d", new_bucket->filename, new_bucket->ID );
    if ( cache->head != NULL )
    {
      aImageCacheNode_t* current = cache->head;
      while( current->next != NULL )
      {
        current = current->next;
      }

      current->next = new_bucket;
      printf( "CUR %s, %d\n", current->filename, current->ID );
    }
    
    else
    {
      cache->head = new_bucket;
      printf( "CAC %s, %d\n", cache->head->filename, cache->head->ID );
    }

  }

  return 0;
}

int at_TestImageCache( aImageCache_t* cache )
{
  aImageCacheNode_t* current = cache->head;
  if ( cache == NULL )
  {
    printf("NULL\n");
  }
  while ( current != NULL )
  {
    printf( "%s, %d\n", current->filename, current->ID );
    current = current->next;
  }

  return 0;
}

