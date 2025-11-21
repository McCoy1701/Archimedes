#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Archimedes.h"
#include "test_text.h"
#include "player_actions.h"

// Scene system
typedef enum {
  SCENE_GAME,
  SCENE_TEST_TEXT
} Scene_t;

static Scene_t current_scene = SCENE_GAME;
static int esc_pressed = 0;  // Debounce ESC

// Scene function declarations
static void scene_game_logic( float dt );
static void scene_game_draw( float dt );

static void aDoLoop( float );
static void aRenderLoop( float );

SDL_Surface* surf;

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

  // Initialize player
  player_init();
}

static void aDoLoop( float dt )
{
  a_DoInput();

  // Dispatch to current scene
  switch ( current_scene )
  {
    case SCENE_GAME:
      scene_game_logic( dt );
      break;
    case SCENE_TEST_TEXT:
      if ( test_text_logic( dt ) )
      {
        current_scene = SCENE_GAME;
      }
      break;
  }
}

static void scene_game_logic( float dt )
{
  // Update player movement and shooting
  player_update( dt );

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
        float dx = player_get_x() - enemies[i].x;
        float dy = player_get_y() - enemies[i].y;
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

  // Check bullet collisions with enemies
  for ( int j = 0; j < MAX_ENEMIES; j++ )
  {
    if ( enemies[j].active )
    {
      int bullet_hit = player_check_bullet_collision( enemies[j].x, enemies[j].y, 8.0f );
      if ( bullet_hit >= 0 )
      {
        printf( "Enemy struck by bullet %d!\n", bullet_hit );
        enemies[j].hit_count++;
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
      float player_center_x = player_get_x();
      float player_center_y = player_get_y();
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
      float player_dx = (enemies[i].x + 8) - player_get_x();
      float player_dy = (enemies[i].y + 8) - player_get_y();
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

  // Ctrl+T to switch to test scene
  static int ctrl_t_pressed = 0;
  if ( (app.keyboard[ SDL_SCANCODE_LCTRL ] || app.keyboard[ SDL_SCANCODE_RCTRL ]) &&
       app.keyboard[ SDL_SCANCODE_T ] == 1 && !ctrl_t_pressed )
  {
    current_scene = SCENE_TEST_TEXT;
    ctrl_t_pressed = 1;
    app.keyboard[ SDL_SCANCODE_T ] = 0;
  }
  if ( app.keyboard[ SDL_SCANCODE_T ] == 0 )
  {
    ctrl_t_pressed = 0;
  }

  // ESC to quit
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    app.running = 0;
  }
}

static void aRenderLoop( float dt )
{
  // Dispatch to current scene
  switch ( current_scene )
  {
    case SCENE_GAME:
      scene_game_draw( dt );
      break;
    case SCENE_TEST_TEXT:
      test_text_draw( dt );
      break;
  }

  a_DrawWidgets();
}

static void scene_game_draw( float dt )
{
  (void)dt;

  // Draw countdown timer
  int minutes = (int)(time_remaining / 60.0f);
  int seconds = (int)time_remaining % 60;
  char timer_text[32];
  snprintf( timer_text, sizeof(timer_text), "%02d:%02d", minutes, seconds );

  aTextStyle_t timer_config = {
    .type = FONT_ENTER_COMMAND,
    .fg = {255, 255, 255, 255},
    .align = TEXT_ALIGN_CENTER,
    .wrap_width = 0,
    .scale = 0.8f
  };
  a_DrawText( timer_text, SCREEN_WIDTH / 2, 25, &timer_config );

  // Draw player and bullets
  player_draw( surf );

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

      a_DrawFilledRect( (aRectf_t){enemies[i].x, enemies[i].y, 16, 16}, (aColor_t){red, green, blue, 255} );
    }
  }
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