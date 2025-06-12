#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Archimedes.h"

aApp_t app;

int a_Init( const int width, const int height, const char *title )
{
  SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER );
  IMG_Init( IMG_INIT_PNG );
  TTF_Init();
  SDL_SetWindowTitle( app.window, title );
  SDL_CreateWindowAndRenderer( width, height, 0, &app.window, &app.renderer );
 
  app.mouse = (Mouse_t){ .x = 0, .y = 0, .state = 0, . pressed = 0, .button = 0, .wheel = 0, .clicks = 0 };

  app.time.lastTime    = 0;
  app.time.currentTime = 0;

  app.time.frameStart = 0;
  app.time.frameTime  = 0;
  app.time.lastFrameCounterClear = 0;
  app.time.frames = 0;

  app.img_cache = NULL;

  app.running = 1;

  a_InitAudio();
  a_InitImage();
  a_InitFont();

  srand(time(NULL));

  return 0;
}

void a_Quit( void )
{
  if ( app.delegate.onExit ) app.delegate.onExit();
	SDL_DestroyRenderer( app.renderer );
	SDL_DestroyWindow( app.window );
  free( app.img_cache );
	SDL_Quit();
}


char* log_level_strings[LOG_LEVEL_COUNT] =
{
  "NORMAL",
  "WARNING",
  "FATAL",
  "INFO",
  "DEBUG",
  "TRACE",
};

