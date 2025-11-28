#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "Archimedes.h"
#include "test_text.h"
#include "player_actions.h"
#include "audio_test.h"
#include "enemy.h"

// Scene system
typedef enum {
  SCENE_GAME,
  SCENE_TEST_TEXT,
  SCENE_AUDIO_TEST
} Scene_t;

static Scene_t current_scene = SCENE_GAME;

// Scene function declarations
static void scene_game_logic( float dt );
static void scene_game_draw( float dt );

static void aDoLoop( float );
static void aRenderLoop( float );

aImage_t* bullet_img;

// Hit sounds (exported for enemy.c)
#define HIT_SOUND_COUNT 5
aSoundEffect_t hit_sounds[HIT_SOUND_COUNT];
int hit_sounds_loaded = 0;

// Weather sound
static aSoundEffect_t rain_sound;
static int rain_loaded = 0;

// Death sound (exported for enemy.c)
aSoundEffect_t die_sound;
int die_loaded = 0;

// Timer system (countdown from 15:00)
static float time_remaining = 15.0f * 60.0f; // 15 minutes in seconds

// Enemy spawning
#define MAX_ENEMIES 50
#define MAX_BLOOD_PARTICLES 500
static float spawn_timer = 0.0f;
static float spawn_interval = 2.0f; // spawn every 2 seconds

void aInitGame( void )
{
  app.delegate.logic = aDoLoop;
  app.delegate.draw  = aRenderLoop;

  // Set background color (dark blue)
  app.background = (aColor_t){20, 20, 60, 255};

  bullet_img = a_ImageLoad( "resources/assets/bullet.png" );
  if ( bullet_img == NULL )
  {
    printf( "Failed to load bullet image\n" );
  }

  // Initialize player
  player_init();

  // Initialize enemy system
  enemy_init(MAX_ENEMIES, MAX_BLOOD_PARTICLES);

  // Load death sound
  if (a_AudioLoadSound("resources/soundEffects/die1.wav", &die_sound) == 0) {
    printf("Loaded die1.wav\n");
    die_loaded = 1;
  } else {
    printf("Failed to load die1.wav\n");
  }

  // Load hit sounds
  const char* hit_files[HIT_SOUND_COUNT] = {
    "resources/soundEffects/hit1.wav",
    "resources/soundEffects/hit2.wav",
    "resources/soundEffects/hit3.wav",
    "resources/soundEffects/hit4.wav",
    "resources/soundEffects/hit5.wav"
  };

  hit_sounds_loaded = 1;
  for (int i = 0; i < HIT_SOUND_COUNT; i++) {
    if (a_AudioLoadSound(hit_files[i], &hit_sounds[i]) < 0) {
      printf("Failed to load %s\n", hit_files[i]);
      hit_sounds_loaded = 0;
    }
  }
  if (hit_sounds_loaded) {
    printf("Loaded %d hit sounds\n", HIT_SOUND_COUNT);
  }

  // Load and play rain sound on weather channel
  if (a_AudioLoadSound("resources/music/rain_2.wav", &rain_sound) == 0) {
    printf("Loaded rain_2.wav\n");
    rain_loaded = 1;

    // Play on weather channel, loop forever, volume 32, interrupt mode
    aAudioOptions_t rain_opts = {
      .channel = AUDIO_CHANNEL_WEATHER,
      .volume = 16,           // 32 out of 128 (25% volume)
      .loops = -1,            // Loop forever
      .fade_ms = 0,           // No fade
      .interrupt = 1          // Interrupt any previous weather sound
    };
    a_AudioPlaySound(&rain_sound, &rain_opts);
    printf("Playing rain on channel %d at volume 32\n", AUDIO_CHANNEL_WEATHER);
  } else {
    printf("Failed to load rain_2.wav\n");
  }

  // Initialize audio test (loads calm BGM and starts playback)
  audio_test_init();
}

static void aDoLoop( float dt )
{
  a_DoInput();

  // Global ESC to quit (works in all scenes)
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    app.running = 0;
  }

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
    case SCENE_AUDIO_TEST:
      audio_test_update();
      // ESC to return to game
      if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
      {
        current_scene = SCENE_GAME;
        app.keyboard[ SDL_SCANCODE_ESCAPE ] = 0;
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

  // Spawn enemies (only if not at max capacity)
  int current_enemy_count = enemy_get_count();
  if ( current_enemy_count < MAX_ENEMIES )
  {
    spawn_timer += dt;
  }

  if ( spawn_timer >= spawn_interval && current_enemy_count < MAX_ENEMIES )
  {
    spawn_timer = 0.0f;

    // Random spawn position outside screen
    int side = rand() % 4; // 0=top, 1=right, 2=bottom, 3=left
    float spawn_x, spawn_y;

    if ( side == 0 ) // Top
    {
      spawn_x = RANDF( 0, SCREEN_WIDTH );
      spawn_y = -16;
    }
    else if ( side == 1 ) // Right
    {
      spawn_x = SCREEN_WIDTH + 16;
      spawn_y = RANDF( 0, SCREEN_HEIGHT );
    }
    else if ( side == 2 ) // Bottom
    {
      spawn_x = RANDF( 0, SCREEN_WIDTH );
      spawn_y = SCREEN_HEIGHT + 16;
    }
    else // Left
    {
      spawn_x = -16;
      spawn_y = RANDF( 0, SCREEN_HEIGHT );
    }

    enemy_spawn( spawn_x, spawn_y );
  }

  // Check bullet collisions with enemies (alive or repositioning, not corpses)
  for ( int j = 0; j < MAX_ENEMIES; j++ )
  {
    EnemyState_t state = enemy_get_state( j );
    if ( enemy_is_active( j ) && state != ENEMY_STATE_CORPSE && state != ENEMY_STATE_HIT_KNOCKBACK )
    {
      float enemy_x, enemy_y;
      enemy_get_position( j, &enemy_x, &enemy_y );
      int bullet_hit = player_check_bullet_collision( enemy_x, enemy_y, 8.0f );

      if ( bullet_hit >= 0 )
      {
        // Get bullet velocity BEFORE destroying it
        float bullet_vx, bullet_vy;
        player_get_bullet_velocity( bullet_hit, &bullet_vx, &bullet_vy );

        // Apply hit with knockback
        enemy_hit( j, bullet_vx, bullet_vy );

        // Destroy the bullet
        player_destroy_bullet( bullet_hit );
      }
    }
  }

  // Update all enemies (AI, physics, blood particles)
  enemy_update( dt, player_get_x(), player_get_y(), player_get_vx(), player_get_vy() );

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

  // Ctrl+T to switch to test text scene
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

  // Ctrl+M to switch to audio test scene
  static int ctrl_m_pressed = 0;
  if ( (app.keyboard[ SDL_SCANCODE_LCTRL ] || app.keyboard[ SDL_SCANCODE_RCTRL ]) &&
       app.keyboard[ SDL_SCANCODE_M ] == 1 && !ctrl_m_pressed )
  {
    current_scene = SCENE_AUDIO_TEST;
    ctrl_m_pressed = 1;
    app.keyboard[ SDL_SCANCODE_M ] = 0;
  }
  if ( app.keyboard[ SDL_SCANCODE_M ] == 0 )
  {
    ctrl_m_pressed = 0;
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
    case SCENE_AUDIO_TEST:
      audio_test_draw();
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
  a_DrawText( timer_text, SCREEN_WIDTH / 2, 25, timer_config );

  // Draw enemy count and spawn timer (top right)
  int current_enemy_count = enemy_get_count();
  char enemy_count_text[32];
  snprintf( enemy_count_text, sizeof(enemy_count_text), "Enemies: %d/%d", current_enemy_count, MAX_ENEMIES );

  aTextStyle_t enemy_count_style = {
    .type = FONT_ENTER_COMMAND,
    .fg = {255, 255, 255, 255},
    .align = TEXT_ALIGN_RIGHT,
    .wrap_width = 0,
    .scale = 0.6f
  };
  a_DrawText( enemy_count_text, SCREEN_WIDTH - 20, 15, enemy_count_style );

  // Draw spawn timer (pauses at 0:00 if at max capacity)
  char spawn_timer_text[32];
  if ( current_enemy_count >= MAX_ENEMIES )
  {
    snprintf( spawn_timer_text, sizeof(spawn_timer_text), "Next spawn: --" );
  }
  else
  {
    float time_until_spawn = spawn_interval - spawn_timer;
    if ( time_until_spawn < 0.0f ) time_until_spawn = 0.0f;
    int spawn_seconds = (int)time_until_spawn;
    int spawn_hundredths = (int)((time_until_spawn - spawn_seconds) * 100);
    snprintf( spawn_timer_text, sizeof(spawn_timer_text), "Next spawn: %d:%02d", spawn_seconds, spawn_hundredths );
  }

  a_DrawText( spawn_timer_text, SCREEN_WIDTH - 20, 40, enemy_count_style );

  // Draw enemies (includes blood particles)
  enemy_draw();

  // Draw player and bullets
  player_draw( bullet_img );

  // Draw keyboard shortcuts (bottom right, 30% opacity white)
  aTextStyle_t shortcuts_style = {
    .type = FONT_ENTER_COMMAND,
    .fg = {255, 255, 255, 77},  // 30% opacity (255 * 0.3 = 77)
    .align = TEXT_ALIGN_RIGHT,
    .wrap_width = 0,
    .scale = 0.5f
  };

  int y_offset = SCREEN_HEIGHT - 80;
  a_DrawText("Ctrl+T - Text Test", SCREEN_WIDTH - 20, y_offset, shortcuts_style);
  y_offset += 20;
  a_DrawText("Ctrl+M - Audio Test", SCREEN_WIDTH - 20, y_offset, shortcuts_style);
  y_offset += 20;
  a_DrawText("ESC - Quit", SCREEN_WIDTH - 20, y_offset, shortcuts_style);
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