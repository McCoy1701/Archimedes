#include <stdio.h>

#include "Archimedes.h"

static void e_WorldEditorDoLoop( float );
static void e_WorldEditorRenderLoop( float );
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

void aInitGame( void )
{
  app.delegate.logic = e_WorldEditorDoLoop;
  app.delegate.draw  = e_WorldEditorRenderLoop;
  
  a_InitWidgets( "resources/widgets/world.json" );
  
  app.active_widget = a_GetWidget( "tab_bar" );

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
  
  w = a_GetWidget( "menu_bar" );
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

  a_DoWidget();

}

static void we_CreationRenderLoop( float dt )
{
//  a_DrawFilledRect( 240, 240, 400, 265, 255, 0, 255, 255 );
  
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

}

static void item( void )
{

}

static void entity( void )
{

}

static void color( void )
{

}

static void ui( void )
{

}

static void e_WorldEditorDoLoop( float dt )
{
  a_DoInput();
  
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
    app.running = 0;
  }

  a_DoWidget();
}

static void e_WorldEditorRenderLoop( float dt )
{
  a_DrawFilledRect( 100, 100, 32, 32, 255, 0, 255, 255 );
  a_DrawFilledRect( 300, 300, 32, 32, 0, 255, 255, 255 );
  a_DrawText( "Whore", 400, 250, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0 );

  a_DrawWidgets();
}

void aMainloop( void )
{
  a_PrepareScene();

  app.delegate.logic( a_GetDeltaTime() );
  app.delegate.draw( a_GetDeltaTime() );
  
  a_PresentScene();
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
