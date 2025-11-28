/**
 * player_actions.c - Player movement and combat
 *
 * Handles player character movement, shooting, and bullet physics.
 */

#include <stdio.h>
#include <math.h>
#include "Archimedes.h"
#include "player_actions.h"

// ============================================================================
// Player State
// ============================================================================

// Player position (use float for smooth movement)
static float player_x = 100.0f;
static float player_y = 100.0f;
static float player_speed = 200.0f; // pixels per second

// ============================================================================
// Bullet System
// ============================================================================

#define MAX_BULLETS 100

typedef struct {
  float x, y;       // Position
  float vx, vy;     // Velocity
  int active;       // Is this bullet alive?
} Bullet_t;

static Bullet_t bullets[MAX_BULLETS];
static float bullet_speed = 400.0f; // pixels per second

// ============================================================================
// Initialization
// ============================================================================

void player_init(void)
{
  player_x = 100.0f;
  player_y = 100.0f;

  // Clear all bullets
  for (int i = 0; i < MAX_BULLETS; i++) {
    bullets[i].active = 0;
  }
}

// ============================================================================
// Shooting
// ============================================================================

void player_shoot(int target_x, int target_y)
{
  // Find first inactive bullet slot
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (!bullets[i].active)
    {
      // Calculate direction from player to target
      float dx = target_x - player_x;
      float dy = target_y - player_y;
      float distance = sqrtf(dx * dx + dy * dy);

      // Normalize direction and set velocity
      if (distance > 0)
      {
        // Start from center of player (32x32, so center is at +16)
        // Offset by half bullet size to center it
        bullets[i].x = player_x + 16 - 4; // Center 8x8 bullet on player center
        bullets[i].y = player_y + 16 - 4;
        bullets[i].vx = (dx / distance) * bullet_speed;
        bullets[i].vy = (dy / distance) * bullet_speed;
        bullets[i].active = 1;
        printf("Fired bullet %d toward (%d, %d)\n", i, target_x, target_y);
      }
      break;
    }
  }
}

// ============================================================================
// Update
// ============================================================================

void player_update(float dt)
{
  // Move player with arrow keys (speed is pixels per second)
  float dx = 0.0f;
  float dy = 0.0f;

  if (app.keyboard[SDL_SCANCODE_UP])    dy -= 1.0f;
  if (app.keyboard[SDL_SCANCODE_DOWN])  dy += 1.0f;
  if (app.keyboard[SDL_SCANCODE_LEFT])  dx -= 1.0f;
  if (app.keyboard[SDL_SCANCODE_RIGHT]) dx += 1.0f;

  // Shoot bullet on left click (check when button is pressed)
  if (app.mouse.button == SDL_BUTTON_LEFT && app.mouse.pressed == 1)
  {
    player_shoot(app.mouse.x, app.mouse.y);
    app.mouse.button = 0; // Prevent repeated firing
  }

  // Normalize diagonal movement
  if (dx != 0.0f && dy != 0.0f)
  {
    float diagonal = 0.7071f; // 1/sqrt(2)
    dx *= diagonal;
    dy *= diagonal;
  }

  player_x += dx * player_speed * dt;
  player_y += dy * player_speed * dt;

  // Update all active bullets
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active)
    {
      bullets[i].x += bullets[i].vx * dt;
      bullets[i].y += bullets[i].vy * dt;

      // Deactivate bullets that go off screen
      if (bullets[i].x < 0 || bullets[i].x > SCREEN_WIDTH ||
          bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT)
      {
        bullets[i].active = 0;
      }
    }
  }
}

// ============================================================================
// Collision - exposed for enemy system to use
// ============================================================================

int player_check_bullet_collision(float enemy_x, float enemy_y, float enemy_radius)
{
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active)
    {
      float dx = (bullets[i].x + 4) - (enemy_x + enemy_radius);
      float dy = (bullets[i].y + 4) - (enemy_y + enemy_radius);
      float dist = sqrtf(dx * dx + dy * dy);

      // Hit detected (bullet radius 4 + enemy radius)
      if (dist < (4.0f + enemy_radius))
      {
        bullets[i].active = 0; // Destroy bullet
        return i; // Return bullet index that hit
      }
    }
  }
  return -1; // No hit
}

// ============================================================================
// Drawing
// ============================================================================

void player_draw(aImage_t* img)
{
  // Draw the player square
  a_DrawFilledRect((aRectf_t){player_x, player_y, 32, 32}, (aColor_t){0, 0, 255, 255});

  // Draw all active bullets (8x8 blitted from surface)
  for (int i = 0; i < MAX_BULLETS; i++)
  {
    if (bullets[i].active)
    {
      aRectf_t dest = (aRectf_t){ bullets[i].x, bullets[i].y, img->surface->w, img->surface->h };
      aRectf_t src = (aRectf_t){0, 0, img->surface->w, img->surface->h};
      a_BlitRect(img, &src, &dest, 0.25);
    }
  }
}

/*void player_draw(aImage_t* img)
{
  // Draw the player square
  a_DrawFilledRect((aRectf_t){player_x, player_y, 32, 32}, (aColor_t){0, 0, 255, 255});

  // Draw all active bullets at 25% size
  if (img != NULL && img->surface != NULL)
  {
    float img_w = (float)img->surface->w;
    float img_h = (float)img->surface->h;
    float scaled_w = img_w * 0.25f;
    float scaled_h = img_h * 0.25f;

    for (int i = 0; i < MAX_BULLETS; i++)
    {
      if (bullets[i].active)
      {
        aRectf_t src = {0, 0, img_w, img_h};
        aRectf_t dest = {bullets[i].x, bullets[i].y, scaled_w, scaled_h};

        // scale = 1 because dest already has the scaled size
        a_BlitRect(img, &src, &dest, 1);
      }
    }
  }
}*/

// ============================================================================
// Getters (for enemy targeting)
// ============================================================================

float player_get_x(void)
{
  return player_x + 16; // Return center X
}

float player_get_y(void)
{
  return player_y + 16; // Return center Y
}
