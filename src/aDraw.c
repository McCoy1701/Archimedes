#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

void a_prepare_scene( void )
{
  SDL_SetRenderDrawColor(app.renderer, app.background.r, app.background.g, app.background.b, app.background.a);
  SDL_RenderClear(app.renderer);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  a_do_input();
}

void a_present_scene( void )
{
  SDL_RenderPresent(app.renderer);
}

void a_draw_point( int x, int y, aColor_t color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawPoint(app.renderer, x, y);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void a_draw_line( int x1, int y1, int x2, int y2, aColor_t color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x1, y1, x2, y2);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void a_draw_horizontal_line( int x1, int x2, int y, aColor_t color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x1, y, x2, y);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void a_draw_vertical_line( int y1, int y2, int x, aColor_t color )
{
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x, y1, x, y2);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

void a_draw_circle( int posX, int posY, int radius, aColor_t color )
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

void a_draw_filled_circle( int posX, int posY, int radius, aColor_t color )
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

void a_draw_triangle( int x0, int y0, int x1, int y1, int x2, int y2, aColor_t color )
{
  a_draw_line(x0, y0, x1, y1, color);
  a_draw_line(x1, y1, x2, y2, color);
  a_draw_line(x2, y2, x0, y0, color);
}

void a_draw_filled_triangle( int x0, int y0, int x1, int y1, int x2, int y2, aColor_t color )
{
  int maxX = MAX(x0, MAX(x1, x2));
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

      float s = d_cross_product_dVec2f(q, v2) / d_cross_product_dVec2f(v1, v2);
      float t = d_cross_product_dVec2f(v1, q) / d_cross_product_dVec2f(v1, v2);

      if ( (s >= 0) && (t >= 0) && (s + t <= 1)) {
        a_draw_point(x, y, color);
      }
    }
  }
}

void a_draw_rect(int x, int y, int w, int h, aColor_t color )
{
  a_draw_line(x, y, x+w, y, color );
  a_draw_line(x+w, y, x+w, y+h, color );
  a_draw_line(x+w, y+h, x, y+h, color );
  a_draw_line(x, y+h, x, y, color );
}

void a_draw_filled_rect( int x, int y, int w, int h, aColor_t color )
{
  for ( int i = x; i < x+w; i++ )
  {
    a_draw_vertical_line( y, y+h, i, color );
  }
}

void a_draw_sdl_rect( SDL_Rect* src, int value )
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

void a_blit( SDL_Surface* surf, int x, int y )
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

void a_blit_rect( SDL_Surface* surf, SDL_Rect src, int x, int y )
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
