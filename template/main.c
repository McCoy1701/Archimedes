#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Archimedes.h"

static void aDoLoop( float );
static void aRenderLoop( float );
static void shootbullet( int x, int y );

SDL_Surface* surf;

// Square position (use float for smooth movement)
static float square_x = 100.0f;
static float square_y = 100.0f;
static float speed = 200.0f; // pixels per second

// Bullet system
#define MAX_BULLETS 100
typedef struct {
  float x, y;       // Position
  float vx, vy;     // Velocity
  int active;       // Is this bullet alive?
} Bullet_t;

static Bullet_t bullets[MAX_BULLETS];
static float bullet_speed = 400.0f; // pixels per second

// Timer system (countdown from 15:00)
static float time_remaining = 15.0f * 60.0f; // 15 minutes in seconds

// Enemy system
#define MAX_ENEMIES 50
typedef struct {
  float x, y;       // Position
  float vx, vy;     // Velocity
  float target_angle; // Offset angle around player
  float next_retarget; // Time until next retarget
  int active;       // Is this enemy alive?
  int hit_count;    // Number of times hit (for color)
} Enemy_t;

static Enemy_t enemies[MAX_ENEMIES];
static float enemy_speed = 100.0f; // pixels per second
static float spawn_timer = 0.0f;
static float spawn_interval = 2.0f; // spawn every 2 seconds

// Swarm parameters
static float orbit_distance = 40.0f;  // How far from player they try to stay
static float retarget_interval = 1.5f; // How often they pick new angle
static float collision_radius = 16.0f; // Enemy size for collision
static float separation_force_radius = 25.0f; // Distance to apply separation force
static float separation_strength = 150.0f; // Strength of separation force

void aInitGame( void )
{
  app.delegate.logic = aDoLoop;
  app.delegate.draw  = aRenderLoop;

  // Set background color (dark blue)
  app.background = (aColor_t){20, 20, 60, 255};

  surf = a_Image( "resources/assets/bullet.png" );
  if ( surf == NULL )
  {
    printf( "Failed to load image\n" );
  }
}

static void shootbullet( int x, int y )
{
  // Find first inactive bullet slot
  for ( int i = 0; i < MAX_BULLETS; i++ )
  {
    if ( !bullets[i].active )
    {
      // Calculate direction from square to mouse click
      float dx = x - square_x;
      float dy = y - square_y;
      float distance = sqrtf( dx * dx + dy * dy );

      // Normalize direction and set velocity
      if ( distance > 0 )
      {
        // Start from center of square (32x32, so center is at +16)
        // Offset by half bullet size to center it
        bullets[i].x = square_x + 16 - 4; // Center 8x8 bullet on square center
        bullets[i].y = square_y + 16 - 4;
        bullets[i].vx = (dx / distance) * bullet_speed;
        bullets[i].vy = (dy / distance) * bullet_speed;
        bullets[i].active = 1;
        printf("Fired bullet %d toward (%d, %d)\n", i, x, y);
      }
      break;
    }
  }
}

static void aDoLoop( float dt )
{
  a_DoInput();

  // Move square with arrow keys (speed is pixels per second)
  float dx = 0.0f;
  float dy = 0.0f;

  if ( app.keyboard[ SDL_SCANCODE_UP ] )    dy -= 1.0f;
  if ( app.keyboard[ SDL_SCANCODE_DOWN ] )  dy += 1.0f;
  if ( app.keyboard[ SDL_SCANCODE_LEFT ] )  dx -= 1.0f;
  if ( app.keyboard[ SDL_SCANCODE_RIGHT ] ) dx += 1.0f;

  // Shoot bullet on left click (check when button is released)
  if ( app.mouse.button == SDL_BUTTON_LEFT && app.mouse.pressed == 0 )
  {
    shootbullet( app.mouse.x, app.mouse.y );
    app.mouse.button = 0; // Prevent repeated firing
  }

  // Normalize diagonal movement
  if ( dx != 0.0f && dy != 0.0f )
  {
    float diagonal = 0.7071f; // 1/sqrt(2)
    dx *= diagonal;
    dy *= diagonal;
  }

  square_x += dx * speed * dt;
  square_y += dy * speed * dt;

  // Update timer (countdown)
  time_remaining -= dt;
  if ( time_remaining < 0.0f )
  {
    time_remaining = 0.0f;
  }

  // Spawn enemies
  spawn_timer += dt;
  if ( spawn_timer >= spawn_interval )
  {
    spawn_timer = 0.0f;

    // Find inactive enemy slot
    for ( int i = 0; i < MAX_ENEMIES; i++ )
    {
      if ( !enemies[i].active )
      {
        // Random spawn position outside screen
        int side = rand() % 4; // 0=top, 1=right, 2=bottom, 3=left

        if ( side == 0 ) // Top
        {
          enemies[i].x = RANDF( 0, SCREEN_WIDTH );
          enemies[i].y = -16;
        }
        else if ( side == 1 ) // Right
        {
          enemies[i].x = SCREEN_WIDTH + 16;
          enemies[i].y = RANDF( 0, SCREEN_HEIGHT );
        }
        else if ( side == 2 ) // Bottom
        {
          enemies[i].x = RANDF( 0, SCREEN_WIDTH );
          enemies[i].y = SCREEN_HEIGHT + 16;
        }
        else // Left
        {
          enemies[i].x = -16;
          enemies[i].y = RANDF( 0, SCREEN_HEIGHT );
        }

        // Calculate direction toward player
        float dx = (square_x + 16) - enemies[i].x;
        float dy = (square_y + 16) - enemies[i].y;
        float distance = sqrtf( dx * dx + dy * dy );

        if ( distance > 0 )
        {
          enemies[i].vx = (dx / distance) * enemy_speed;
          enemies[i].vy = (dy / distance) * enemy_speed;
          enemies[i].active = 1;
          enemies[i].target_angle = RANDF( 0, 2.0f * M_PI ); // Random angle
          enemies[i].next_retarget = retarget_interval;
          enemies[i].hit_count = 0;
        }
        break;
      }
    }
  }

  // Update all active bullets
  for ( int i = 0; i < MAX_BULLETS; i++ )
  {
    if ( bullets[i].active )
    {
      bullets[i].x += bullets[i].vx * dt;
      bullets[i].y += bullets[i].vy * dt;

      // Check collision with enemies
      for ( int j = 0; j < MAX_ENEMIES; j++ )
      {
        if ( enemies[j].active )
        {
          float dx = (bullets[i].x + 4) - (enemies[j].x + 8); // Bullet center to enemy center
          float dy = (bullets[i].y + 4) - (enemies[j].y + 8);
          float dist = sqrtf( dx * dx + dy * dy );

          // Hit detected (bullet radius 4 + enemy radius 8)
          if ( dist < 12.0f )
          {
            printf( "Enemy struck!\n" );
            enemies[j].hit_count++;
            bullets[i].active = 0; // Destroy bullet
            break;
          }
        }
      }

      // Deactivate bullets that go off screen
      if ( bullets[i].x < 0 || bullets[i].x > SCREEN_WIDTH ||
           bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT )
      {
        bullets[i].active = 0;
      }
    }
  }

  // Update all active enemies with offset target system
  for ( int i = 0; i < MAX_ENEMIES; i++ )
  {
    if ( enemies[i].active )
    {
      // Update retarget timer
      enemies[i].next_retarget -= dt;
      if ( enemies[i].next_retarget <= 0.0f )
      {
        enemies[i].target_angle = RANDF( 0, 2.0f * M_PI );
        enemies[i].next_retarget = retarget_interval;
      }

      // Calculate offset target position around player
      float player_center_x = square_x + 16;
      float player_center_y = square_y + 16;
      float target_x = player_center_x + cosf( enemies[i].target_angle ) * orbit_distance;
      float target_y = player_center_y + sinf( enemies[i].target_angle ) * orbit_distance;

      // Calculate separation force from nearby enemies
      float sep_x = 0.0f;
      float sep_y = 0.0f;
      int sep_count = 0;

      for ( int j = 0; j < MAX_ENEMIES; j++ )
      {
        if ( i != j && enemies[j].active )
        {
          float ex = enemies[i].x - enemies[j].x;
          float ey = enemies[i].y - enemies[j].y;
          float edist = sqrtf( ex * ex + ey * ey );

          // Apply strong separation force if too close
          if ( edist < separation_force_radius && edist > 0.1f )
          {
            // Inverse square law - much stronger when closer
            float force = 1.0f / (edist * edist);
            sep_x += (ex / edist) * force;
            sep_y += (ey / edist) * force;
            sep_count++;
          }
        }
      }

      // Average separation force
      if ( sep_count > 0 )
      {
        sep_x /= sep_count;
        sep_y /= sep_count;
      }

      // Chase the offset target
      float dx = target_x - (enemies[i].x + 8);
      float dy = target_y - (enemies[i].y + 8);
      float dist = sqrtf( dx * dx + dy * dy );

      // Normalize chase direction
      float chase_x = 0.0f;
      float chase_y = 0.0f;
      if ( dist > 0.1f )
      {
        chase_x = dx / dist;
        chase_y = dy / dist;
      }

      // Combine chase + separation forces
      float total_x = (chase_x * enemy_speed) + (sep_x * separation_strength);
      float total_y = (chase_y * enemy_speed) + (sep_y * separation_strength);

      // Normalize and apply speed
      float total_dist = sqrtf( total_x * total_x + total_y * total_y );
      if ( total_dist > 0.1f )
      {
        enemies[i].vx = (total_x / total_dist) * enemy_speed;
        enemies[i].vy = (total_y / total_dist) * enemy_speed;
      }

      // Move enemy
      enemies[i].x += enemies[i].vx * dt;
      enemies[i].y += enemies[i].vy * dt;

      // Player collision detection - prevent enemies from overlapping player
      float player_dx = (enemies[i].x + 8) - (square_x + 16);
      float player_dy = (enemies[i].y + 8) - (square_y + 16);
      float player_dist = sqrtf( player_dx * player_dx + player_dy * player_dy );

      float min_distance = 24.0f; // 16 (enemy radius) + 16 (player radius) - 8 overlap allowance
      if ( player_dist < min_distance && player_dist > 0.1f )
      {
        // Push enemy away from player
        float pushback = min_distance - player_dist;
        enemies[i].x += (player_dx / player_dist) * pushback;
        enemies[i].y += (player_dy / player_dist) * pushback;
      }

      // Hard collision resolution - push overlapping enemies apart
      for ( int j = i + 1; j < MAX_ENEMIES; j++ )
      {
        if ( enemies[j].active )
        {
          float ex = enemies[i].x - enemies[j].x;
          float ey = enemies[i].y - enemies[j].y;
          float edist = sqrtf( ex * ex + ey * ey );

          // If overlapping, push them apart instantly
          if ( edist < collision_radius && edist > 0.1f )
          {
            float overlap = collision_radius - edist;
            float push_x = (ex / edist) * (overlap / 2.0f);
            float push_y = (ey / edist) * (overlap / 2.0f);

            enemies[i].x += push_x;
            enemies[i].y += push_y;
            enemies[j].x -= push_x;
            enemies[j].y -= push_y;
          }
        }
      }

      // Deactivate enemies that go too far off screen
      if ( enemies[i].x < -100 || enemies[i].x > SCREEN_WIDTH + 100 ||
           enemies[i].y < -100 || enemies[i].y > SCREEN_HEIGHT + 100 )
      {
        enemies[i].active = 0;
      }
    }
  }

  if ( app.mouse.wheel == 1 )
  {
    printf( "scroll up\n" );
    app.mouse.wheel = 0;
  }

  if ( app.mouse.wheel == -1 )
  {
    printf( "scroll down\n" );
    app.mouse.wheel = 0;
  }

  //a_DoWidget(); // Disabled - not using widgets

  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    app.running = 0;
  }
}

static void aRenderLoop( float dt )
{
  // Draw countdown timer
  int minutes = (int)(time_remaining / 60.0f);
  int seconds = (int)time_remaining % 60;
  char timer_text[32];
  snprintf( timer_text, sizeof(timer_text), "%02d:%02d", minutes, seconds );

  aFontConfig_t timer_config = {
    .type = FONT_ENTER_COMMAND,
    .color = {255, 255, 255, 255},
    .align = TEXT_ALIGN_CENTER,
    .wrap_width = 0,
    .scale = 1.0f
  };
  a_DrawTextStyled( timer_text, SCREEN_WIDTH / 2, 25, &timer_config );

  // Draw the player square
  a_DrawFilledRect( (int)square_x, (int)square_y, 32, 32, 0, 0, 255, 255 );

  // Draw all active bullets (scaled down to 8x8)
  SDL_Texture* bullet_tex = a_ToTexture( surf, 0 ); // Don't destroy surface
  for ( int i = 0; i < MAX_BULLETS; i++ )
  {
    if ( bullets[i].active )
    {
      a_BlitTextureScaled( bullet_tex, (int)bullets[i].x, (int)bullets[i].y, 8, 8 );
    }
  }
  SDL_DestroyTexture( bullet_tex );

  // Draw all active enemies - color changes with hits
  for ( int i = 0; i < MAX_ENEMIES; i++ )
  {
    if ( enemies[i].active )
    {
      // Calculate color based on hit count (more hits = more red)
      int red = 255;
      int green = 255 - ( enemies[i].hit_count * 50 ); // Decrease green with hits
      int blue = 255 - ( enemies[i].hit_count * 50 );  // Decrease blue with hits

      // Clamp values
      if ( green < 0 ) green = 0;
      if ( blue < 0 ) blue = 0;

      a_DrawFilledRect( (int)enemies[i].x, (int)enemies[i].y, 16, 16, red, green, blue, 255 );
    }
  }

  a_DrawWidgets();
}

void aMainloop( void )
{
  a_PrepareScene();

  app.delegate.logic( a_GetDeltaTime() );
  app.delegate.draw( a_GetDeltaTime() );
  
  a_PresentScene();
}

int main( void )
{
  a_Init( SCREEN_WIDTH, SCREEN_HEIGHT, "Archimedes" );

  aInitGame();

  #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop( aMainloop, 0, 1 );
  #endif

  #ifndef __EMSCRIPTEN__
    while( app.running ) {
      aMainloop();
    }
  #endif

  a_Quit();

  return 0;
}