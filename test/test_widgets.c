#include <stdio.h>

#include "Archimedes.h"

static void e_Logic( float );
static void e_Draw( float );
static void we_CreationDoLoop( float );
static void we_CreationRenderLoop( float );

static void world( void );
static void item( void );
static void entity( void );
static void color( void );
static void ui( void );

static void we_edit( void );
static void we_load( void );
static void we_save( void );
static void we_creation( void );

static aWidget_t* w;
aAUF_t* root = NULL;

int draw_box = 0;
aTimer_t* one_shot = NULL;

void aInitGame( void )
{
  app.delegate.logic = e_Logic;
  app.delegate.draw  = e_Draw;
    
  //root = a_AUFParser( "resources/widgets/world.auf" );
  //a_PrintAUFTree( root->head, 10 );

  one_shot = a_TimerCreate();
  
  a_InitWidgets( "resources/widgets/world.auf" );

  app.active_widget = a_GetWidget( "tab_bar" );
  app.options.frame_cap = 1;
  
  aContainerWidget_t* container = ( aContainerWidget_t* )app.active_widget->data;
  for ( int i = 0; i < container->num_components; i++ )
  {
    aWidget_t* current = &container->components[i];

    if ( strcmp( current->name, "world" ) == 0 )
    {
      current->action = world;
    }
    
    if ( strcmp( current->name, "item" ) == 0 )
    {
      current->action = item;
    }
    
    if ( strcmp( current->name, "entity" ) == 0 )
    {
      current->action = entity;
    }
    
    if ( strcmp( current->name, "colors" ) == 0 )
    {
      current->action = color;
    }
    
    if ( strcmp( current->name, "ui" ) == 0 )
    {
      current->action = ui;
    }
  }
  
  w = a_GetWidget( "world_menu_bar" );
  container = ( aContainerWidget_t* )w->data;
  for ( int i = 0; i < container->num_components; i++ )
  {
    aWidget_t* current = &container->components[i];

    if ( strcmp( current->name, "creation" ) == 0 )
    {
      current->action = we_creation;
    }
    
    if ( strcmp( current->name, "edit" ) == 0 )
    {
      current->action = we_edit;
    }
    
    if ( strcmp( current->name, "save" ) == 0 )
    {
      current->action = we_save;
    }
    
    if ( strcmp( current->name, "load" ) == 0 )
    {
      current->action = we_load;
    }
  }
}

static void we_creation( void )
{
  app.delegate.logic = we_CreationDoLoop;
  app.delegate.draw  = we_CreationRenderLoop;
  
  app.active_widget = a_GetWidget( "generation_menu" );
  aContainerWidget_t* container = ( aContainerWidget_t* )app.active_widget->data;
  app.active_widget->hidden = 0;
  
  for ( int i = 0; i < container->num_components; i++ )
  {
    aWidget_t* current = &container->components[i];
    current->hidden = 0;
  }

}

static void we_CreationDoLoop( float dt )
{
  a_DoInput();
  
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
    aInitGame();
  }
  
  if ( draw_box )
  {
    if ( a_TimerOneshot( one_shot, 5000 ) == 1 )
    {
      draw_box = 0;
    }
  }

  a_DoWidget();

}

static void we_CreationRenderLoop( float dt )
{
  if ( draw_box )
  {
    aColor_t color_something = { .r = 0, .g = 255, .b = 255, .a = 255 };
    aRectf_t rect_something = { .x = 500, .y = 100, .w = 32, .h = 32 };
    a_DrawFilledRect( rect_something, color_something );
    
    char timer_text[MAX_NAME_LENGTH];
    snprintf( timer_text, MAX_NAME_LENGTH, "%d", a_TimerGetTicks( one_shot ) );

    a_DrawText( timer_text, 600, 100, &a_default_text_style );
  }

  a_DrawWidgets();
}

static void we_edit( void )
{

}

static void we_save( void )
{

}

static void we_load( void )
{

}

static void world( void )
{
  printf("world\n");
  draw_box = 1;
}

static void item( void )
{

}

static void entity( void )
{

}

static void color( void )
{
  app.delegate.logic = we_CreationDoLoop;
  app.delegate.draw  = we_CreationRenderLoop;

}

static void ui( void )
{
  printf("Hello, World!\n");

}

static void e_Logic( float dt )
{
  a_DoInput();
  
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
    app.running = 0;
  }

  a_DoWidget();
  //printf( "DT: %f\n", dt );
}

static void e_Draw( float dt )
{
  aColor_t color_something = { .r = 0, .g = 0, .b = 255, .a = 255 };
  aRectf_t rect_something = { .x = 100, .y = 100, .w = 32, .h = 32 };
  a_DrawFilledRect( rect_something, color_something );
  
  char fps_text[MAX_NAME_LENGTH];
  snprintf(fps_text, MAX_NAME_LENGTH, "%f", app.time.avg_FPS );
  
  aTextStyle_t fps_text_style = (aTextStyle_t){
    .fg = white,
    .bg = black,
    .type = FONT_CODE_PAGE_437,
    .align = TEXT_ALIGN_CENTER,
    .padding = 0,
    .scale = 0,
    .wrap_width = 0,
  };

  a_DrawText( fps_text, 600, 100, &fps_text_style );

  a_DrawWidgets();
}

void aMainloop( void )
{
  float dt = a_GetDeltaTime();
  a_TimerStart( app.time.FPS_cap_timer );
  a_GetFPS();
  a_PrepareScene();
  
  app.delegate.logic( dt );
  app.delegate.draw( dt );
  
  a_PresentScene();
  app.time.frames++;
  
  if ( app.options.frame_cap )
  {
    int frame_tick = a_TimerGetTicks( app.time.FPS_cap_timer );
    if ( frame_tick < LOGIC_RATE )
    {
      SDL_Delay( LOGIC_RATE - frame_tick );
    }
  }
}

int main( void )
{
  a_Init( SCREEN_WIDTH, SCREEN_HEIGHT, "Archimedes" );

  aInitGame();

  while( app.running ) {
    aMainloop();
  }
  
  a_Quit();

  return 0;
}

