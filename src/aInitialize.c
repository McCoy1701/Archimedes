#include <stdio.h>
#include <stdlib.h>
//#include <SDL2/SDL_image.h>
#include <time.h>

#include "Archimedes.h"

aApp_t app;

void a_Init( const int width, const int height, const char *title )
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
  IMG_Init(IMG_INIT_PNG);
  SDL_SetWindowTitle(app.window, title);
  SDL_CreateWindowAndRenderer(width, height, 0, &app.window, &app.renderer);
  
  app.time.lastTime = 0;
  app.time.currentTime = 0;

  app.time.frameStart = 0;
  app.time.frameTime = 0;
  app.time.lastFrameCounterClear = 0;
  app.time.frames = 0;

  app.surfaceHead = NULL;

  app.running = 1;

  a_InitAudio();

  srand(time(NULL));
}

void a_Quit( void )
{
  if ( app.delegate.onExit ) app.delegate.onExit();
	SDL_DestroyRenderer( app.renderer );
	SDL_DestroyWindow( app.window );
  d_ClearLinkedList( app.surfaceHead );
  free( app.surfaceHead );
	SDL_Quit();
}
