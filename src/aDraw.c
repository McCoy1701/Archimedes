#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

void a_PrepareScene( void )
{
  SDL_SetRenderDrawColor(app.renderer, app.background.r, app.background.g, app.background.b, app.background.a);
  SDL_RenderClear(app.renderer);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void a_PresentScene( void )
{
  SDL_RenderPresent(app.renderer);
}

void a_DrawPoint( const int x, const int y, const aColor_t color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawPoint(app.renderer, x, y);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void a_DrawLine( const int x1, const int y1, const int x2, const int y2, const aColor_t color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x1, y1, x2, y2);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void a_DrawHorizontalLine( const int x1, const int x2, const int y, const aColor_t color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x1, y, x2, y);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void a_DrawVerticalLine( const int y1, const int y2, const int x, const aColor_t color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x, y1, x, y2);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void a_DrawCircle( const int posX, const int posY, const int radius, const aColor_t color )
{
  int x = 0;
  int y = radius;
  int decision = 5 - ( 4 * radius );

  SDL_SetRenderDrawColor( app.renderer, color.r, color.g, color.b, color.a );
  while ( x <= y )
  {
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

void a_DrawFilledCircle( const int posX, const int posY, const int radius, const aColor_t color )
{
  int x = 0;
  int y = radius;
  int decision = 5 - ( 4 * radius );

  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  while ( x <= y )
  {
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

void a_DrawTriangle( const int x0, const int y0, const int x1, const int y1,
                     const int x2, const int y2, const aColor_t color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x0, y0, x1, y1);
  SDL_RenderDrawLine(app.renderer, x1, y1, x2, y2);
  SDL_RenderDrawLine(app.renderer, x2, y2, x0, y0);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void a_DrawFilledTriangle( const int x0, const int y0, const int x1, const int y1,
                           const int x2, const int y2, const aColor_t color )
{
/*  int maxX = MAX(x0, MAX(x1, x2));
  int minX = MIN(x0, MIN(x1, x2));
  int maxY = MAX(y0, MAX(y1, y2));
  int minY = MIN(y0, MIN(y1, y2));

  dVec2_t v1 = {x1 - x0, y1 - y0};
  dVec2_t v2 = {x2 - x0, y2 - y0};

  for (int x = minX; x <= maxX; x++)
  {
    for (int y = minY; y <= maxY; y++)
    {
      dVec2_t q = {x - x0, y - y0};

      float s = d_CrossProductVec2f(q, v2) / d_CrossProductVec2f(v1, v2);
      float t = d_CrossProductVec2f(v1, q) / d_CrossProductVec2f(v1, v2);

      if ( (s >= 0) && (t >= 0) && (s + t <= 1)) {
        SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint(app.renderer, x, y);
      }
    }
  }*/
}

void a_DrawRect( const int x, const int y, const int w, const int h, const aColor_t color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_Rect rect = { x, y, w, h };
  SDL_RenderDrawRect( app.renderer, &rect );
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}


void a_DrawFilledRect( const int x, const int y, const int w, const int h, const aColor_t color )
{
  SDL_SetRenderDrawColor( app.renderer, color.r, color.g, color.b, color.a );
  SDL_Rect rect = { x, y, w, h };
  SDL_RenderFillRect( app.renderer, &rect );
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}


void a_Blit( SDL_Surface* surf, const int x, const int y )
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

void a_BlitRect( SDL_Surface* surf, SDL_Rect src, const int x, const int y )
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

void a_UpdateTitle( const char *title )
{
  SDL_SetWindowTitle( app.window, title );
}

aColor_t black   = {   0,   0,   0, 255 };
aColor_t blue    = {   0,   0, 255, 255 };
aColor_t green   = {   0, 255,   0, 255 };
aColor_t cyan    = {   0, 255, 255, 255 };
aColor_t red     = { 255,   0,   0, 255 };
aColor_t magenta = { 255,   0, 255, 255 };
aColor_t yellow  = { 255, 255,   0, 255 };
aColor_t white   = { 255, 255, 255, 255 };
aColor_t shit0   = { 128, 128, 128, 255 };
aColor_t shit1   = { 128, 255, 255, 255 };
aColor_t shit2   = { 128, 128, 255, 255 };
aColor_t shit3   = {   0, 255, 128, 255 };

aColor_t gray9   = { 235, 235, 235, 255 };
aColor_t gray8   = { 215, 215, 215, 255 };
aColor_t gray7   = { 195, 195, 195, 255 };
aColor_t gray6   = { 175, 175, 175, 255 };
aColor_t gray5   = { 155, 155, 155, 255 };
aColor_t gray4   = { 135, 135, 135, 255 };
aColor_t gray3   = { 115, 115, 115, 255 };
aColor_t gray2   = {  95,  95,  95, 255 };
aColor_t gray1   = {  55,  55,  55, 255 };
aColor_t gray0   = {  35,  35,  35, 255 };

