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

/**
 * @brief Processes all pending SDL events and dispatches them to appropriate handlers.
 *
 * This function continuously polls for SDL events (keyboard, mouse, quit, text input)
 * using `SDL_PollEvent` and dispatches them to internal static functions (`a_DoKeyDown`,
 * `a_DoKeyUp`, `a_DoMouseDown`, `a_DoMouseUp`, `a_DoMouseWheel`, `a_DoMouseMotion`).
 * It updates the global `app` state based on these events, managing the game's input.
 * Unknown event types are silently ignored.
 *
 * @note The `app.running` flag is set to `0` if an `SDL_QUIT` event is received.
 * @note For `SDL_TEXTINPUT` events, the input text is copied to `app.input_text`
 * if its length is valid (between 1 and `MAX_INPUT_LENGTH - 1`).
 *
 * @see a_DoKeyDown
 * @see a_DoKeyUp
 * @see a_DoMouseDown
 * @see a_DoMouseUp
 * @see a_DoMouseWheel
 * @see a_DoMouseMotion
 */
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

/**
 * @brief Handles SDL keyboard key press events and updates the global keyboard state.
 *
 * This internal function is called by `a_DoInput()` when an `SDL_KEYDOWN` event occurs.
 * It sets the corresponding scancode in the `app.keyboard` array to `1` (pressed)
 * and records the `last_key_pressed`. Key repeat events (when a key is held down)
 * are explicitly ignored to provide clean, single-press detection for game logic.
 *
 * @param event Pointer to the `SDL_KeyboardEvent` structure containing key press details.
 *
 * @note Updates `app.keyboard[event->keysym.scancode] = 1`.
 * @note Records `event->keysym.scancode` in `app.last_key_pressed`.
 * @note Ignores key repeat events (`event->repeat != 0`).
 * @note Only processes scancodes within the valid range (`0` to `MAX_KEYBOARD_KEYS - 1`).
 * @note Includes defensive checks for `NULL` event pointer, scancode bounds, and unusual repeat flag values.
 * @warning Scancodes outside `[0, MAX_KEYBOARD_KEYS-1]` will result in a warning and be ignored.
 */
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

/**
 * @brief Handles SDL keyboard key release events and updates the global keyboard state.
 *
 * @details This internal function is called by `a_DoInput()` when an `SDL_KEYUP` event occurs.
 * It sets the corresponding scancode in the `app.keyboard` array to `0` (released).
 * Like `a_DoKeyDown`, key repeat events are ignored for consistency in input detection.
 *
 * @param event Pointer to the `SDL_KeyboardEvent` structure containing key release details.
 *
 * @note Updates `app.keyboard[event->keysym.scancode] = 0`.
 * @note Ignores key repeat events (`event->repeat != 0`).
 * @note Only processes scancodes within the valid range (`0` to `MAX_KEYBOARD_KEYS - 1`).
 * @note Includes defensive checks for `NULL` event pointer, scancode bounds, and unusual repeat flag values.
 * @warning Scancodes outside `[0, MAX_KEYBOARD_KEYS-1]` will result in a warning and be ignored.
 */
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

/**
 * @brief Handles SDL mouse button press events and updates the global mouse state.
 *
 * @details This internal function is called by `a_DoInput()` when an `SDL_MOUSEBUTTONDOWN` event occurs.
 * It updates the `app.mouse` structure with the button's `SDL_PRESSED` state,
 * the number of clicks (`button->clicks`), and the mouse cursor's `x` and `y` coordinates
 * relative to the window.
 *
 * @param button Pointer to the `SDL_MouseButtonEvent` structure containing button press details.
 *
 * @note Sets `app.mouse.state` to `SDL_PRESSED`.
 * @note Sets `app.mouse.button` to `0` (as per current code logic for DOWN events).
 * @note Updates `app.mouse.clicks` with the event's click count.
 * @note Updates `app.mouse.x` and `app.mouse.y` with the mouse cursor's position.
 * @note Sets `app.mouse.pressed` to `1`.
 * @note Includes defensive checks for `NULL` event pointer, valid button IDs, and reasonable coordinate/click values.
 */
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

/**
 * @brief Handles SDL mouse button release events and updates the global mouse state.
 *
 * @details This internal function is called by `a_DoInput()` when an `SDL_MOUSEBUTTONUP` event occurs.
 * It updates the `app.mouse` structure with the button's `SDL_RELEASED` state,
 * the specific `button->button` ID, the number of clicks (`button->clicks`),
 * and the mouse cursor's `x` and `y` coordinates relative to the window.
 *
 * @param button Pointer to the `SDL_MouseButtonEvent` structure containing button release details.
 *
 * @note Sets `app.mouse.state` to `SDL_RELEASED`.
 * @note Records the specific `event->button` ID in `app.mouse.button`.
 * @note Updates `app.mouse.clicks` with the event's click count.
 * @note Updates `app.mouse.x` and `app.mouse.y` with the mouse cursor's position.
 * @note Sets `app.mouse.pressed` to `0`.
 * @note Includes defensive checks for `NULL` event pointer, valid button IDs, and reasonable coordinate/click values.
 */
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

/**
 * @brief Handles SDL mouse wheel events and updates the global wheel state.
 *
 * @param wheel Pointer to the `SDL_MouseWheelEvent` structure containing wheel scroll details.
 *
 * @note Updates `app.mouse.wheel` with the vertical scroll amount (`wheel->y`).
 * @note Horizontal wheel scrolling (`wheel->x`) is detected and logged, but its value is not stored in `app.mouse`.
 * @note The `app.mouse.wheel` value persists until the next wheel event or until manually reset to `0`
 * by the application logic after consumption.
 * @note Includes defensive checks for `NULL` event pointer and reasonable wheel values.
 */
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

/**
 * @brief Handles SDL mouse motion events and updates the global mouse coordinates.
 *
 * @param motion Pointer to the `SDL_MouseMotionEvent` structure containing motion details.
 *
 * @note Updates `app.mouse.x` and `app.mouse.y` with the absolute mouse coordinates.
 * @note This function is typically called very frequently during mouse movement.
 */
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