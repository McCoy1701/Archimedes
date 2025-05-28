#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

float a_GetDeltaTime( void )
{
  app.time.lastTime = app.time.currentTime;
  app.time.currentTime = SDL_GetTicks();
  Uint32 elapsedTime = app.time.currentTime - app.time.lastTime;
  return(float)elapsedTime / 1000.0f;
}
