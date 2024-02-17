#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

void aDoInput( void )
{
  SDL_Event event;
  
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
      case SDL_QUIT:
        exit(0);
        break;

      case SDL_KEYDOWN:
        aDoKeyDown(&event.key);
        break;
          
      case SDL_KEYUP:
        aDoKeyUp(&event.key);
        break;

      default:
        break;        
    }
  }
}

void aDoKeyDown( SDL_KeyboardEvent *event )
{
  if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
    app.keyboard[event->keysym.scancode] = 1;
  }
}

void aDoKeyUp( SDL_KeyboardEvent *event )
{
  if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
    app.keyboard[event->keysym.scancode] = 0;
  }
}
