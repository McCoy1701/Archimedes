#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>

#include "Archimedes.h"

SDL_Texture *fontTexture;
static char drawTextBuffer[MAX_LINE_LENGTH];

void aInitFont( void )
{
  SDL_Surface* surface = aImageLoad("assets/font/font.png");

  fontTexture = SDL_CreateTextureFromSurface(app.renderer, surface);
  
  SDL_FreeSurface(surface);
}

void aDrawText( int x, int y, int r, int g, int b, int align, int scale, char *format, ... )
{
  int i, len, c;
  SDL_Rect rect;
  SDL_Rect dest;
  va_list args;

  memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

  va_start(args, format);
  vsprintf(drawTextBuffer, format, args);
  va_end(args);

  len = strlen(drawTextBuffer);

  switch(align)
  {
    case TEXT_RIGHT:
      x -= (len * GLYPH_WIDTH);
      break;
      
    case TEXT_CENTER:
      x -= (len * GLYPH_WIDTH) / 2;
      break;
  }

  rect.w = GLYPH_WIDTH;
  rect.h = GLYPH_HEIGHT;
  rect.y = 0;
  if (scale) {
    dest.w = GLYPH_WIDTH * 3;
    dest.h = GLYPH_HEIGHT * 3;
    dest.y = y;
    dest.x = x;
  } 

  else {
    dest.w = GLYPH_WIDTH;
    dest.h = GLYPH_HEIGHT;
    dest.y = y;
    dest.x = x;
  }

  SDL_SetTextureColorMod(fontTexture, r, g, b);

  for (i = 0; i < len; i++)
  {
    c = drawTextBuffer[i];

    if (c >= ' ' && c <= 'Z') {
      rect.x = (c - ' ') * GLYPH_WIDTH;
      if (!scale) {
          SDL_RenderCopy(app.renderer, fontTexture, &rect, &dest);
          dest.x += GLYPH_WIDTH;
        } 
        
      else {
          SDL_RenderCopy(app.renderer, fontTexture, &rect, &dest);
          dest.x += (GLYPH_WIDTH * 3);
      }


    }
  }
}

