// src/aInput.c - Input handling system for Archimedes project
// This file defines the functions used to process input events from the game window.

#include <SDL2/SDL_mouse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Archimedes.h" 

// Forward declarations for static internal functions
static void a_DoKeyDown( SDL_KeyboardEvent *event );
static void a_DoKeyUp( SDL_KeyboardEvent *event );
static void a_DoMouseDown( SDL_MouseButtonEvent* button );
static void a_DoMouseUp( SDL_MouseButtonEvent* button );
static void a_DoMouseWheel( SDL_MouseWheelEvent* wheel );
static void a_DoMouseMotion( SDL_MouseMotionEvent* motion );

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
        a_DoMouseDown( &event.button );
        break;

      case SDL_MOUSEBUTTONUP:
        a_DoMouseUp( &event.button );
        break;

      case SDL_MOUSEWHEEL:
        a_DoMouseWheel( &event.wheel );
        break;

      case SDL_TEXTINPUT:
        // Validate input length before copying to prevent buffer overflows.
        // Assumes MAX_INPUT_LENGTH is defined in Archimedes.h and includes space for null terminator.
        if (strlen(event.text.text) > 0 && strlen(event.text.text) < MAX_INPUT_LENGTH) {
          STRNCPY( app.input_text, event.text.text, MAX_INPUT_LENGTH );
        } else {
          printf("Warning: Invalid text input length: %zu (max allowed: %d)\n",
                 strlen(event.text.text), MAX_INPUT_LENGTH - 1);
        }
        break;

      case SDL_MOUSEMOTION:
        a_DoMouseMotion( &event.motion );
        break;

      default:
        // Silently ignore unknown events (normal behavior for unhandled types)
        break;
    }
  }
}

static void a_DoKeyDown( SDL_KeyboardEvent *event )
{
  // Defensive programming: Check for NULL pointer
  if (event == NULL) {
    printf("Error: NULL keyboard event passed to a_DoKeyDown.\n");
    return;
  }

  // Validate scancode range (critical bounds check to prevent array out-of-bounds access)
  if (event->keysym.scancode < 0 || event->keysym.scancode >= MAX_KEYBOARD_KEYS) {
    printf("Warning: Scancode %d is out of valid range [0, %d) in a_DoKeyDown. Ignoring event.\n",
           event->keysym.scancode, MAX_KEYBOARD_KEYS);
    return;
  }

  // Validate repeat flag (should be 0 for initial press, >0 for repeats)
  if (event->repeat > 0) { // SDL's repeat is 0 for first press, >0 for repeats
    // printf("Debug: Ignoring key repeat event for scancode %d (repeat: %d).\n", event->keysym.scancode, event->repeat);
    return; // Ignore key repeat events
  }

  // If all checks pass and it's a non-repeat press, update the keyboard state
  app.keyboard[event->keysym.scancode] = 1;
  app.last_key_pressed = event->keysym.scancode;
  // printf("Debug: Key Down: Scancode %d (repeat: %d)\n", event->keysym.scancode, event->repeat);
}

static void a_DoKeyUp( SDL_KeyboardEvent *event )
{
  // Defensive programming: Check for NULL pointer
  if (event == NULL) {
    printf("Error: NULL keyboard event passed to a_DoKeyUp.\n");
    return;
  }

  // Validate scancode range (critical bounds check)
  if (event->keysym.scancode < 0 || event->keysym.scancode >= MAX_KEYBOARD_KEYS) {
    printf("Warning: Scancode %d is out of valid range [0, %d) in a_DoKeyUp. Ignoring event.\n",
           event->keysym.scancode, MAX_KEYBOARD_KEYS);
    return;
  }

  // Validate repeat flag (should be 0 for initial release, >0 for repeats)
  if (event->repeat > 0) { // SDL's repeat is 0 for first press, >0 for repeats
    // printf("Debug: Ignoring key repeat event for scancode %d (repeat: %d).\n", event->keysym.scancode, event->repeat);
    return; // Ignore key repeat events
  }

  // If all checks pass and it's a non-repeat release, update the keyboard state
  app.keyboard[event->keysym.scancode] = 0;
  // printf("Debug: Key Up: Scancode %d (repeat: %d)\n", event->keysym.scancode, event->repeat);
}

static void a_DoMouseDown( SDL_MouseButtonEvent* button )
{
  // Defensive programming: Check for NULL pointer
  if (button == NULL) {
    printf("Error: NULL mouse button event passed to a_DoMouseDown.\n");
    return;
  }

  // Validate mouse button ID (SDL defines specific button constants like SDL_BUTTON_LEFT, etc.)
  if (button->button < SDL_BUTTON_LEFT || button->button > SDL_BUTTON_X2) {
    printf("Warning: Invalid mouse button ID: %d in a_DoMouseDown. Event will still be processed.\n", button->button);
  }

  // Validate mouse state (should always be SDL_PRESSED for a MOUSEBUTTONDOWN event)
  if (button->state != SDL_PRESSED) {
    printf("Warning: Unexpected mouse button state: %d in a_DoMouseDown. Expected SDL_PRESSED.\n", button->state);
  }

  // Validate click count (reasonable range check, though SDL handles this)
  if (button->clicks < 0 || button->clicks > 5) { // Clicks typically 1, 2, 3. 5 is a generous upper bound.
    printf("Warning: Unusual click count: %d in a_DoMouseDown. Clamping value.\n", button->clicks);
    if (button->clicks < 0) button->clicks = 0;
    if (button->clicks > 5) button->clicks = 5;
  }

  // Validate mouse coordinates (warn about extreme values but allow them, as they might be valid for multi-monitor setups)
  if (button->x < -5000 || button->x > 5000 || // Arbitrary large range
      button->y < -5000 || button->y > 5000) {
    printf("Warning: Extreme mouse coordinates: (%d, %d) in a_DoMouseDown. Processing anyway.\n", button->x, button->y);
  }

  // Update global mouse state (all checks passed or warnings logged)
  app.mouse.state  = button->state;
  app.mouse.button = 0; // Current code sets button to 0 for DOWN events
  app.mouse.clicks = button->clicks;
  app.mouse.x = button->x;
  app.mouse.y = button->y;
  app.mouse.pressed = 1; // Explicitly mark as pressed
  // printf("Debug: Mouse Down: Button %d, State %d, Clicks %d, Pos (%d, %d)\n", button->button, button->state, button->clicks, button->x, button->y);
}

static void a_DoMouseUp( SDL_MouseButtonEvent* button )
{
  // Defensive programming: Check for NULL pointer
  if (button == NULL) {
    printf("Error: NULL mouse button event passed to a_DoMouseUp.\n");
    return;
  }

  // Validate mouse button ID
  if (button->button < SDL_BUTTON_LEFT || button->button > SDL_BUTTON_X2) {
    printf("Warning: Invalid mouse button ID: %d in a_DoMouseUp. Event will still be processed.\n", button->button);
  }

  // Validate mouse state (should always be SDL_RELEASED for a MOUSEBUTTONUP event)
  if (button->state != SDL_RELEASED) {
    printf("Warning: Unexpected mouse button state: %d in a_DoMouseUp. Expected SDL_RELEASED.\n", button->state);
  }

  // Validate click count (reasonable range check)
  if (button->clicks < 0 || button->clicks > 5) {
    printf("Warning: Unusual click count: %d in a_DoMouseUp. Clamping value.\n", button->clicks);
    if (button->clicks < 0) button->clicks = 0;
    if (button->clicks > 5) button->clicks = 5;
  }

  // Validate mouse coordinates
  if (button->x < -5000 || button->x > 5000 ||
      button->y < -5000 || button->y > 5000) {
    printf("Warning: Extreme mouse coordinates: (%d, %d) in a_DoMouseUp. Processing anyway.\n", button->x, button->y);
  }

  // Update global mouse state
  app.mouse.state  = button->state;
  app.mouse.button = button->button; // Current code sets actual button for UP events
  app.mouse.clicks = button->clicks;
  app.mouse.x = button->x;
  app.mouse.y = button->y;
  app.mouse.pressed = 0; // Explicitly mark as not pressed
  // printf("Debug: Mouse Up: Button %d, State %d, Clicks %d, Pos (%d, %d)\n", button->button, button->state, button->clicks, button->x, button->y);
}

static void a_DoMouseWheel( SDL_MouseWheelEvent* wheel )
{
  // Defensive programming: Check for NULL pointer
  if (wheel == NULL) {
    printf("Error: NULL mouse wheel event passed to a_DoMouseWheel.\n");
    return;
  }

  // Validate wheel direction values (typically -1 or 1, but be defensive)
  if (wheel->y < -10 || wheel->y > 10) { // Arbitrary range for unusual hardware
    printf("Warning: Extreme vertical wheel scroll value: %d in a_DoMouseWheel. Processing anyway.\n", wheel->y);
  }

  // Log horizontal wheel events for debugging (currently not used in app.mouse state)
  if (wheel->x != 0) {
    printf("Info: Horizontal wheel scroll detected: %d in a_DoMouseWheel. Not processed into app.mouse.wheel.\n", wheel->x);
  }

  // Update global wheel state
  app.mouse.wheel = wheel->y;
  // printf("Debug: Mouse Wheel: X=%d, Y=%d\n", wheel->x, wheel->y);
}

static void a_DoMouseMotion( SDL_MouseMotionEvent* motion )
{
  // Defensive programming: Check for NULL pointer (less critical for motion, but good practice)
  if (motion == NULL) {
    printf("Error: NULL mouse motion event passed to a_DoMouseMotion.\n");
    return;
  }

  // Update mouse coordinates directly
  app.mouse.x = motion->x;
  app.mouse.y = motion->y;
  // printf("Debug: Mouse Motion: Pos (%d, %d)\n", motion->x, motion->y);
}