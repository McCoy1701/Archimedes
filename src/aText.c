#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

#define FONT_SIZE 32 
#define FONT_TEXTURE_SIZE 512
#define MAX_GLYPHS 400
#define MAX_GLYPH_SIZE 8
#define MAX_WORD_LENGTH 32
#define MAX_LINE_LENGTH 1024

static void initFont( char* filename, int font_type, int font_size );
static void initFontPNG( char* filename, int font_type, int glyph_width, int glyph_height );
static int DrawTextWrapped( char* text, int x, int y, int r, int g, int b, int font_type, int align, int max_width, int draw );
static void DrawTextLine( char* text, int x, int y, int r, int g, int b, int font_type, int align );
static int NextGlyph( const char* string, int* i, char* glyph_buffer );

static SDL_Color white_ = {255, 255, 255, 255};
//static TTF_Font* fonts[FONT_MAX];
static SDL_Rect glyphs[FONT_MAX][MAX_GLYPHS];
//static SDL_Texture* font_textures[FONT_MAX];

//static char* characters = " !#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¡¢£¥¦§ª«¬°±²µ¶·º»¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿŒœŸſƒΆΈΉΊΌΎΏΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩΪΫάέήίαβγδεζηθικλμνξοπρςστυφχψωϊϋόύώϕ•‼ⁿ₧€Ω←↑→↓↔↕↨∂∅∈∏∑∙√∞∟∩∫≈≡≤≥⌀⌂⌐⌠⌡⎮─│┌┐└┘├┤┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬▀▄█▌▐░▒▓■▬▲►▼◄○◘◙☺☻☼♀♂♠♣♥♦♪♫♬✓";
static char *characters = "~`^$Ö&|_# POfileorTBFS:handWCpygt2015-6,JwsbuGNUL3.Emj@c/\"IV\\RMD8+v?x;=%!AYq()'kH[]KzQX4Z79*àéí¡Çóè·úïçüºòÉÒÍÀ°æåøÆÅØ<>öÄäßÜá¿ñÁÊûâîôÈêùœÙìëęąłćżńśźŻŚŁĆÖ";

void a_InitFonts( void )
{
  initFont( "resources/fonts/EnterCommand.ttf", FONT_ENTER_COMMAND, 48 );
  initFont( "resources/fonts/JetBrains.ttf", FONT_LINUX, 32 );
  initFontPNG( "resources/fonts/CodePage737Font.png", FONT_GAME, 9, 16 );

  app.font_scale = 1;
  app.font_type = FONT_ENTER_COMMAND;
}

void a_CalcTextDimensions( char* text, int font_type, int* w, int* h )
{
  int i, n;
  SDL_Rect* g;

  *w = *h = 0;

  i = 0;

  while ( ( n = NextGlyph( text, &i, NULL ) ) != 0 )
  {
    g = &glyphs[font_type][n];
    *w += g->w * app.font_scale;
    *h = MAX( g->h * app.font_scale, *h );
  }

}

int a_GetWrappedTextHeight( char* text, int font_type, int max_width )
{
  return DrawTextWrapped( text, 0, 0, 255, 255, 255, font_type, TEXT_ALIGN_LEFT, max_width, 0 );
}

SDL_Texture* a_GetTextTexture( char* text, int font_type )
{
  SDL_Surface* surface;
  surface = TTF_RenderUTF8_Blended( app.fonts[font_type], text, white_ );
  
  return a_ToTexture( surface, 1 );
}

void a_DrawText( char* text, int x, int y, int r, int g, int b, int font_type, int align, int max_width )
{
  if ( max_width > 0 )
  {
    DrawTextWrapped( text, x, y, r, g, b, font_type, align, max_width, 1 );
  }

  else
  {
    DrawTextLine( text, x, y, r, g, b, font_type, align );
  }
}

static void initFontPNG( char* filename, int font_type, int glyph_width, int glyph_height )
{
  SDL_Surface* surface, *font_surf;
  SDL_Rect dest, rect;
  int i;

  memset( &glyphs[font_type], 0, sizeof( SDL_Rect ) * MAX_GLYPHS );
  
  font_surf = a_Image( filename );
  if( font_surf == NULL )
  {
    printf( "Failed to open font surface %s, %s", filename, SDL_GetError() );
    exit(1);
  }

  surface = SDL_CreateRGBSurface( 0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0xff );

  SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGBA( surface->format, 0, 0, 0, 0 ) );

  dest.x = dest.y = 0;
  rect.x = rect.y = 0;
  rect.w = dest.w = glyph_width;
  rect.h = dest.h = glyph_height;
  i = 0;

  while ( rect.x < font_surf->w )
  {
    if ( dest.x + dest.w >= FONT_TEXTURE_SIZE )
    {
      dest.x = 0;
      dest.y += dest.h + 1;
      if ( dest.y + dest.h >= FONT_TEXTURE_SIZE )
      {
        SDL_LogMessage( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Out of glyph space in %dx%d font atlas texture map.", FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE );
        exit(1);
      }
    }

    SDL_BlitSurface( font_surf, &rect, surface, &dest );
    
    glyphs[font_type][i++] = dest;

    dest.x += dest.w;
    rect.x += rect.w;
  }
  SDL_FreeSurface( font_surf );
  IMG_SavePNG( surface, "resources/fonts/test.png" );
  app.font_textures[font_type] = a_ToTexture( surface, 1 );
}

static void initFont( char* filename, int font_type, int font_size )
{
  SDL_Surface* surface, *text;
  SDL_Rect dest;
  int i, n;
  char glyph_buffer[MAX_GLYPH_SIZE];

  memset( &glyphs[font_type], 0, sizeof( SDL_Rect ) * MAX_GLYPHS );

  app.fonts[font_type] = TTF_OpenFont( filename, font_size );
  if( app.fonts[font_type] == NULL )
  {
    printf( "Failed to open font %s, %s", filename, TTF_GetError() );
    exit(1);
  }

  surface = SDL_CreateRGBSurface( 0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0xff );

  SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGBA( surface->format, 0, 0, 0, 0 ) );

  dest.x = dest.y = 0;
  i = 0;
  while ( ( n = NextGlyph( characters, &i, glyph_buffer ) ) != 0 )
  {
    if ( n >= MAX_GLYPHS )
    {
      printf( "Glyphs '%s' index exceeds array size (%d >= %d)\n", glyph_buffer, n, MAX_GLYPHS );
      exit(1);
    }

    text = TTF_RenderUTF8_Blended( app.fonts[font_type], glyph_buffer, white_ );

    TTF_SizeText( app.fonts[font_type], glyph_buffer, &dest.w, &dest.h );

    if ( dest.x + dest.w >= FONT_TEXTURE_SIZE )
    {
      dest.x = 0;
      dest.y += dest.h + 1;

      if ( dest.y + dest.h >= FONT_TEXTURE_SIZE )
      {
        SDL_LogMessage( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Out of glyph space in %dx%d font atlas texture map.", FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE );
        exit(1);
      }
    }

    SDL_BlitSurface( text, NULL, surface, &dest );
    glyphs[font_type][n] = dest;

    SDL_FreeSurface( text );
    dest.x += dest.w;
  }
  
  app.font_textures[font_type] = a_ToTexture( surface, 1 );
}

static int DrawTextWrapped( char* text, int x, int y, int r, int g, int b, int font_type, int align, int max_width, int draw )
{
  char word[MAX_WORD_LENGTH], line[MAX_LINE_LENGTH], glyph_buffer[MAX_GLYPH_SIZE];
  int i, n, word_width, line_width, len;

  i = 0;

  memset( word, 0, MAX_WORD_LENGTH );
  memset( line, 0, MAX_LINE_LENGTH );

  n = 0;

  line_width = word_width = 0;
  len = strlen( text );

  while ( ( n = NextGlyph( text, &i, glyph_buffer ) ) != 0 )
  {
    word_width += glyphs[font_type][n].w * app.font_scale;
    
    if ( n != ' ' )
    {
      strcat( word, glyph_buffer );
    }

    if ( n == ' ' || i == len )
    {
      if ( line_width + word_width >= max_width )
      {
        if ( draw )
        {
          DrawTextLine( line, x, y, r, g, b, font_type, align );
        }

        memset( line, 0, MAX_LINE_LENGTH );

        y += glyphs[font_type][' '].h * app.font_scale;
        line_width = 0;
      }
      
      else if ( line_width != 0 )
      {
        strcat( line, " " );
      }

      strcat( line, word );

      line_width += word_width;

      memset( word, 0, MAX_WORD_LENGTH );

      word_width = 0;
    }
  }

  if ( draw )
  {
    DrawTextLine( line, x, y, r, g, b, font_type, align );
  }

  return y + glyphs[font_type][' '].h * app.font_scale;
}

static void DrawTextLine( char* text, int x, int y, int r, int g, int b, int font_type, int align )
{
  int i, n, w, h;
  SDL_Rect* glyph, dest;

  if ( align != TEXT_ALIGN_LEFT )
  {
    a_CalcTextDimensions( text, font_type, &w, &h );

    if ( align == TEXT_ALIGN_CENTER )
    {
      x -= ( w / 2 );
    }
    else if ( align == TEXT_ALIGN_RIGHT )
    {
      x -= w;
    }
  }

  SDL_SetTextureColorMod( app.font_textures[font_type], r, g, b );

  i = 0;

  while ( ( n = NextGlyph( text, &i, NULL ) ) != 0 )
  {
    glyph = &glyphs[font_type][n];

    dest.x = x;
    dest.y = y;
    dest.w = glyph->w * app.font_scale;
    dest.h = glyph->h * app.font_scale;

    SDL_RenderCopy( app.renderer, app.font_textures[font_type], glyph, &dest );

    x += glyph->w * app.font_scale;
  }
}

static int NextGlyph( const char* string, int* i, char* glyph_buffer )
{
  int len;
  unsigned int bit;
  const char *p;

  bit = ( unsigned char ) string[*i];

  if ( bit < ' ' )
  {
    return 0;
  }

  len = 1;

  if ( bit >= 0xf0 )
  {
    bit  = ( int )( string[*i]     & 0x07 ) << 18;
    bit |= ( int )( string[*i + 1] & 0x3f ) << 12;
    bit |= ( int )( string[*i + 2] & 0x3f ) << 6;
    bit |= ( int )( string[*i + 3] & 0x3f );
    
    len = 4;
  }

  else if ( bit >= 0xe0 )
  {
    bit  = ( int )( string[*i]     & 0x0f ) << 12;
    bit |= ( int )( string[*i + 1] & 0x3f ) << 6;
    bit |= ( int )( string[*i + 2] & 0x3f );

    len = 3;
  }

  else if ( bit >= 0xc0 )
  {
    bit  = ( int )( string[*i]     & 0x1f ) << 6;
    bit |= ( int )( string[*i + 1] & 0x3f );

    len = 2;
  }

  if ( glyph_buffer != NULL )
  {
    p = string + *i;

    memset( glyph_buffer, 0, MAX_GLYPH_SIZE );

    memcpy( glyph_buffer, p, len );
  }
  *i = *i + len;
  
  return bit;
}

