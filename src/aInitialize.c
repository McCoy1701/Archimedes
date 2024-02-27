#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "Archimedes.h"

App app;

void aInit( void )
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
  IMG_Init(IMG_INIT_PNG);
  SDL_SetWindowTitle(app.window, "3D-Test");
  SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &app.window, &app.renderer);
  
  app.lastTime = 0;
  app.currentTime = 0;

  app.surfaceHead = NULL;

  //aInitAudio();

  aInitFont();

  srand(time(NULL));
}

void aQuit( void )
{
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();
}
