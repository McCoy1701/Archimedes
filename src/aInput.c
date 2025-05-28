#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

static void a_DoKeyDown( SDL_KeyboardEvent *event );
static void a_DoKeyUp( SDL_KeyboardEvent *event );

void a_DoInput( void )
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
        a_DoKeyDown(&event.key);
        break;
          
      case SDL_KEYUP:
        a_DoKeyUp(&event.key);
        break;

      default:
        break;        
    }
  }
}

static void a_DoKeyDown( SDL_KeyboardEvent *event )
{
  if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
    app.keyboard[event->keysym.scancode] = 1;
  }
}

static void a_DoKeyUp( SDL_KeyboardEvent *event )
{
  if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
    app.keyboard[event->keysym.scancode] = 0;
  }
}
