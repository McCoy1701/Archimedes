#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

aMesh cubeMesh = {
  {
    { { { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 0.0f } } },
    { { { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } } },
  
    { { { 1.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.0f } } },
    { { { 1.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 0.0f } } },
  
    { { { 1.0f, 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f, 0.0f } } },
    { { { 1.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f } } },
  
    { { { 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } } },
    { { { 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } } },
  
    { { { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.0f } } },
    { { { 0.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f, 0.0f } } },
  
    { { { 1.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f } } },
    { { { 1.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } } },
  }
};

float matrixProjection[16] = {0};

void aDoLoop( void )
{
  aDoInput();
  
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    exit( 0 );
  }
}

void aRenderLoop( void )
{
  for ( int i = 0; i < 12; i++ )
  {
    aTriangle tri = cubeMesh.triangles[i];
    /* for ( int j = 0; j < 3; j++ )
    {
    printf( "triangle %d x: %.02f y: %.02f z: %.02f w: %.02f \n", i, tri.points[j].x, tri.points[j].y, tri.points[j].z, tri.points[j].w );
    } */
    aTriangle triProjected;
    dMatrixMultiplyVec3f( &triProjected.points[0], matrixProjection, tri.points[0] );
    dMatrixMultiplyVec3f( &triProjected.points[1], matrixProjection, tri.points[1] );
    dMatrixMultiplyVec3f( &triProjected.points[2], matrixProjection, tri.points[2] ); 

    triProjected.points[0].x += 1.0f; triProjected.points[0].y += 1.0f;
    triProjected.points[1].x += 1.0f; triProjected.points[1].y += 1.0f;
    triProjected.points[2].x += 1.0f; triProjected.points[2].y += 1.0f;

    triProjected.points[0].x *= 0.5f * SCREEN_WIDTH; triProjected.points[0].y *= 0.5f * SCREEN_HEIGHT;
    triProjected.points[1].x *= 0.5f * SCREEN_WIDTH; triProjected.points[1].y *= 0.5f * SCREEN_HEIGHT;
    triProjected.points[2].x *= 0.5f * SCREEN_WIDTH; triProjected.points[2].y *= 0.5f * SCREEN_HEIGHT;

    aDrawTriangle(triProjected.points[0].x, triProjected.points[0].y, triProjected.points[1].x, triProjected.points[1].y, triProjected.points[2].x, triProjected.points[2].y, white );
  }
}

void aInitGame( void )
{
  app.delegate.logic = aDoLoop;
  app.delegate.draw = aRenderLoop;
  float fov = 90.0f;
  float far = 1000.0f;
  float near = 0.1f;
  float aspectRatio = (float)SCREEN_HEIGHT/SCREEN_WIDTH;

  dMatrixProjectionf( matrixProjection, aspectRatio, fov, near, far );
}

void aMainloop( void )
{
  aPrepareScene();

  app.delegate.logic();
  app.delegate.draw();
  
  aPresentScene();
}

int main( void )
{
  aInit();

  aInitGame();

  #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop( aMainloop, -1, 1 );
  #endif

  #ifndef __EMSCRIPTEN__
    while( 1 ) {
      aMainloop();
    }
  #endif

  aQuit();

  return 0;
}