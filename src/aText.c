#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

int a_InitFont( void )
{
  app.g_Font = TTF_OpenFont( "resources/fonts/JetBrains.ttf", 12 );

  if ( app.g_Font == NULL )
  {
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to open font: %s",
              log_level_strings[new_error.error_type], TTF_GetError() );
    LOG( new_error.error_msg );
    return 1;
  }

  return 0;
}

Text_t* a_TextConstructor( void )
{
  Text_t* new_text = ( Text_t* )malloc( sizeof( Text_t ) );
  if ( new_text == NULL )
  {
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to allocate memory for text",
              log_level_strings[new_error.error_type] );
    LOG( new_error.error_msg );
    return NULL;
  }

  new_text->text      = NULL;
  new_text->text_tex  = NULL;
  new_text->text_surf = NULL;

  new_text->x = 0;
  new_text->y = 0;
  new_text->w = 0;
  new_text->h = 0;

  return new_text;
}

void a_TextDestructor( Text_t* text )
{
  if ( text == NULL )
  {
    return;
  }

  if ( text->text != NULL )
  {
    free( text->text );
  }

  if ( text->text_tex != NULL )
  {
    SDL_DestroyTexture( text->text_tex );
  }

  if ( text->text_surf != NULL )
  {
    SDL_FreeSurface( text->text_surf );
  }

  if ( text != NULL )
  {  
    free( text );
  }

  return;
}

void a_RenderText( Text_t* text, int x, int y, SDL_Rect* clip, double angle,
                   SDL_Point* center, SDL_RendererFlip flip )
{
  SDL_Rect render_quad = { x, y, text->w, text->h };
  if ( clip != NULL )
  {
    render_quad.w = clip->w;
    render_quad.h = clip->h;
  }

  SDL_RenderCopyEx( app.renderer, text->text_tex, clip, &render_quad, angle, center, flip );
}

int a_SetText( Text_t* text, const char* string, SDL_Color color )
{
  int success = 0;
  if ( text->text_tex != NULL )
  {
    SDL_DestroyTexture( text->text_tex );
    text->text_tex = NULL;
  }

  text->text_surf = TTF_RenderText_Solid( app.g_Font, string, color );

  if ( text->text_surf == NULL )
  {
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to render text: %s",
              log_level_strings[new_error.error_type], TTF_GetError() );
    LOG( new_error.error_msg );
    success = 1;
  }

  text->text_tex = SDL_CreateTextureFromSurface( app.renderer, text->text_surf );
  
  if ( text->text_tex == NULL )
  {
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to create texture from surface: %s",
              log_level_strings[new_error.error_type], SDL_GetError() );
    LOG( new_error.error_msg );
    success = 1;
  }

  text->w = text->text_surf->w;
  text->h = text->text_surf->h;

  SDL_FreeSurface( text->text_surf );
  text->text_surf = NULL;

  if ( text->text != NULL )
  {
    free( text->text );
    text->text = NULL;
  }

  text->text = ( char* )malloc( strlen( string ) + 1 );
  if ( text->text == NULL )
  {
    aError_t new_error;
    new_error.error_type = WARNING;
    snprintf( new_error.error_msg, MAX_LINE_LENGTH, "%s: Failed to allocate memory for text->text",
              log_level_strings[new_error.error_type] );
    LOG( new_error.error_msg );
    success = 1;
  }

  memcpy( text->text, string, strlen( string ) + 1 );

  return success;
}

