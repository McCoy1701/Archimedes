/* 
 * src/aTimer_t.c
 *
 * This file defines the functions used with Archimedes built-in timers.
 *
 * Copyright (c) 2025 Jacob Kellum <jkellum819@gmail.com>
 *                    Mathew Storm <smattymat@gmail.com>
 */

#include "Archimedes.h"
#include <SDL2/SDL_timer.h>

aTimer_t* a_CreateTimer( void )
{
  aTimer_t* new_timer = malloc( sizeof( aTimer_t* ) );
  if ( new_timer == NULL )
  {
    printf( "Failed to allocate memory for new_timer: %s, %d\n", __FILE__, __LINE__ );
    return NULL;

  }

  new_timer->start_ticks  = 0;
  new_timer->paused_ticks = 0;
  new_timer->started      = 0;
  new_timer->paused       = 0;

  return new_timer;
}

void a_FreeTimer( aTimer_t* timer )
{
  if ( timer == NULL )
  {
    printf( "Tried to free NULL timer: %s, %d\n", __FILE__, __LINE__ );
    return;
  }

  free( timer );
}

void a_StartTimer( aTimer_t* timer )
{
  timer->started      = 1;
  timer->paused       = 0;
  timer->start_ticks  = SDL_GetTicks();
  timer->paused_ticks = 0;
}

void a_StopTimer( aTimer_t* timer )
{
  timer->started      = 0;
  timer->paused       = 0;
  timer->start_ticks  = 0;
  timer->paused_ticks = 0;

}

void a_PauseTimer( aTimer_t* timer )
{
  if ( timer->started && !timer->paused )
  {
    timer->started      = 0;
    timer->paused       = 1;

    timer->paused_ticks = SDL_GetTicks() - timer->start_ticks;
    timer->start_ticks  = 0;
  }
}

void a_UnpauseTimer( aTimer_t* timer )
{
  if ( !timer->started && timer->paused )
  {
    timer->started      = 1;
    timer->paused       = 0;

    timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;
    timer->paused_ticks  = 0;
  }

}

uint32_t a_GetTicks( aTimer_t* timer )
{
  uint32_t time = 0;

  if ( timer->started )
  {
    if ( timer->paused )
    {
      time = timer->paused_ticks;
    }
    else
    {
      time = SDL_GetTicks() - timer->start_ticks;
    }
  }
  
  return time;
}

uint8_t a_IsStarted( aTimer_t* timer )
{
  return timer->started;
}

uint8_t a_IsPaused( aTimer_t* timer )
{
  return timer->paused && timer->started;
}

