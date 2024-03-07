#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <time.h>

#include "Archimedes.h"

aApp_t app;

void a_init( void )
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
  IMG_Init(IMG_INIT_PNG);
  SDL_SetWindowTitle(app.window, "3D-Test");
  SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &app.window, &app.renderer);
  
  app.time.lastTime = 0;
  app.time.currentTime = 0;

  app.surfaceHead = NULL;

  a_init_audio();

  a_init_font();

  srand(time(NULL));
}

void a_quit( void )
{
	SDL_DestroyRenderer( app.renderer );
	SDL_DestroyWindow( app.window );
  d_clear_linked_list( app.surfaceHead );
  free( app.surfaceHead );
	SDL_Quit();
}
