#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

static void a_DoKeyDown( SDL_KeyboardEvent *event );
static void a_DoKeyUp( SDL_KeyboardEvent *event );
static void a_DoMouse( SDL_MouseButtonEvent* button );
static void a_DoMouseWheel( SDL_MouseWheelEvent* wheel );

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
      
      case SDL_MOUSEWHEEL:
        a_DoMouseWheel( &event.wheel );
        break;

      case SDL_TEXTINPUT:
        STRNCPY( app.input_text, event.text.text, MAX_INPUT_LENGTH );
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

static void a_DoMouseWheel( SDL_MouseWheelEvent* wheel )
{
  app.mouse.wheel = wheel->y;
}

