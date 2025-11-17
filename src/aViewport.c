/* 
 * src/aViewport.c
 *
 * This file defines the functions used to draw basic constructs
 * to the screen with respect to a viewport.
 *
 * Copyright (c) 2025 Jacob Kellum <jkellum819@gmail.com>
 *                    Mathew Storm <smattymat@gmail.com>
 */

#include "Archimedes.h"

aPoint2f_t a_CalculateScaleOfViewport( void )
{
  aPoint2f_t scale = { .x = (float)SCREEN_WIDTH  / ( app.g_viewport.w * 2.0f ),
                    .y = (float)SCREEN_HEIGHT / ( app.g_viewport.h * 2.0f ) };
  return scale;
}

uint8_t a_IsRectVisibleInViewPort( aRectf_t rect )
{
  if ( rect.x + rect.w < app.g_viewport.x - app.g_viewport.w ||
       rect.x - rect.w > app.g_viewport.x + app.g_viewport.w || 
       rect.y + rect.h < app.g_viewport.y - app.g_viewport.h ||
       rect.y - rect.h > app.g_viewport.y + app.g_viewport.h )
  {
    return 0;
  }

  return 1;
}

uint8_t a_IsPointVisibleInViewPort( aPoint2f_t point )
{
  if ( point.x < app.g_viewport.x - app.g_viewport.w ||
       point.x > app.g_viewport.x + app.g_viewport.w || 
       point.y < app.g_viewport.y - app.g_viewport.h ||
       point.y > app.g_viewport.y + app.g_viewport.h )
  {
    return 0;
  }

  return 1;
}

void a_DrawPointToViewport( aPoint3f_t p, aColor_t color )
{
  aPoint2f_t current_scale = a_CalculateScaleOfViewport();

  float viewport_x1 = app.g_viewport.x - app.g_viewport.w;
  float viewport_y1 = app.g_viewport.y - app.g_viewport.h;

  int x = (int)( ( p.x - viewport_x1 ) * current_scale.x );
  int y = (int)( ( p.y - viewport_y1 ) * current_scale.y );

  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawPoint(app.renderer, x, y);
  // Reset the renderer color to white
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}

void a_DrawRectToViewport( aRectf_t rect, aColor_t color )
{
  aPoint2f_t current_scale = a_CalculateScaleOfViewport();

  float viewport_x1 = app.g_viewport.x - app.g_viewport.w;
  float viewport_y1 = app.g_viewport.y - app.g_viewport.h;

  float world_x1 = rect.x - rect.w;
  float world_y1 = rect.y - rect.h;

  float world_width  = rect.w * 2.0f;
  float world_height = rect.h * 2.0f;

  SDL_Rect r = {
    (int)( ( world_x1 - viewport_x1 ) * current_scale.x ),
    (int)( ( world_y1 - viewport_y1 ) * current_scale.y ),
    (int)( world_width  * current_scale.x ),
    (int)( world_height * current_scale.y )
  };
  
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
  SDL_SetRenderDrawColor( app.renderer, color.r, color.g, color.b, color.a );
 
  SDL_RenderDrawRect( app.renderer, &r );
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}

