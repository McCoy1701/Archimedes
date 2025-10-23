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

void aInitGame( void )
{
  app.delegate.logic = e_Logic;
  app.delegate.draw  = e_Draw;
  
  aAUF_t* root = a_AUFParser( "resources/widgets/world.auf" );
  
  aAUF_Node_t* current = root->head;
  if ( 1 )
  {
    while ( current != NULL )
    {
      if ( 1 )
      {
        printf( "Name: %s %s\n", current->string, current->value_string );
        printf( "x: %d\n", a_GetObjectItem( current, "x" )->value_int );
        printf( "y: %d\n", a_GetObjectItem( current, "y" )->value_int );
        printf( "label: %s\n", a_GetObjectItem( current, "label" )->value_string );
        printf( "boxed: %d\n", a_GetObjectItem( current, "boxed" )->value_int );
        printf( "hidden: %d\n", a_GetObjectItem( current, "hidden" )->value_int );
        printf( "padding: %d\n", a_GetObjectItem( current, "padding" )->value_int );
        printf( "flex: %d\n", a_GetObjectItem( current, "flex" )->value_int );
        printf( "spacing: %d\n", a_GetObjectItem( current, "spacing" )->value_int );
        
        if ( a_GetObjectItem( current, "fg" ) != NULL )
        {
          printf( "fg: " );
          aAUF_Node_t* fg = a_GetObjectItem( current, "fg" );
          aAUF_Node_t* temp = fg->child;
          for ( int i = 0; i < fg->value_int; i++ )
          {
            if ( temp != NULL )
            {
              printf( "%d,", temp->value_int );

            }
            temp = temp->next;
          }
          printf( "\n" );

        }

        if ( a_GetObjectItem( current, "bg" ) != NULL )
        {
          printf( "bg: " );
          aAUF_Node_t* bg = a_GetObjectItem( current, "bg" );
          aAUF_Node_t* temp = bg->child;
          for ( int i = 0; i < bg->value_int; i++ )
          {
            if ( temp != NULL )
            {
              printf( "%d,", temp->value_int );

            }
            temp = temp->next;
          }
          printf( "\n" );


        }
      }

      aAUF_Node_t* container = a_GetObjectItem( current, "container" );

      while ( container != NULL )
      {
        if ( a_GetObjectItem( container, "x" ) != NULL )
        {
          printf( "x: %d\n", a_GetObjectItem( container, "x" )->value_int );

        }
        
        if ( a_GetObjectItem( container, "y" ) != NULL )
        {
          printf( "y: %d\n", a_GetObjectItem( container, "y" )->value_int );

        }
        
        if ( a_GetObjectItem( container, "label" ) != NULL )
        {
          printf( "label: %s\n", a_GetObjectItem( container, "label" )->value_string );

        }
        
        if ( a_GetObjectItem( container, "boxed" ) != NULL )
        {
          printf( "boxed: %d\n", a_GetObjectItem( container, "boxed" )->value_int );

        }
        
        if ( a_GetObjectItem( container, "hidden" ) != NULL )
        {
          printf( "hidden: %d\n", a_GetObjectItem( container, "hidden" )->value_int );

        }
        
        if ( a_GetObjectItem( container, "padding" ) != NULL )
        {
          printf( "padding: %d\n", a_GetObjectItem( container, "padding" )->value_int );

        }
        
        if ( a_GetObjectItem( container, "fg" ) != NULL )
        {
          printf( "fg: " );
          aAUF_Node_t* fg = a_GetObjectItem( container, "fg" );
          aAUF_Node_t* current = fg->child;
          for ( int i = 0; i < fg->value_int; i++ )
          {
            if ( current != NULL )
            {
              printf( "%d,", current->value_int );

            }
            current = current->next;
          }
          printf( "\n" );

        }

        if ( a_GetObjectItem( container, "bg" ) != NULL )
        {
          printf( "bg: " );
          aAUF_Node_t* bg = a_GetObjectItem( container, "bg" );
          aAUF_Node_t* current = bg->child;
          for ( int i = 0; i < bg->value_int; i++ )
          {
            if ( current != NULL )
            {
              printf( "%d,", current->value_int );

            }
            current = current->next;
          }
          printf( "\n" );


        }
        
        if ( a_GetObjectItem( container, "options" ) != NULL )
        {
          printf( "options: " );
          aAUF_Node_t* options = a_GetObjectItem( container, "options" );
          aAUF_Node_t* current = options->child;
          for ( int i = 0; i < options->value_int; i++ )
          {
            if ( current != NULL )
            {
              printf( "%s, ", current->value_string );

            }
            current = current->next;
          }
          printf( "\n" );


        }

        container = container->next;
      }

      current = current->next;
    }
  }

  //a_AUFFree( root );
  
  /*a_InitWidgets( "resources/widgets/world.json" );
  
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
  */

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

static void e_Logic( float dt )
{
  a_DoInput();
  
  if ( app.keyboard[ SDL_SCANCODE_ESCAPE ] == 1 )
  {
    app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
    app.running = 0;
  }

  a_DoWidget();
}

static void e_Draw( float dt )
{
  //a_DrawFilledRect( 100, 100, 32, 32, 255, 0, 255, 255 );
  //a_DrawFilledRect( 300, 300, 32, 32, 0, 255, 255, 255 );
  //a_DrawText( "Whore", 400, 250, 255, 255, 255, FONT_LINUX, TEXT_ALIGN_CENTER, 0 );

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
  //a_FreeAUF();
  a_Quit();

  return 0;
}
