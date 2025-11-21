#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Archimedes.h"

// Scene system
typedef enum {
  SCENE_GAME,
  SCENE_TEST_TEXT
} Scene_t;

static Scene_t current_scene = SCENE_GAME;
static int esc_pressed = 0;  // Debounce ESC
static int test_index = 0;  // Shared index for Test 5, 6, 7 (0-11)

// Test 6: 12 alpha values
static const int test6_alphas[12] = { 0, 25, 50, 75, 100, 125, 150, 175, 200, 225, 240, 255 };

// Test 7: Random colors for 4 padding boxes (randomized on -/+ press)
static int test7_fg_idx[4] = {0, 1, 2, 3};
static int test7_bg_idx[4] = {4, 5, 6, 7};

// Test 5: 12 color combinations (using custom palette)
static const char* test5_labels[12] = {
  "Cream on Ocean", "Coral on Dark", "Lime on Forest", "Gold on Maroon",
  "Cyan on Navy", "Pink on Purple", "Peach on Brown", "Sage on Teal",
  "Rose on Wine", "Sky on Deep", "Cream on Charcoal", "Amber on Rust"
};
static const aColor_t test5_fg[12] = {
  {0xeb,0xed,0xe9,255}, {0xcf,0x57,0x3c,255}, {0xa8,0xca,0x58,255}, {0xe8,0xc1,0x70,255},
  {0x73,0xbe,0xd3,255}, {0xdf,0x84,0xa5,255}, {0xd7,0xb5,0x94,255}, {0xa8,0xb5,0xb2,255},
  {0xda,0x86,0x3e,255}, {0x4f,0x8f,0xba,255}, {0xc7,0xcf,0xcc,255}, {0xde,0x9e,0x41,255}
};
static const aColor_t test5_bg[12] = {
  {0x3c,0x5e,0x8b,255}, {0x10,0x14,0x1f,255}, {0x19,0x33,0x2d,255}, {0x60,0x2c,0x2c,255},
  {0x1e,0x1d,0x39,255}, {0x40,0x27,0x51,255}, {0x4d,0x2b,0x32,255}, {0x25,0x56,0x2e,255},
  {0x75,0x24,0x38,255}, {0x15,0x1d,0x28,255}, {0x39,0x4a,0x50,255}, {0x88,0x4b,0x2b,255}
};

// FlexBox layout for test scene
static FlexBox_t* test_left_col = NULL;
static FlexBox_t* test_right_col = NULL;
static int test_layout_initialized = 0;

// Layout constants
#define TEST_TITLE_HEIGHT 30
#define TEST_CONTENT_HEIGHT 40
#define TEST_GROUP_GAP 15
#define TEST_TITLE_CONTENT_GAP 8

static void init_test_layout( void )
{
  if ( test_layout_initialized ) return;

  int col_width = SCREEN_WIDTH / 2 - 30;

  // Left column: Tests 1-4
  test_left_col = a_CreateFlexBox( 10, 100, col_width, SCREEN_HEIGHT - 120 );
  a_FlexSetDirection( test_left_col, FLEX_DIR_COLUMN );
  a_FlexSetGap( test_left_col, TEST_GROUP_GAP );

  // Add 4 test groups (each group = title + content height)
  for ( int i = 0; i < 4; i++ ) {
    a_FlexAddItem( test_left_col, col_width, TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP + TEST_CONTENT_HEIGHT, NULL );
  }

  // Right column: Tests 5-10
  test_right_col = a_CreateFlexBox( SCREEN_WIDTH / 2 + 20, 100, col_width, SCREEN_HEIGHT - 120 );
  a_FlexSetDirection( test_right_col, FLEX_DIR_COLUMN );
  a_FlexSetGap( test_right_col, TEST_GROUP_GAP );

  // Add 6 test groups
  for ( int i = 0; i < 6; i++ ) {
    a_FlexAddItem( test_right_col, col_width, TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP + TEST_CONTENT_HEIGHT, NULL );
  }

  a_FlexLayout( test_left_col );
  a_FlexLayout( test_right_col );

  test_layout_initialized = 1;
}

static void cleanup_test_layout( void )
{
  if ( test_left_col ) {
    a_DestroyFlexBox( &test_left_col );
  }
  if ( test_right_col ) {
    a_DestroyFlexBox( &test_right_col );
  }
  test_layout_initialized = 0;
}

// Scene function declarations
static void scene_game_logic( float dt );
static void scene_game_draw( float dt );
static void scene_test_text_logic( float dt );
static void scene_test_text_draw( float dt );

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

  // Dispatch to current scene
  switch ( current_scene )
  {
    case SCENE_GAME:
      scene_game_logic( dt );
      break;
    case SCENE_TEST_TEXT:
      scene_test_text_logic( dt );
      break;
  }
}

static void scene_game_logic( float dt )
{
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

  // ESC to switch to test scene (with debounce)
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 && !esc_pressed )
  {
    current_scene = SCENE_TEST_TEXT;
    esc_pressed = 1;
  }
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 0 )
  {
    esc_pressed = 0;
  }
}

// ============================================================================
// Test Text Scene - for testing text rendering and glyph fallback
// ============================================================================

static void scene_test_text_logic( float dt )
{
  (void)dt;  // unused
  static int minus_pressed = 0;
  static int plus_pressed = 0;

  // ESC to return to game (with debounce)
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 && !esc_pressed )
  {
    cleanup_test_layout();
    current_scene = SCENE_GAME;
    esc_pressed = 1;
  }
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 0 )
  {
    esc_pressed = 0;
  }

  // Q to quit
  if ( app.keyboard[ SDL_SCANCODE_Q ] == 1 )
  {
    app.running = 0;
  }

  // Unified controls: - and = keys control Test 5, 6, and 7
  if ( app.keyboard[ SDL_SCANCODE_MINUS ] == 1 && !minus_pressed )
  {
    test_index--;
    if ( test_index < 0 ) test_index = 11;
    // Randomize Test 7 colors
    for ( int i = 0; i < 4; i++ ) {
      test7_fg_idx[i] = rand() % 12;
      test7_bg_idx[i] = rand() % 12;
    }
    minus_pressed = 1;
  }
  if ( app.keyboard[ SDL_SCANCODE_MINUS ] == 0 ) minus_pressed = 0;

  if ( app.keyboard[ SDL_SCANCODE_EQUALS ] == 1 && !plus_pressed )
  {
    test_index++;
    if ( test_index > 11 ) test_index = 0;
    // Randomize Test 7 colors
    for ( int i = 0; i < 4; i++ ) {
      test7_fg_idx[i] = rand() % 12;
      test7_bg_idx[i] = rand() % 12;
    }
    plus_pressed = 1;
  }
  if ( app.keyboard[ SDL_SCANCODE_EQUALS ] == 0 ) plus_pressed = 0;
}

static void scene_test_text_draw( float dt )
{
  (void)dt;

  // Initialize layout if needed
  init_test_layout();

  // Get layout positions
  int col_width = SCREEN_WIDTH / 2 - 30;
  const FlexItem_t* left_items[4];
  const FlexItem_t* right_items[6];
  for ( int i = 0; i < 4; i++ ) left_items[i] = a_FlexGetItem( test_left_col, i );
  for ( int i = 0; i < 6; i++ ) right_items[i] = a_FlexGetItem( test_right_col, i );

  // Title
  aTextStyle_t title_config = {
    .type = FONT_ENTER_COMMAND,
    .fg = {255, 255, 0, 255},
    .bg = {0, 0, 0, 0},
    .align = TEXT_ALIGN_CENTER,
    .wrap_width = 0,
    .scale = 0.8f,
    .padding = 0
  };
  a_DrawText( "TEXT RENDERING TEST", SCREEN_WIDTH / 2, 20, &title_config );

  // Instructions
  aTextStyle_t info_config = {
    .type = FONT_ENTER_COMMAND,
    .fg = {200, 200, 200, 255},
    .bg = {0, 0, 0, 0},
    .align = TEXT_ALIGN_LEFT,
    .wrap_width = 0,
    .scale = 0.8f,
    .padding = 0
  };
  a_DrawText( "ESC: Back to game | Q: Quit", 10, 70, &info_config );

  // ========================================================================
  // LEFT COLUMN: Glyph/Unicode tests
  // ========================================================================
  aTextStyle_t test_config = {
    .type = FONT_ENTER_COMMAND,
    .fg = {255, 255, 255, 255},
    .bg = {0, 0, 0, 0},
    .align = TEXT_ALIGN_LEFT,
    .wrap_width = col_width,
    .scale = 0.8f,
    .padding = 0
  };

  // Test 1
  int y1 = left_items[0]->calc_y;
  a_DrawText( "Test 1: Em-dash (should show - as fallback)", 10, y1, &info_config );
  a_DrawText( "Your Aces shimmer with stolen luck\xe2\x80\x94the House's punishment backfired.", 10, y1 + TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP, &test_config );

  // Test 2
  int y2 = left_items[1]->calc_y;
  a_DrawText( "Test 2: Various Unicode chars", 10, y2, &info_config );
  a_DrawText( "Curly quotes: \xe2\x80\x9cHello\xe2\x80\x9d Ellipsis: \xe2\x80\xa6 En-dash: \xe2\x80\x93", 10, y2 + TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP, &test_config );

  // Test 3
  int y3 = left_items[2]->calc_y;
  a_DrawText( "Test 3: Normal ASCII", 10, y3, &info_config );
  a_DrawText( "The quick brown fox jumps over the lazy dog.", 10, y3 + TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP, &test_config );

  // Test 4
  int y4 = left_items[3]->calc_y;
  a_DrawText( "Test 4: Wrapped text", 10, y4, &info_config );
  aTextStyle_t wrap_config = {
    .type = FONT_ENTER_COMMAND,
    .fg = {100, 255, 100, 255},
    .bg = {0, 0, 0, 0},
    .align = TEXT_ALIGN_LEFT,
    .wrap_width = col_width,
    .scale = 0.8f,
    .padding = 0
  };
  a_DrawText( "This is a longer piece of text that should wrap across multiple lines. "
              "Previously, text with em-dashes would silently truncate here.", 10, y4 + TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP, &wrap_config );

  // ========================================================================
  // RIGHT COLUMN: Background/Padding tests (Issue #12)
  // ========================================================================
  int right_x = SCREEN_WIDTH / 2 + 20;

  // Test 5
  int y5 = right_items[0]->calc_y;
  char test5_header[64];
  snprintf( test5_header, sizeof(test5_header), "Test 5: [-/+] %s", test5_labels[test_index] );
  a_DrawText( test5_header, right_x, y5, &info_config );
  aTextStyle_t bg_solid = {
    .type = FONT_ENTER_COMMAND,
    .fg = test5_fg[test_index],
    .bg = test5_bg[test_index],
    .align = TEXT_ALIGN_LEFT,
    .wrap_width = 0,
    .scale = 0.8f,
    .padding = 4
  };
  a_DrawText( test5_labels[test_index], right_x, y5 + TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP, &bg_solid );

  // Test 6
  int y6 = right_items[1]->calc_y;
  char test6_label[64];
  snprintf( test6_label, sizeof(test6_label), "Test 6: [-/+] Alpha=%d", test6_alphas[test_index] );
  a_DrawText( test6_label, right_x, y6, &info_config );
  aTextStyle_t bg_semi = {
    .type = FONT_ENTER_COMMAND,
    .fg = {255, 255, 255, 255},
    .bg = {0, 0, 0, (uint8_t)test6_alphas[test_index]},
    .align = TEXT_ALIGN_LEFT,
    .wrap_width = 0,
    .scale = 0.8f,
    .padding = 8
  };
  a_DrawText( "Text with alpha bg", right_x, y6 + TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP, &bg_semi );

  // Test 7: Different padding values (random colors from palette)
  int y7 = right_items[2]->calc_y;
  a_DrawText( "Test 7: [-/+] Padding 0, 4, 8, 16", right_x, y7, &info_config );
  aTextStyle_t pad0 = { .type = FONT_ENTER_COMMAND, .fg = test5_fg[test7_fg_idx[0]], .bg = test5_bg[test7_bg_idx[0]], .scale = 0.8f, .padding = 0 };
  aTextStyle_t pad4 = { .type = FONT_ENTER_COMMAND, .fg = test5_fg[test7_fg_idx[1]], .bg = test5_bg[test7_bg_idx[1]], .scale = 0.8f, .padding = 4 };
  aTextStyle_t pad8 = { .type = FONT_ENTER_COMMAND, .fg = test5_fg[test7_fg_idx[2]], .bg = test5_bg[test7_bg_idx[2]], .scale = 0.8f, .padding = 8 };
  aTextStyle_t pad16 = { .type = FONT_ENTER_COMMAND, .fg = test5_fg[test7_fg_idx[3]], .bg = test5_bg[test7_bg_idx[3]], .scale = 0.8f, .padding = 16 };
  int y7c = y7 + TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP;
  a_DrawText( "p=0", right_x, y7c, &pad0 );
  a_DrawText( "p=4", right_x + 80, y7c, &pad4 );
  a_DrawText( "p=8", right_x + 170, y7c, &pad8 );
  a_DrawText( "p=16", right_x + 270, y7c, &pad16 );

  // Test 8: Color combos
  int y8 = right_items[3]->calc_y;
  a_DrawText( "Test 8: Color combos", right_x, y8, &info_config );
  aTextStyle_t red_white = { .type = FONT_ENTER_COMMAND, .fg = {255,255,255,255}, .bg = {200,50,50,255}, .scale = 0.8f, .padding = 6 };
  aTextStyle_t green_black = { .type = FONT_ENTER_COMMAND, .fg = {0,0,0,255}, .bg = {50,200,50,255}, .scale = 0.8f, .padding = 6 };
  aTextStyle_t yellow_dark = { .type = FONT_ENTER_COMMAND, .fg = {50,50,50,255}, .bg = {255,220,100,255}, .scale = 0.8f, .padding = 6 };
  int y8c = y8 + TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP;
  a_DrawText( "Error!", right_x, y8c, &red_white );
  a_DrawText( "Success", right_x + 100, y8c, &green_black );
  a_DrawText( "Warning", right_x + 220, y8c, &yellow_dark );

  // Test 9: Wrapped with background
  int y9 = right_items[4]->calc_y;
  a_DrawText( "Test 9: Wrapped + background", right_x, y9, &info_config );
  aTextStyle_t wrap_bg = {
    .type = FONT_ENTER_COMMAND,
    .fg = {255, 255, 255, 255},
    .bg = {40, 40, 80, 200},
    .align = TEXT_ALIGN_LEFT,
    .wrap_width = col_width - 20,
    .scale = 0.8f,
    .padding = 8
  };
  a_DrawText( "This tooltip has a background and wraps nicely. Great for hints!", right_x, y9 + TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP, &wrap_bg );

  // Test 10: Centered with background
  int y10 = right_items[5]->calc_y;
  a_DrawText( "Test 10: Center-aligned bg", right_x, y10, &info_config );
  aTextStyle_t center_bg = {
    .type = FONT_ENTER_COMMAND,
    .fg = {255, 255, 255, 255},
    .bg = {100, 50, 150, 255},
    .align = TEXT_ALIGN_CENTER,
    .wrap_width = 0,
    .scale = 0.8f,
    .padding = 10
  };
  a_DrawText( "CENTERED", right_x + col_width / 2, y10 + TEST_TITLE_HEIGHT + TEST_TITLE_CONTENT_GAP, &center_bg );
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
      scene_test_text_draw( dt );
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

  // Draw the player square
  a_DrawFilledRect( (aRectf_t){square_x, square_y, 32, 32}, (aColor_t){0, 0, 255, 255} );

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