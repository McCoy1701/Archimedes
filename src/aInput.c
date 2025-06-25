// File: src/aInput.c - Input handling system for Archimedes project

#include <SDL2/SDL_mouse.h>
#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

static void a_DoKeyDown( SDL_KeyboardEvent *event );
static void a_DoKeyUp( SDL_KeyboardEvent *event );
static void a_DoMouseDown( SDL_MouseButtonEvent* button );
static void a_DoMouseUp( SDL_MouseButtonEvent* button );
static void a_DoMouseWheel( SDL_MouseWheelEvent* wheel );
static void a_DoMouseMotion( SDL_MouseMotionEvent* motion );

void a_DoInput( void )
{
  SDL_Event event;

  // Defensive check: Ensure we have a valid event structure
  memset(&event, 0, sizeof(event));

  while ( SDL_PollEvent( &event ) )
  {
    // Validate event structure integrity (basic sanity check)
    if (event.type < 0 || event.type > SDL_LASTEVENT) {
      printf("Warning: Invalid SDL event type received: %d\n", event.type);
      continue;
    }

    switch ( event.type )
    {
      case SDL_QUIT:
        app.running = 0;
        break;

      case SDL_KEYDOWN:
        if (&event.key != NULL) {
          a_DoKeyDown( &event.key );
        } else {
          printf("Warning: NULL keyboard event in SDL_KEYDOWN\n");
        }
        break;

      case SDL_KEYUP:
        if (&event.key != NULL) {
          a_DoKeyUp( &event.key );
        } else {
          printf("Warning: NULL keyboard event in SDL_KEYUP\n");
        }
        break;

      case SDL_MOUSEBUTTONDOWN:
        if (&event.button != NULL) {
          a_DoMouseDown( &event.button );
        } else {
          printf("Warning: NULL mouse button event in SDL_MOUSEBUTTONDOWN\n");
        }
        break;

      case SDL_MOUSEBUTTONUP:
        if (&event.button != NULL) {
          a_DoMouseUp( &event.button );
        } else {
          printf("Warning: NULL mouse button event in SDL_MOUSEBUTTONUP\n");
        }
        break;

      case SDL_MOUSEWHEEL:
        if (&event.wheel != NULL) {
          a_DoMouseWheel( &event.wheel );
        } else {
          printf("Warning: NULL mouse wheel event in SDL_MOUSEWHEEL\n");
        }
        break;

      case SDL_TEXTINPUT:
        if (event.text.text != NULL) {
          // Additional validation for text input
          if (strlen(event.text.text) > 0 && strlen(event.text.text) < 32) {
            STRNCPY( app.input_text, event.text.text, MAX_INPUT_LENGTH );
          } else {
            printf("Warning: Invalid text input length: %zu\n", strlen(event.text.text));
          }
        } else {
          printf("Warning: NULL text in SDL_TEXTINPUT event\n");
        }
        break;

      case SDL_MOUSEMOTION:
        a_DoMouseMotion( &event.motion );
        break;

      default:
        // Silently ignore unknown events (normal behavior)
        break;
    }
  }
}

/*
 * Handle SDL keyboard key press events and update keyboard state
 *
 * `event` - Pointer to SDL keyboard event structure
 *
 * -- Internal function called by a_DoInput() for SDL_KEYDOWN events
 * -- Updates app.keyboard[scancode] = 1 for the pressed key
 * -- Ignores key repeat events (event->repeat != 0) to provide clean press detection
 * -- Only processes scancodes within valid range (< MAX_KEYBOARD_KEYS)
 * -- Uses SDL scancodes for consistent cross-platform key identification
 * -- Does nothing if event is NULL or scancode is out of bounds
 * -- Key state remains active until corresponding a_DoKeyUp() call
 * -- Includes comprehensive error checking and defensive programming
 * -- Validates scancode range and repeat flags before processing
 */
static void a_DoKeyDown( SDL_KeyboardEvent *event )
{
  // Defensive programming: Check for NULL pointer
  if (event == NULL) {
    printf("Error: NULL keyboard event passed to a_DoKeyDown\n");
    return;
  }

  // Validate scancode range (critical bounds check)
  if (event->keysym.scancode >= MAX_KEYBOARD_KEYS) {
    printf("Warning: Scancode %d exceeds maximum keyboard keys (%d)\n",
           event->keysym.scancode, MAX_KEYBOARD_KEYS);
    return;
  }

  // Validate repeat flag (should be 0 or 1, but be defensive)
  if (event->repeat > 1) {
    printf("Warning: Unusual repeat value in key down event: %d\n", event->repeat);
  }

  // Only process non-repeat key presses within valid range
  if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
    // Additional validation: ensure scancode is not negative (should never happen with SDL_Scancode enum)
    if (event->keysym.scancode >= 0) {
      app.keyboard[event->keysym.scancode] = 1;
      app.last_key_pressed = event->keysym.scancode;
    } else {
      printf("Error: Negative scancode received: %d\n", event->keysym.scancode);
    }
  }
}

/*
 * Handle SDL keyboard key release events and update keyboard state
 *
 * `event` - Pointer to SDL keyboard event structure
 *
 * -- Internal function called by a_DoInput() for SDL_KEYUP events
 * -- Updates app.keyboard[scancode] = 0 for the released key
 * -- Ignores key repeat events (event->repeat != 0) for consistency with key down
 * -- Only processes scancodes within valid range (< MAX_KEYBOARD_KEYS)
 * -- Uses SDL scancodes for consistent cross-platform key identification
 * -- Does nothing if event is NULL or scancode is out of bounds
 * -- Provides clean key release detection for game input handling
 * -- Includes comprehensive error checking and defensive programming
 * -- Validates scancode range and repeat flags before processing
 */
static void a_DoKeyUp( SDL_KeyboardEvent *event )
{
  // Defensive programming: Check for NULL pointer
  if (event == NULL) {
    printf("Error: NULL keyboard event passed to a_DoKeyUp\n");
    return;
  }

  // Validate scancode range (critical bounds check)
  if (event->keysym.scancode >= MAX_KEYBOARD_KEYS) {
    printf("Warning: Scancode %d exceeds maximum keyboard keys (%d)\n",
           event->keysym.scancode, MAX_KEYBOARD_KEYS);
    return;
  }

  // Validate repeat flag (should be 0 or 1, but be defensive)
  if (event->repeat > 1) {
    printf("Warning: Unusual repeat value in key up event: %d\n", event->repeat);
  }

  // Only process non-repeat key releases within valid range
  if (event->repeat == 0 && event->keysym.scancode < MAX_KEYBOARD_KEYS) {
    // Additional validation: ensure scancode is not negative
    if (event->keysym.scancode >= 0) {
      app.keyboard[event->keysym.scancode] = 0;
    } else {
      printf("Error: Negative scancode received: %d\n", event->keysym.scancode);
    }
  }
}

/*
 * Handle SDL mouse button events and update mouse state
 *
 * `button` - Pointer to SDL mouse button event structure
 *
 * -- Internal function called by a_DoInput() for mouse button events
 * -- Updates app.mouse structure with current button state, position, and click info
 * -- Stores button state (SDL_PRESSED or SDL_RELEASED) in app.mouse.state
 * -- Records which button was pressed/released in app.mouse.button
 * -- Tracks click count (single, double, triple clicks) in app.mouse.clicks
 * -- Updates mouse position (app.mouse.x, app.mouse.y) from event coordinates
 * -- Handles SDL_MOUSEBUTTONDOWN events
 * -- Mouse position is relative to the SDL window coordinate system
 * -- Does nothing if button parameter is NULL
 * -- Includes comprehensive error checking and defensive programming
 * -- Validates mouse coordinates and button values before processing
 */
static void a_DoMouseDown( SDL_MouseButtonEvent* button )
{
  // Defensive programming: Check for NULL pointer
  if (button == NULL) {
    printf("Error: NULL mouse button event passed to a_DoMouse\n");
    return;
  }

  // Validate mouse button range (SDL defines specific button constants)
  if (button->button < SDL_BUTTON_LEFT || button->button > SDL_BUTTON_X2) {
    printf("Warning: Invalid mouse button ID: %d\n", button->button);
    // Continue processing but log the warning
  }

  // Validate mouse state (should be SDL_PRESSED or SDL_RELEASED)
  if (button->state != SDL_PRESSED && button->state != SDL_RELEASED) {
    printf("Warning: Invalid mouse button state: %d\n", button->state);
    // Continue processing but log the warning
  }

  // Validate click count (reasonable range check)
  if (button->clicks < 0 || button->clicks > 10) {
    printf("Warning: Unusual click count: %d\n", button->clicks);
    // Continue processing but clamp the value
    if (button->clicks < 0) button->clicks = 0;
    if (button->clicks > 10) button->clicks = 10;
  }

  // Validate mouse coordinates (warn about extreme values but allow them)
  if (button->x < -10000 || button->x > 10000 ||
      button->y < -10000 || button->y > 10000) {
    printf("Warning: Extreme mouse coordinates: (%d, %d)\n", button->x, button->y);
    // Allow extreme coordinates but log them (might be valid for multi-monitor setups)
  }

  // Update mouse state (all checks passed or warnings logged)
  app.mouse.state  = button->state;
  app.mouse.button = 0;
  app.mouse.clicks = button->clicks;
  app.mouse.x = button->x;
  app.mouse.y = button->y;

  // Update pressed flag based on state
  app.mouse.pressed = (button->state == SDL_PRESSED) ? 1 : 0;
}

/*
 * Handle SDL mouse button events and update mouse state
 *
 * `button` - Pointer to SDL mouse button event structure
 *
 * -- Internal function called by a_DoInput() for mouse button events
 * -- Updates app.mouse structure with current button state, position, and click info
 * -- Stores button state (SDL_PRESSED or SDL_RELEASED) in app.mouse.state
 * -- Records which button was pressed/released in app.mouse.button
 * -- Tracks click count (single, double, triple clicks) in app.mouse.clicks
 * -- Updates mouse position (app.mouse.x, app.mouse.y) from event coordinates
 * -- Handles SDL_MOUSEBUTTONUP events
 * -- Mouse position is relative to the SDL window coordinate system
 * -- Does nothing if button parameter is NULL
 * -- Includes comprehensive error checking and defensive programming
 * -- Validates mouse coordinates and button values before processing
 */
static void a_DoMouseUp( SDL_MouseButtonEvent* button )
{
  // Defensive programming: Check for NULL pointer
  if (button == NULL) {
    printf("Error: NULL mouse button event passed to a_DoMouse\n");
    return;
  }

  // Validate mouse button range (SDL defines specific button constants)
  if (button->button < SDL_BUTTON_LEFT || button->button > SDL_BUTTON_X2) {
    printf("Warning: Invalid mouse button ID: %d\n", button->button);
    // Continue processing but log the warning
  }

  // Validate mouse state (should be SDL_PRESSED or SDL_RELEASED)
  if (button->state != SDL_PRESSED && button->state != SDL_RELEASED) {
    printf("Warning: Invalid mouse button state: %d\n", button->state);
    // Continue processing but log the warning
  }

  // Validate click count (reasonable range check)
  if (button->clicks < 0 || button->clicks > 10) {
    printf("Warning: Unusual click count: %d\n", button->clicks);
    // Continue processing but clamp the value
    if (button->clicks < 0) button->clicks = 0;
    if (button->clicks > 10) button->clicks = 10;
  }

  // Validate mouse coordinates (warn about extreme values but allow them)
  if (button->x < -10000 || button->x > 10000 ||
      button->y < -10000 || button->y > 10000) {
    printf("Warning: Extreme mouse coordinates: (%d, %d)\n", button->x, button->y);
    // Allow extreme coordinates but log them (might be valid for multi-monitor setups)
  }

  // Update mouse state (all checks passed or warnings logged)
  app.mouse.state  = button->state;
  app.mouse.button = button->button;
  app.mouse.clicks = button->clicks;
  app.mouse.x = button->x;
  app.mouse.y = button->y;

  // Update pressed flag based on state
  app.mouse.pressed = (button->state == SDL_PRESSED) ? 1 : 0;
}

/*
 * Handle SDL mouse wheel events and update wheel state
 *
 * `wheel` - Pointer to SDL mouse wheel event structure
 *
 * -- Internal function called by a_DoInput() for SDL_MOUSEWHEEL events
 * -- Updates app.mouse.wheel with vertical scroll direction and magnitude
 * -- Positive values indicate upward/away scrolling
 * -- Negative values indicate downward/toward scrolling
 * -- Wheel value persists until next wheel event or manual reset
 * -- Commonly used pattern: check wheel value, process it, then reset to 0
 * -- Does nothing if wheel parameter is NULL
 * -- Horizontal wheel scrolling (wheel->x) is currently not processed
 * -- Includes comprehensive error checking and defensive programming
 * -- Validates wheel values and provides warnings for extreme values
 */
static void a_DoMouseWheel( SDL_MouseWheelEvent* wheel )
{
  // Defensive programming: Check for NULL pointer
  if (wheel == NULL) {
    printf("Error: NULL mouse wheel event passed to a_DoMouseWheel\n");
    return;
  }

  // Validate wheel direction values (reasonable range check)
  if (wheel->y < -1 || wheel->y > 1) {
    printf("Warning: Extreme wheel scroll value: %d\n", wheel->y);
    // Allow extreme values but log them (might be valid for some hardware)
  }

  // Log horizontal wheel events for debugging (currently not used)
  if (wheel->x != 0) {
    printf("Info: Horizontal wheel scroll detected but not processed: %d\n", wheel->x);
  }

  // Update wheel state
  app.mouse.wheel = wheel->y;
}

static void a_DoMouseMotion( SDL_MouseMotionEvent* motion )
{
  app.mouse.x = motion->x;
  app.mouse.y = motion->y;
}
