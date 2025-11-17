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

void aInitGame( void )
{
  app.delegate.logic = e_Logic;
  app.delegate.draw  = e_Draw;
  
  root = a_AUFParser( "resources/widgets/world.auf" );

  aAUF_Node_t* current = root->head;
  //a_PrintAUFTree( current, 1 );
  
  /*if ( 1 )
  {
    while ( current != NULL )
    {
      if ( 1 )
      {
        printf( "Name: %s %s\n", current->string, current->value_string );
        printf( "x: %d\n", a_AUFGetObjectItem( current, "x" )->value_int );
        printf( "y: %d\n", a_AUFGetObjectItem( current, "y" )->value_int );
        printf( "label: %s\n", a_AUFGetObjectItem( current, "label" )->value_string );
        printf( "boxed: %d\n", a_AUFGetObjectItem( current, "boxed" )->value_int );
        printf( "hidden: %d\n", a_AUFGetObjectItem( current, "hidden" )->value_int );
        printf( "padding: %d\n", a_AUFGetObjectItem( current, "padding" )->value_int );
        printf( "flex: %d\n", a_AUFGetObjectItem( current, "flex" )->value_int );
        printf( "spacing: %d\n", a_AUFGetObjectItem( current, "spacing" )->value_int );
        printf( "current type: %d\n", current->type );
        
        if ( a_AUFGetObjectItem( current, "fg" ) != NULL )
        {
          printf( "fg: " );
          aAUF_Node_t* fg = a_AUFGetObjectItem( current, "fg" );
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

        if ( a_AUFGetObjectItem( current, "bg" ) != NULL )
        {
          printf( "bg: " );
          aAUF_Node_t* bg = a_AUFGetObjectItem( current, "bg" );
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
      
      aAUF_Node_t* container = a_AUFGetObjectItem( current, "container" );
      printf( "name count: %s %d\n", container->string, container->value_int );
      aAUF_Node_t* current_container = container->child;

      if ( 1 )
      {
        while ( current_container != NULL )
        {

          if ( current_container->string != NULL )
          {
            printf( "Type, Name: %s %s\n", current_container->string, current_container->value_string );
          }

          if ( a_AUFGetObjectItem( current_container, "x" ) != NULL )
          {
            printf( "x: %d\n", a_AUFGetObjectItem( current_container, "x" )->value_int );

          }

          if ( a_AUFGetObjectItem( current_container, "y" ) != NULL )
          {
            printf( "y: %d\n", a_AUFGetObjectItem( current_container, "y" )->value_int );

          }

          if ( a_AUFGetObjectItem( current_container, "label" ) != NULL )
          {
            printf( "label: %s\n", a_AUFGetObjectItem( current_container, "label" )->value_string );

          }

          if ( a_AUFGetObjectItem( current_container, "boxed" ) != NULL )
          {
            printf( "boxed: %d\n", a_AUFGetObjectItem( current_container, "boxed" )->value_int );

          }

          if ( a_AUFGetObjectItem( current_container, "hidden" ) != NULL )
          {
            printf( "hidden: %d\n", a_AUFGetObjectItem( current_container, "hidden" )->value_int );

          }

          if ( a_AUFGetObjectItem( current_container, "padding" ) != NULL )
          {
            printf( "padding: %d\n", a_AUFGetObjectItem( current_container, "padding" )->value_int );

          }
          
          printf( "container type: %d\n", current_container->type );

          if ( a_AUFGetObjectItem( current_container, "fg" ) != NULL )
          {
            printf( "fg: " );
            aAUF_Node_t* fg = a_AUFGetObjectItem( current_container, "fg" );
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

          if ( a_AUFGetObjectItem( current_container, "bg" ) != NULL )
          {
            printf( "bg: " );
            aAUF_Node_t* bg = a_AUFGetObjectItem( current_container, "bg" );
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

          if ( a_AUFGetObjectItem( current_container, "options" ) != NULL )
          {
            printf( "options: " );
            aAUF_Node_t* options = a_AUFGetObjectItem( current_container, "options" );
            aAUF_Node_t* current = options->child;
            
            printf("options count: %d\n", options->value_int );

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

          current_container = current_container->next;
        }
      }

      current = current->next;
    }
  }*/

  //a_AUFFree( root );
  
  a_InitWidgets( "resources/widgets/world.auf" );
  
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
  aColor_t color_something = { .r = 255, .g = 0, .b = 255, .a = 255 };
  aRectf_t rect_something = { .x = 240, .y = 240, .w = 400, .h = 265 };
  a_DrawFilledRect( rect_something, color_something );
  
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
  //printf( "DT: %f\n", dt );
}

static void e_Draw( float dt )
{
  aColor_t color_something = { .r = 0, .g = 0, .b = 255, .a = 255 };
  aRectf_t rect_something = { .x = 100, .y = 100, .w = 32, .h = 32 };
  a_DrawFilledRect( rect_something, color_something );
  
  char fps_text[MAX_NAME_LENGTH];
  snprintf(fps_text, MAX_NAME_LENGTH, "%f", app.time.avg_FPS );

  a_DrawText( fps_text, 600, 100, black, white, FONT_CODE_PAGE_437, TEXT_ALIGN_CENTER, 0 );

  a_DrawWidgets();
}

void aMainloop( void )
{
  float dt = a_GetDeltaTime();
  a_GetFPS();
  a_PrepareScene();
  
  app.delegate.logic( dt );
  app.delegate.draw( dt );
  
  a_PresentScene();
  app.time.frames++;
  
  if ( app.options.frame_cap )
  {
    int frame_tick = SDL_GetTicks();
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
  
  a_AUFFree( root );
  
  a_Quit();

  return 0;
}

