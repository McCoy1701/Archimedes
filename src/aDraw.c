#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

void aPrepareScene( void )
{
  SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
  SDL_RenderClear(app.renderer);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  aDoInput();
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

void aDrawHoriLine( int x1, int x2, int y, Color color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x1, y, x2, y);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void aDrawVertLine( int y1, int y2, int x, Color color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x, y1, x, y2);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void aDrawCircle( int posX, int posY, int radius, Color color )
{
  int x = 0;
  int y = radius;
  int decision = 5 - ( 4 * radius );

  while ( x <= y )
  {
    SDL_SetRenderDrawColor( app.renderer, color.r, color.g, color.b, color.a );
    SDL_RenderDrawPoint( app.renderer, posX + x, posY - y );
    SDL_RenderDrawPoint( app.renderer, posX + x, posY + y );
    SDL_RenderDrawPoint( app.renderer, posX - x, posY - y );
    SDL_RenderDrawPoint( app.renderer, posX - x, posY + y );
    SDL_RenderDrawPoint( app.renderer, posX + y, posY - x );
    SDL_RenderDrawPoint( app.renderer, posX + y, posY + x );
    SDL_RenderDrawPoint( app.renderer, posX - y, posY - x );
    SDL_RenderDrawPoint( app.renderer, posX - y, posY + x );
    SDL_SetRenderDrawColor( app.renderer, 255, 255, 255, 255 );
   
    if ( decision > 0 )
    {
      y--;
      decision -= 8 *  y;
    }

    x++;
    
    decision += 8 * x + 4;
  }
}

void aDrawFilledCircle( int posX, int posY, int radius, Color color )
{
  int x = 0;
  int y = radius;
  int decision = 5 - ( 4 * radius );

  while ( x <= y )
  {
    SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(app.renderer, posX - x, posY - y, posX + x, posY - y);
    SDL_RenderDrawLine(app.renderer, posX - y, posY - x, posX + y, posY - x);
    SDL_RenderDrawLine(app.renderer, posX - y, posY + x, posX + y, posY + x);
    SDL_RenderDrawLine(app.renderer, posX - x, posY + y, posX + x, posY + y);
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);

    if ( decision > 0 )
    {
      y--;
      decision -= 8 *  y;
    }

    x++;
    
    decision += 8 * x + 4;
  }
}

void aDrawTriangle( int x0, int y0, int x1, int y1, int x2, int y2, Color color )
{
  aDrawLine(x0, y0, x1, y1, color);
  aDrawLine(x1, y1, x2, y2, color);
  aDrawLine(x2, y2, x0, y0, color);
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

      float s = dCross2f(q, v2) / dCross2f(v1, v2);
      float t = dCross2f(v1, q) / dCross2f(v1, v2);

      if ( (s >= 0) && (t >= 0) && (s + t <= 1)) {
        aDrawPoint(x, y, color);
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

Color black   = {  0,   0,   0, 255};
Color blue    = {  0,   0, 255, 255};
Color green   = {  0, 255,   0, 255};
Color cyan    = {  0, 255, 255, 255};
Color red     = {255,   0,   0, 255};
Color magenta = {255,   0, 255, 255};
Color yellow  = {255, 255,   0, 255};
Color white   = {255, 255, 255, 255};
Color shit0   = {128, 128, 128, 255};
Color shit1   = {128, 255, 255, 255};
Color shit2   = {128, 128, 255, 255};
Color shit3   = {  0, 255, 128, 255};

Color gray9 = {235, 235, 235, 255};
Color gray8  = {215, 215, 215, 255};
Color gray7  = {195, 195, 195, 255};
Color gray6  = {175, 175, 175, 255};
Color gray5  = {155, 155, 155, 255};
Color gray4  = {135, 135, 135, 255};
Color gray3  = {115, 115, 115, 255};
Color gray2  = { 95,  95,  95, 255};
Color gray1  = { 55,  55,  55, 255};
Color gray0  = { 35,  35,  35, 255};
