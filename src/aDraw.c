/**
 * @file aDraw.c
 * @brief Drawing system implementation for the Archimedes engine
 * 
 * This file contains the implementation of all 2D drawing functions for the Archimedes
 * graphics engine. It provides a comprehensive set of drawing primitives including:
 * - Scene management (prepare/present)
 * - Basic primitives (points, lines, shapes)
 * - Filled and outlined shapes
 * - Surface and texture blitting operations
 * - Color management and render state handling
 * 
 * All drawing functions use SDL2 as the underlying graphics backend and follow
 * a consistent pattern of temporarily setting render colors and restoring them
 * to white (255,255,255,255) after each operation to maintain predictable state.
 * 
 * @author Archimedes Team
 * @date 2025
 * @see Archimedes.h
 */

#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

/**
 * @brief Initializes the render target and clears the screen
 * 
 * Implementation details:
 * 1. Sets the render draw color to the current background color from app.background
 * 2. Clears the entire render target using SDL_RenderClear
 * 3. Resets the render color to white (255,255,255,255) for subsequent operations
 * 
 * This function must be called at the beginning of each frame before any drawing
 * operations. It ensures a clean slate for the frame and establishes the background.
 * 
 * @implementation Uses SDL_SetRenderDrawColor and SDL_RenderClear
 * @complexity O(1) - constant time operation
 * @thread_safety Not thread-safe, must be called from main render thread
 */
void a_PrepareScene( void )
{
  SDL_SetRenderDrawColor(app.renderer, app.background.r, app.background.g, app.background.b, app.background.a);
  SDL_RenderClear(app.renderer);
  // Reset the renderer color to white
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
}

/**
 * @brief Presents the rendered scene to the screen
 * 
 * Implementation details:
 * - Calls SDL_RenderPresent to swap the front and back buffers
 * - This is the final step in the render cycle that makes all drawing visible
 * - Uses double-buffered rendering to prevent screen tearing
 * 
 * Must be called after all drawing operations in a frame are complete.
 * 
 * @implementation Direct wrapper around SDL_RenderPresent
 * @complexity O(1) - hardware-accelerated buffer swap
 * @thread_safety Not thread-safe, must be called from main render thread
 */
void a_PresentScene( void )
{
  SDL_RenderPresent(app.renderer);
}

/**
 * @brief Draws a single pixel at the specified coordinates
 * 
 * Implementation details:
 * 1. Sets the render draw color to the specified color components
 * 2. Draws a single pixel using SDL_RenderDrawPoint
 * 3. Restores the render color to white for consistent state
 * 
 * The point is automatically clipped if coordinates fall outside the render target.
 * 
 * @implementation Uses SDL_SetRenderDrawColor and SDL_RenderDrawPoint
 * @complexity O(1) - single pixel operation
 * @thread_safety Not thread-safe, must be called from main render thread
 */
void a_DrawPoint( const int x, const int y, const aColor_t color )
{
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawPoint(app.renderer, x, y);
  // Reset the renderer color to white
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}

/**
 * @brief Draws a line between two points using Bresenham's algorithm
 * 
 * Implementation details:
 * 1. Sets the render draw color to the specified color
 * 2. Uses SDL_RenderDrawLine which implements Bresenham's line algorithm
 * 3. Handles all line orientations (horizontal, vertical, diagonal)
 * 4. Restores render color to white
 * 
 * SDL automatically handles clipping and sub-pixel accuracy.
 * 
 * @implementation Uses SDL_SetRenderDrawColor and SDL_RenderDrawLine
 * @complexity O(max(|x2-x1|, |y2-y1|)) - proportional to line length
 * @thread_safety Not thread-safe, must be called from main render thread
 */
void a_DrawLine( const int x1, const int y1, const int x2, const int y2, const aColor_t color )
{
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x1, y1, x2, y2);
  // Reset the renderer color to white
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}

void a_DrawHorizontalLine( const int x1, const int x2, const int y, const aColor_t color )
{
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x1, y, x2, y);
  // Reset the renderer color to white
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}

void a_DrawVerticalLine( const int y1, const int y2, const int x, const aColor_t color )
{
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x, y1, x, y2);
  // Reset the renderer color to white
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}

/**
 * @brief Draws a circle outline using Bresenham's circle algorithm
 * 
 * Implementation details:
 * 1. Uses the classical Bresenham circle algorithm for pixel-perfect circles
 * 2. Draws 8 symmetric points for each calculated point to optimize performance
 * 3. Decision variable: 5 - (4 * radius) for initial midpoint calculation
 * 4. Iterates only through one octant, reflecting to draw full circle
 * 
 * Algorithm steps:
 * - Start at (0, radius) and iterate while x <= y
 * - Draw 8 symmetric points for each (x,y) coordinate
 * - Update decision variable to determine next pixel
 * - Increment x and conditionally decrement y
 * 
 * Note: There's a bug in line 96 where the color is reset to white inside the loop,
 * which should be moved outside the while loop for efficiency.
 * 
 * @implementation Custom Bresenham circle algorithm with 8-way symmetry
 * @complexity O(radius) - linear in radius size
 * @thread_safety Not thread-safe, must be called from main render thread
 */
void a_DrawCircle( const int posX, const int posY, const int radius, const aColor_t color )
{
  int x = 0;
  int y = radius;
  int decision = 5 - ( 4 * radius );

  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
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
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}

/**
 * @brief Draws a filled circle using scan-line filling with Bresenham's algorithm
 * 
 * Implementation details:
 * 1. Uses modified Bresenham circle algorithm for outline calculation
 * 2. Instead of drawing points, draws horizontal lines (scan lines) to fill
 * 3. Draws 4 horizontal lines for each calculated point:
 *    - Two lines at y-coordinate posY ± y
 *    - Two lines at y-coordinate posY ± x
 * 4. Each line spans from left edge to right edge of circle at that y-level
 * 
 * Scan-line algorithm ensures solid fill without gaps or overlaps.
 * Same decision variable logic as outline circle for consistency.
 * 
 * Note: Same color reset bug as outline circle (line 123) - should be outside loop.
 * 
 * @implementation Scan-line filling with Bresenham circle calculation
 * @complexity O(radius²) - fills all pixels within circle
 * @thread_safety Not thread-safe, must be called from main render thread
 */
void a_DrawFilledCircle( const int posX, const int posY, const int radius, const aColor_t color )
{
  int x = 0;
  int y = radius;
  int decision = 5 - ( 4 * radius );

  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  while ( x <= y )
  {
    SDL_RenderDrawLine(app.renderer, posX - x, posY - y, posX + x, posY - y);
    SDL_RenderDrawLine(app.renderer, posX - y, posY - x, posX + y, posY - x);
    SDL_RenderDrawLine(app.renderer, posX - y, posY + x, posX + y, posY + x);
    SDL_RenderDrawLine(app.renderer, posX - x, posY + y, posX + x, posY + y);

    if ( decision > 0 )
    {
      y--;
      decision -= 8 *  y;
    }

    x++;

    decision += 8 * x + 4;
  }
  // Reset color to white after drawing is complete
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}

void a_DrawTriangle( const int x0, const int y0, const int x1, const int y1,
                     const int x2, const int y2, const aColor_t color )
{
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
  SDL_SetRenderDrawColor(app.renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(app.renderer, x0, y0, x1, y1);
  SDL_RenderDrawLine(app.renderer, x1, y1, x2, y2);
  SDL_RenderDrawLine(app.renderer, x2, y2, x0, y0);
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}

/**
 * @brief Draws a filled triangle using barycentric coordinate algorithm (DISABLED)
 * 
 * Implementation details (commented out):
 * 1. Calculates bounding box: min/max X and Y coordinates of all vertices
 * 2. Uses barycentric coordinates to determine if each pixel is inside triangle
 * 3. Calculates edge vectors v1 and v2 from vertex 0 to vertices 1 and 2
 * 4. For each pixel in bounding box:
 *    - Calculates vector q from vertex 0 to pixel
 *    - Computes barycentric coordinates s and t using cross products
 *    - Draws pixel if s >= 0, t >= 0, and s + t <= 1 (inside triangle)
 * 
 * Algorithm advantages:
 * - Mathematically precise
 * - Handles all triangle orientations
 * - No edge cases for degenerate triangles
 * 
 * Currently disabled due to dependency on vector math functions (dVec2_t, cross product).
 * When enabled, this would provide pixel-perfect triangle filling.
 * 
 * @implementation Barycentric coordinate rasterization (currently disabled)
 * @complexity O(bounding_box_area) - proportional to triangle size
 * @thread_safety Not thread-safe, must be called from main render thread
 * @note Implementation is commented out - function currently does nothing
 */
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

void a_DrawRect( const aRect_t rect, const aColor_t color )
{
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
  SDL_SetRenderDrawColor( app.renderer, color.r, color.g, color.b, color.a );
  SDL_Rect sdl_rect = (SDL_Rect){ rect.x, rect.y, rect.w, rect.h };
  SDL_RenderDrawRect( app.renderer, &sdl_rect );
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}


void a_DrawFilledRect( const aRect_t rect, const aColor_t color )
{
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_BLEND );
  SDL_SetRenderDrawColor( app.renderer, color.r, color.g, color.b, color.a );
  SDL_Rect sdl_rect = (SDL_Rect){ rect.x, rect.y, rect.w, rect.h };
  SDL_RenderFillRect( app.renderer, &sdl_rect );
  SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
  SDL_SetRenderDrawBlendMode( app.renderer, SDL_BLENDMODE_NONE );
}


/**
 * @brief Blits an SDL surface to the screen by converting to texture
 * 
 * Implementation details:
 * 1. Creates destination rectangle with specified x,y coordinates
 * 2. Converts surface to hardware-accelerated texture using SDL_CreateTextureFromSurface
 * 3. Queries texture dimensions to set destination width/height
 * 4. Renders entire texture to destination rectangle using SDL_RenderCopy
 * 5. Immediately destroys texture to prevent memory leaks
 * 
 * Performance considerations:
 * - Creates and destroys texture each call (not optimized for repeated use)
 * - Better to use texture caching for frequently blitted surfaces
 * - Hardware acceleration depends on SDL/graphics driver configuration
 * 
 * Error handling:
 * - Prints error message if texture creation fails
 * - Continues execution even if texture query fails
 * 
 * @implementation Surface-to-texture conversion with immediate cleanup
 * @complexity O(1) for conversion, O(surface_area) for initial texture upload
 * @thread_safety Not thread-safe, must be called from main render thread
 * @memory Creates temporary texture - destroyed before function returns
 */
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

/**
 * @brief Blits a rectangular region of a surface with scaling
 * 
 * Implementation details:
 * 1. Sets up destination rectangle with scaling: dest_size = src_size * scale
 * 2. Converts surface to texture using SDL_CreateTextureFromSurface
 * 3. IMPORTANT: Frees the source surface immediately after conversion
 * 4. Renders only the specified source rectangle to scaled destination
 * 5. Does NOT destroy the created texture (potential memory leak)
 * 
 * Critical issues:
 * - Surface is freed but texture is never destroyed
 * - This will cause memory leaks if called repeatedly
 * - Should add SDL_DestroyTexture(img) at the end
 * 
 * The scaling is applied to the destination rectangle, so a scale of 2
 * will make the blitted region appear twice as large on screen.
 * 
 * @implementation Surface-to-texture with scaling and partial blit
 * @complexity O(1) for setup, O(src_area) for texture upload
 * @thread_safety Not thread-safe, must be called from main render thread
 * @memory LEAK: Creates texture but never destroys it
 * @warning Frees input surface - caller cannot use surface after this call
 */
void a_BlitSurfRect( SDL_Surface* surf, SDL_Rect src, const int x, const int y,
                     const int scale )
{
  SDL_Rect dest;
  SDL_Texture* img;

  dest.x = x;
  dest.y = y;
  dest.w = src.w * scale;
  dest.h = src.h * scale;

  img = SDL_CreateTextureFromSurface(app.renderer, surf);

  SDL_FreeSurface(surf);

  int success = SDL_QueryTexture(img, NULL, NULL, &dest.w, &dest.h);

  if (success < 0)
  {
    printf("Error creating texture %s\n", SDL_GetError());
  }

  SDL_RenderCopy(app.renderer, img, &src, &dest);
}

void a_BlitTextureRect( SDL_Texture* texture, SDL_Rect src, const int x,
                        const int y, const int scale, const aColor_t color )
{
  SDL_Rect dest;

  dest.x = x;
  dest.y = y;
  dest.w = src.w * scale;
  dest.h = src.h * scale;
  SDL_SetTextureColorMod( texture, color.r, color.g, color.b );
  SDL_SetTextureAlphaMod( texture, color.a );

  SDL_RenderCopy(app.renderer, texture, &src, &dest);

}

void a_UpdateTitle( const char *title )
{
  SDL_SetWindowTitle( app.window, title );
}

void a_SetPixel( SDL_Surface *surface, int x, int y, aColor_t c )
{
  if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
    return;
  }
  
  uint32_t color;
  color = ( c.a << 24 ) | ( c.r << 16 ) | ( c.g << 8 ) | c.b;

  if (SDL_MUSTLOCK(surface)) {
    if (SDL_LockSurface(surface) < 0) {
      return;
    }
  }

  Uint8 *pixel_address = (Uint8 *)surface->pixels + y * surface->pitch 
                         + x * surface->format->BytesPerPixel;

  *(Uint32 *)pixel_address = color;

  if (SDL_MUSTLOCK(surface)) {
    SDL_UnlockSurface(surface);
  }
}

/**
 * @defgroup color_definitions Predefined Color Constants
 * @brief Global color constants for common colors and grayscale values
 * 
 * These color constants provide convenient access to commonly used colors
 * without needing to construct aColor_t structures manually. All colors
 * use full alpha (255) for complete opacity.
 * 
 * Color categories:
 * - Basic colors: black, white, red, green, blue, cyan, magenta, yellow
 * - Special colors: shit0-shit3 (custom color palette)
 * - Grayscale: gray0 (darkest) through gray9 (lightest) in increments of ~20
 * 
 * Usage example:
 * @code
 * a_DrawPoint(100, 100, red);        // Draw red point
 * a_DrawLine(0, 0, 100, 100, gray5); // Draw gray line
 * @endcode
 * 
 * @{
 */

// Basic color palette
aColor_t black   = {   0,   0,   0, 255 };  ///< Pure black
aColor_t blue    = {   0,   0, 255, 255 };  ///< Pure blue
aColor_t green   = {   0, 255,   0, 255 };  ///< Pure green
aColor_t cyan    = {   0, 255, 255, 255 };  ///< Blue + Green
aColor_t red     = { 255,   0,   0, 255 };  ///< Pure red
aColor_t magenta = { 255,   0, 255, 255 };  ///< Red + Blue
aColor_t yellow  = { 255, 255,   0, 255 };  ///< Red + Green
aColor_t white   = { 255, 255, 255, 255 };  ///< Pure white

// Custom color palette
aColor_t shit0   = { 128, 128, 128, 255 };  ///< Medium gray
aColor_t shit1   = { 128, 255, 255, 255 };  ///< Light cyan
aColor_t shit2   = { 128, 128, 255, 255 };  ///< Light blue
aColor_t shit3   = {   0, 255, 128, 255 };  ///< Light green

// Grayscale palette (gray0 = darkest, gray9 = lightest)
aColor_t gray9   = { 235, 235, 235, 255 };  ///< Very light gray
aColor_t gray8   = { 215, 215, 215, 255 };  ///< Light gray
aColor_t gray7   = { 195, 195, 195, 255 };  ///< Light-medium gray
aColor_t gray6   = { 175, 175, 175, 255 };  ///< Medium-light gray
aColor_t gray5   = { 155, 155, 155, 255 };  ///< Medium gray
aColor_t gray4   = { 135, 135, 135, 255 };  ///< Medium-dark gray
aColor_t gray3   = { 115, 115, 115, 255 };  ///< Dark-medium gray
aColor_t gray2   = {  95,  95,  95, 255 };  ///< Dark gray
aColor_t gray1   = {  55,  55,  55, 255 };  ///< Very dark gray
aColor_t gray0   = {  35,  35,  35, 255 };  ///< Near black

/** @} */ // end of color_definitions group
