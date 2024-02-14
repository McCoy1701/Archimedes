#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

void aPrepareScene( void )
{
  SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
  SDL_RenderClear(app.renderer);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  doInput();
}

void aPresentScene( void )
{
  SDL_RenderPresent(app.renderer);
}

void aDrawPoint( int x, int y, Color color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawPoint(app.renderer, x, y);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void aDrawLine( int x1, int y1, int x2, int y2, Color color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x1, y1, x2, y2);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void aDrawHoriLine( int x1, int x2, int y )
{
  SDL_RenderDrawLine(app.renderer, x1, y, x2, y);
}

void aDrawVertLine( int y1, int y2, int x )
{
  SDL_RenderDrawLine(app.renderer, x, y1, x, y2);
}

void aDrawCircle( int x, int y, int radius )
{
  int centerX;
  int centerY;

  for (int i = 0; i < 360; i++)  {
    centerX = x + radius * cos(i);
    centerY = y + radius * sin(i);
    SDL_RenderDrawPoint(app.renderer, centerX, centerY);
  }
}

void aDrawTriangle( int x0, int y0, int x1, int y1, int x2, int y2, Color color )
{
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

static int crossProduct(float *v1, float *v2) {
    return (v1[0] * v2[1] - v1[1] * v2[0]);
}

void aFillTriangle( int x0, int y0, int x1, int y1, int x2, int y2, Color color )
{
  int maxX = MAX(x0, MAX(x1, x2));
  int minX = MIN(x0, MIN(x1, x2));
  int maxY = MAX(y0, MAX(y1, y2));
  int minY = MIN(y0, MIN(y1, y2));

  float v1[2] = {x1 - x0, y1 - y0};
  float v2[2] = {x2 - x0, y2 - y0};

  for (int x = minX; x <= maxX; x++)
  {
    for (int y = minY; y <= maxY; y++)
    {
      float q[2] = {x - x0, y - y0};

      float s = (float)crossProduct(q, v2) / crossProduct(v1, v2);
      float t = (float)crossProduct(v1, q) / crossProduct(v1, v2);

      if ( (s >= 0) && (t >= 0) && (s + t <= 1)) {
        drawPoint(x, y, color);
      }
    }
  }
}

void aDrawRect( SDL_Rect* src, int value )
{
  if (!value) 
  {
    SDL_RenderDrawRect(app.renderer, src);
  }

  else 
  {
    SDL_RenderFillRect(app.renderer, src);
  }
}

void aBlit( SDL_Surface* surf, int x, int y )
{
  SDL_Rect dest;
  SDL_Texture* img;

  dest.x = x;
  dest.y = y;

  img = SDL_CreateTextureFromSurface(app.renderer, surf);
  
  int success = SDL_QueryTexture(img, NULL, NULL, &dest.w, &dest.h);

  if (success < 0)
  {
    printf("Error creating texture %s\n", SDL_GetError());
  }

  SDL_RenderCopy(app.renderer, img, NULL, &dest);

  SDL_DestroyTexture(img);
}

void aBlitRect( SDL_Surface* surf, SDL_Rect src, int x, int y )
{
  SDL_Rect dest;
  SDL_Texture* img;

  dest.x = x;
  dest.y = y;
  dest.w = src.w;
  dest.h = src.h;

  img = SDL_CreateTextureFromSurface(app.renderer, surf);

  SDL_FreeSurface(surf);

  int success = SDL_QueryTexture(img, NULL, NULL, &dest.w, &dest.h);

  if (success < 0)
  {
    printf("Error creating texture %s\n", SDL_GetError());
  }

  SDL_RenderCopy(app.renderer, img, &src, &dest);
}
