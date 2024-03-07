#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

void a_do_input( void )
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
        a_do_key_down(&event.key);
        break;
          
      case SDL_KEYUP:
        a_do_key_up(&event.key);
        break;

      default:
        break;        
    }
  }
}

void a_do_key_down( SDL_KeyboardEvent *event )
{
  if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
    app.keyboard[event->keysym.scancode] = 1;
  }
}

void a_do_key_up( SDL_KeyboardEvent *event )
{
  if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
    app.keyboard[event->keysym.scancode] = 0;
  }
}
