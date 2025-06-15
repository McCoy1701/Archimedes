#include "Archimedes.h"

#ifndef __TEST_H__
#define __TEST_H__

extern aImageCache_t* test_cache;

aImageCacheNode_t* at_InitImageCacheNode( SDL_Surface* surf, const char* filename, const int ID );
int at_LoadImageCache( aImageCache_t* cache );
int at_TestImageCache(aImageCache_t* cache );

#endif

