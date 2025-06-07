#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

static void a_DoKeyDown( SDL_KeyboardEvent *event );
static void a_DoKeyUp( SDL_KeyboardEvent *event );
static void a_DoMouse( SDL_MouseButtonEvent* button );

void a_DoInput( void )
{
  SDL_Event event;

  while ( SDL_PollEvent( &event ) )
  {
    switch ( event.type )
    {
      case SDL_QUIT:
        app.running = 0;
        break;

      case SDL_KEYDOWN:
        a_DoKeyDown( &event.key );
        break;
          
      case SDL_KEYUP:
        a_DoKeyUp( &event.key );
        break;
      
      case SDL_MOUSEBUTTONDOWN:
        a_DoMouse( &event.button );
        break;
      
      case SDL_MOUSEBUTTONUP:
        a_DoMouse( &event.button );
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

static void a_DoMouse( SDL_MouseButtonEvent* button )
{
  app.mouse.state  = button->state;
  app.mouse.button = button->button;
  app.mouse.clicks = button->clicks;
  app.mouse.x = button->x;
  app.mouse.y = button->y;
}

