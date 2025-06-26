#include <stdlib.h>
#include "Archimedes.h"

static SDL_Texture* GetTexture( char* name );
static void AddTextureToCache( char* name, SDL_Texture* sdl_texture );

static aTexture_t  textures_head;
static aTexture_t* textures_tail;

void a_InitTextures( void )
{
  memset( &textures_head, 0, sizeof( aTexture_t ) );
  textures_tail = &textures_head;
}

SDL_Texture* a_LoadTexture( char* filename )
{
  SDL_Texture* texture;

  texture = GetTexture( filename );

  if ( texture == NULL )
  {
    SDL_LogMessage( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s ...", filename );
    texture = IMG_LoadTexture( app.renderer, filename );
    AddTextureToCache( filename, texture );
  }

  return texture;
}

SDL_Texture* a_ToTexture( SDL_Surface* surf, int destroy )
{
  SDL_Texture* texture;

  texture = SDL_CreateTextureFromSurface( app.renderer, surf );

  if ( destroy )
  {
    SDL_FreeSurface( surf );
    surf = NULL;
  }

  return texture;
}

static SDL_Texture* GetTexture( char* name )
{
  aTexture_t* t;

  for ( t = textures_head.next; t != NULL; t = t->next )
  {
    if ( strcmp( t->name, name ) == 0 )
    {
      return t->texture;
    }
  }

  return NULL;
}

static void AddTextureToCache( char* name, SDL_Texture* sdl_texture )
{
  aTexture_t* texture;

  texture = malloc( sizeof( aTexture_t ) );
  memset( texture, 0, sizeof( aTexture_t ) );

  textures_tail->next = texture;
  textures_tail = texture;

  STRNCPY( texture->name, name, MAX_FILENAME_LENGTH );
  texture->texture = sdl_texture;
}

