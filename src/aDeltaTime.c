#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

float a_GetDeltaTime( void )
{
  app.time.last_time = app.time.current_time;
  app.time.current_time = SDL_GetTicks();
  Uint32 elapsed_time = app.time.current_time - app.time.last_time;
  return(float)elapsed_time / 1000.0f;
}

void a_GetFPS( void )
{
  app.time.avg_FPS = app.time.frames / ( a_TimerGetTicks( app.time.FPS_timer ) / 1000.0f );

  if ( app.time.avg_FPS > 200000 )
  {
    app.time.avg_FPS = 0;
  }
}

