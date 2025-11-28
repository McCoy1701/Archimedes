#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

aAnimation_t* a_AnimationCreate( const char* filename, const float w,
                                 const float h, const int frame_count,
                                 const uint32_t frame_duration )
{
  aAnimation_t* new_animation = malloc( sizeof( aAnimation_t ) );
  if ( new_animation == NULL )
  {
    printf( "Failed to allocate memory for animation: %s, %d\n", __FILE__, __LINE__ );
    return NULL;
  }
  
  new_animation->sprite_sheet = a_ImageLoad( filename );
  new_animation->animation_timer = a_TimerCreate();
  new_animation->frame_count = frame_count;
  new_animation->frame_duration = frame_duration;
  new_animation->frame_index = 0;
  new_animation->sprite_rect = (aRectf_t){ .x = 0, .y = 0,
                                           .w = w, .h = h };

  return new_animation;
}

void a_AnimationFree( aAnimation_t* animation )
{
  if ( animation != NULL )
  {
    a_TimerFree( animation->animation_timer );
    SDL_DestroyTexture( animation->sprite_sheet->texture );
    SDL_FreeSurface( animation->sprite_sheet->surface );
    free( animation );
  }

  else
  {
    LOG( "Animation is NULL" );
  }
}

void a_AnimationPlay( aPoint2f_t pos, aAnimation_t* animation )
{
  if ( !a_TimerStarted( animation->animation_timer ) )
  {
    a_TimerStart( animation->animation_timer );
  }
  else
  {
    if ( a_TimerOneshot( animation->animation_timer, animation->frame_duration ) )
    {
      //increment frame_index
      animation->frame_index++;
      a_TimerStop( animation->animation_timer );
    }

    //display frame
    animation->sprite_rect.x += animation->frame_index * animation->sprite_rect.w;
    animation->sprite_rect.y += animation->frame_index * animation->sprite_rect.h;
  }
}

