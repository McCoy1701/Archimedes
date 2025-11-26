#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

aAnimation_t* a_AnimationCreate( const char* filename )
{
  aAnimation_t* new_animation = malloc( sizeof( aAnimation_t ) );
  if ( new_animation == NULL )
  {
    printf( "Failed to allocate memory for animation: %s, %d\n", __FILE__, __LINE__ );
    return NULL;
  }
  
  new_animation = 


  return NULL;
}

void a_AnimationFree( aAnimation_t* animation )
{

}

void a_AnimationPlay( aPoint2f_t pos, aAnimation_t* animtion )
{

}

