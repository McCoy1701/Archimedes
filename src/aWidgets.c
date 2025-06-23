#include <stdio.h>
#include <stdlib.h>
#include "Archimedes.h"

#ifndef __EMSCRIPTEN__
#include <cjson/cJSON.h>
#endif
#ifdef __EMSCRIPTEN__
#include "cJSON.h"
#endif

static aWidget_t widget_head;
static aWidget_t* widget_tail;

static void LoadWidgets( const char* filename );
static int GetWidgetType( char* type );
static void ChangeWidgetValue( int value );
static void CreateWidget( cJSON* root );
static char* ReadFile( const char* filename );

static void CreateButtonWidget( aWidget_t* w, cJSON* root );
static void CreateSelectWidget( aWidget_t* w, cJSON* root );
static void CreateSliderWidget( aWidget_t* w, cJSON* root );
static void CreateInputWidget( aWidget_t* w, cJSON* root );
static void CreateControlWidget( aWidget_t* w, cJSON* root );
static void CreateContainerWidget( aWidget_t* w, cJSON* root );
static void DrawButtonWidget( aWidget_t* w );
static void DrawSelectWidget( aWidget_t* w );
static void DrawSliderWidget( aWidget_t* w );
static void DrawInputWidget( aWidget_t* w );
static void DrawControlWidget( aWidget_t* w );
static void DrawContainerWidget( aWidget_t* w );

static void DoInputWidget( void );
static void DoControlWidget( void );

static double slider_delay;
static double cursor_blink;
static int handle_input_widget;
static int handle_control_widget;

void a_DoWidget( void )
{
  aWidget_t* temp;
  int r,g,b = 0;

  slider_delay = MAX( slider_delay - a_GetDeltaTime(), 0 );

  cursor_blink += a_GetDeltaTime();

  if ( !handle_input_widget && !handle_control_widget )
  {
    if ( app.mouse.button == 1 )  //left mouse click
    {
      app.mouse.button = 0;
      aWidget_t* current = &widget_head;
      
      while ( current != NULL )
      {
        if ( current->hidden == 0 )
        {
          if ( app.mouse.x >= current->x && app.mouse.y >= current->y &&
            app.mouse.x <= ( current->x + current->w ) && 
            app.mouse.y <= ( current->y + current->h ) )
          {
            app.active_widget = current;

            if ( current->type == WT_CONTAINER )
            {
              aContainerWidget_t* container = ( aContainerWidget_t* )current->data;

              for ( int i = 0; i < container->num_components; i++ )
              {
                aWidget_t* component = &container->components[i];

                if ( component->hidden == 0 )
                {
                  if ( app.mouse.x >= component->x && app.mouse.y >= component->y &&
                       app.mouse.x <= ( component->x + component->w ) &&
                       app.mouse.y <= ( component->y + component->h ) )
                  {
                    if ( component->action != NULL )
                    {
                      component->action();
                    }
                    app.active_widget = component;
                    printf( "Active: %s\n", app.active_widget->name );
                    return;
                  }
                }
              }

            }
 
            else
            {
              if ( current->action != NULL )
              {
                current->action();
              }
              app.active_widget = current;
              printf( "Active: %s\n", app.active_widget->name );
              return;
            }
          }
        }
      
        current = current->next;
      }
    }

    /*if ( app.keyboard[SDL_SCANCODE_UP] )
    {
      app.keyboard[SDL_SCANCODE_UP] = 0;
      if ( app.active_widget->prev->hidden == 1 )
      {
        temp = app.active_widget;
        while ( temp != NULL && temp->hidden != 1 )
        {
          temp = temp->prev;
        }

        if ( temp != NULL )
        {
          app.active_widget = temp;

        }

      }

      else
      {
        app.active_widget = app.active_widget->prev;
      }

      if ( app.active_widget == &widget_head )
      {
        app.active_widget = widget_tail;
      }
    }

    if ( app.keyboard[SDL_SCANCODE_DOWN] )
    {
      app.keyboard[SDL_SCANCODE_DOWN] = 0;

      if ( app.active_widget->next != NULL )
      {
        if ( app.active_widget->next->hidden == 1 )
        {
          temp = app.active_widget;
          while ( temp != NULL && temp->hidden != 1 )
          {
            temp = temp->next;
          }

          if ( temp != NULL )
          {
            app.active_widget = temp;

          }
        }
  
        else
        {
          app.active_widget = app.active_widget->next;
        }
      }
  
      else
      {
        app.active_widget = widget_head.next;
      }

      if ( app.active_widget == NULL )
      {
        app.active_widget = widget_head.next;
      }
    }*/

    if ( app.keyboard[SDL_SCANCODE_LEFT] )
    {
      app.keyboard[SDL_SCANCODE_LEFT] = 0;
      ChangeWidgetValue( -1 );
    }
    
    if ( app.keyboard[SDL_SCANCODE_RIGHT] )
    {
      app.keyboard[SDL_SCANCODE_RIGHT] = 0;
      ChangeWidgetValue( 1 );
    }

    if ( app.keyboard[SDL_SCANCODE_SPACE] || app.keyboard[SDL_SCANCODE_RETURN] )
    { 
      if ( app.active_widget->type == WT_INPUT )
      {
        cursor_blink = 0;
        handle_input_widget = 1;
        memset( app.input_text, 0, sizeof( app.input_text ) );
      }
      
      else if ( app.active_widget->type == WT_CONTROL )
      {
        app.last_key_pressed = -1;
        handle_control_widget = 1;
      }

      else if ( app.active_widget->action != NULL )
      {
        app.active_widget->action();
      }
    }
  }

  else if ( handle_input_widget )
  {
    DoInputWidget();
  }

  else if( handle_control_widget )
  {
    DoControlWidget();
  }
}


void a_DrawWidgets( void )
{
  aWidget_t* w;
  for ( w = widget_head.next; w != NULL; w = w->next )
  {
    switch ( w->type )
    {
      case WT_BUTTON:
        DrawButtonWidget( w );
        break;
      
      case WT_SELECT:
        DrawSelectWidget( w );
        break;
      
      case WT_SLIDER:
        DrawSliderWidget( w );
        break;
      
      case WT_INPUT:
        DrawInputWidget( w );
        break;
      
      case WT_CONTROL:
        DrawControlWidget( w );
        break;
      
      case WT_CONTAINER:
        DrawContainerWidget( w );
        break;

      default:
        break;
    }
  }
}

void a_InitWidgets( const char* filename )
{
  memset( &widget_head, 0, sizeof( aWidget_t ) );
  widget_tail = &widget_head;

  LoadWidgets( filename );
  
  slider_delay = 0;
  cursor_blink = 0;
  handle_input_widget = 0;
  handle_control_widget = 0;
}

aWidget_t* a_GetWidget( char* name )
{
  aWidget_t* w;

  for ( w = widget_head.next; w != NULL; w = w->next )
  {
    if ( strcmp( w->name, name ) == 0 )
    {
      return w;
    }
  }

  SDL_LogMessage( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "No such widget: '%s'", name);

  return NULL;
}

static void LoadWidgets( const char* filename )
{
  cJSON* root, *node;
  char* text;

  text = ReadFile( filename );

  root = cJSON_Parse( text );

  for ( node = root->child; node != NULL; node = node->next )
  {
    CreateWidget( node );
  }

  cJSON_Delete( root );

  free( text );
}

static int GetWidgetType( char* type )
{
  if ( strcmp( type, "WT_BUTTON" ) == 0 )
  {
    return WT_BUTTON;
  }

  if ( strcmp( type, "WT_SELECT" ) == 0 )
  {
    return WT_SELECT;
  }

  if ( strcmp( type, "WT_SLIDER" ) == 0 )
  {
    return WT_SLIDER;
  }

  if ( strcmp( type, "WT_INPUT" ) == 0 )
  {
    return WT_INPUT;
  }

  if ( strcmp( type, "WT_CONTROL" ) == 0 )
  {
    return WT_CONTROL;
  }
  
  if ( strcmp( type, "WT_CONTAINER" ) == 0 )
  {
    return WT_CONTAINER;
  }

  SDL_LogMessage( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "unknown widget type: '%s'", type );

  return -1;
}

static void DoInputWidget( void )
{
  aInputWidget_t* input;
  int i, l, n;
  input = ( aInputWidget_t* )app.active_widget->data;

  l = strlen( input->text );
  n = strlen( app.input_text );

  if ( n + l > input->max_length )
  {
    n = input->max_length - l;
  }

  for (i = 0; i < n; i++ )
  {
    input->text[l++] = app.input_text[i];
  }

  memset( app.input_text, 0, sizeof( app.input_text ) );

  if ( l > 0 && app.keyboard[SDL_SCANCODE_BACKSPACE] )
  {
    input->text[--l] = '\0';
    app.keyboard[SDL_SCANCODE_BACKSPACE] = 0;
  }

  if ( app.keyboard[SDL_SCANCODE_RETURN] || app.keyboard[SDL_SCANCODE_ESCAPE] )
  {
    app.keyboard[SDL_SCANCODE_RETURN] = app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
    handle_input_widget = 0;
    if ( app.active_widget->action != NULL )
    {
      app.active_widget->action();
    }
  }
}

static void DoControlWidget( void )
{
  if ( app.last_key_pressed != -1 )
  {
    if ( app.last_key_pressed != SDL_SCANCODE_ESCAPE )
    {
      ( ( aControlWidget_t* )app.active_widget->data )->value = app.last_key_pressed;

      if ( app.active_widget->action != NULL )
      {
        app.active_widget->action();
      }
    }
    handle_control_widget = 0;

    app.keyboard[app.last_key_pressed] = 0;
  }
}

static void ChangeWidgetValue( int value )
{
  aSelectWidget_t* select;
  aSliderWidget_t* slider;

  switch (app.active_widget->type)
  {
    case WT_SELECT:
      select = ( aSelectWidget_t* ) app.active_widget->data;
      select->value += value;

      if ( select->value < 0 )
      {
        select->value = select->num_options - 1;
      }

      if ( select->value >= select->num_options )
      {
        select->value = 0;
      }

      if ( app.active_widget->action != NULL )
      {
        app.active_widget->action();
      }

      break;

    case WT_SLIDER:
      slider = ( aSliderWidget_t* )app.active_widget->data;
      
      if ( slider_delay == 0 || slider->wait_on_change )
      {
        if ( slider->wait_on_change )
        {
          app.keyboard[SDL_SCANCODE_LEFT] = app.keyboard[SDL_SCANCODE_RIGHT] = 0;
        }

        slider->value = MIN( MAX( slider->value + ( slider->step * value ), 0 ), 100 );

        slider_delay = 1;

        if ( app.active_widget->action != NULL )
        {
          app.active_widget->action();
        }
      }
      break;
  
    default:
      break;
  }
}

static void CreateWidget( cJSON* root )
{
  aWidget_t* w;
  cJSON* object, *node;
  int type, i;

  type = GetWidgetType( cJSON_GetObjectItem( root, "type" )->valuestring );

  if ( type != -1)
  {
    w = malloc( sizeof( aWidget_t ) );
    memset( w, 0, sizeof( aWidget_t ) );

    widget_tail->next = w;
    w->prev = widget_tail;
    widget_tail = w;

    STRCPY( w->name, cJSON_GetObjectItem( root, "name" )->valuestring );
    STRCPY( w->label, cJSON_GetObjectItem( root, "label" )->valuestring );
    w->type = GetWidgetType( cJSON_GetObjectItem( root, "type" )->valuestring );
    w->x = cJSON_GetObjectItem( root, "x" )->valueint;
    w->y = cJSON_GetObjectItem( root, "y" )->valueint;
    w->boxed = cJSON_GetObjectItem( root, "boxed" )->valueint;
    w->hidden = cJSON_GetObjectItem( root, "hidden" )->valueint;
    w->padding = cJSON_GetObjectItem( root, "padding" )->valueint;

    object = cJSON_GetObjectItem( root, "fg");
    i = 0;
    for ( node = object->child; node != NULL; node = node->next )
    {
      w->fg[i++] = node->valueint;
    }

    object = cJSON_GetObjectItem( root, "bg");
    i = 0;
    for ( node = object->child; node != NULL; node = node->next )
    {
      w->bg[i++] = node->valueint;
    }

    switch ( w->type )
    {
      case WT_BUTTON:
        CreateButtonWidget( w, root );
        break;

      case WT_SELECT:
        CreateSelectWidget( w, root );
        break;
      
      case WT_SLIDER:
        CreateSliderWidget( w, root );
        break;
      
      case WT_INPUT:
        CreateInputWidget( w, root );
        break;
      
      case WT_CONTROL:
        CreateControlWidget( w, root );
        break;
      
      case WT_CONTAINER:
        CreateContainerWidget( w, root );
        break;

      default:
        break;
    }
  }
}

static void CreateButtonWidget( aWidget_t* w, cJSON* root )
{
  a_CalcTextDimensions( w->label, app.font_type, &w->w, &w->h );
}

static void CreateSelectWidget( aWidget_t* w, cJSON* root )
{
  cJSON* options, *node;
  int i, len, temp_w, temp_h, width, height;
  aSelectWidget_t* s;

  s = malloc( sizeof( aSelectWidget_t ) );
  memset( s, 0, sizeof( aSelectWidget_t ) );
  w->data = s;

  options = cJSON_GetObjectItem( root, "options" );

  s->num_options = cJSON_GetArraySize( options );
  
  temp_w = temp_h = width = height = 0;

  if ( s->num_options > 0 )
  {
    i = 0;

    s->options = malloc( sizeof(char*) * s->num_options );

    for( node = options->child; node != NULL; node = node->next )
    {
      len = strlen( node->valuestring ) + 1;

      s->options[i] = malloc(len);

      STRNCPY( s->options[i], node->valuestring, len );
      a_CalcTextDimensions( s->options[i], app.font_type, &width, &height );
      
      if ( width > temp_w )
      {
        temp_w = width; //Get the largest width
      }
      
      if ( height > temp_h )
      {
        temp_h = height;
      }

      i++;
    }
  }

  a_CalcTextDimensions( w->label, app.font_type, &w->w, &w->h );

  s->x = w->x + 100;
  s->y = w->y;
  s->w = temp_w;
  s->h = temp_h;
}

static void CreateSliderWidget( aWidget_t* w, cJSON* root )
{
  aSliderWidget_t* s;
  s = malloc( sizeof( aSliderWidget_t ) );
  memset( s, 0, sizeof( aSliderWidget_t ) );
  w->data = s;

  s->step = cJSON_GetObjectItem( root, "step" )->valueint;
  s->wait_on_change = cJSON_GetObjectItem( root, "wait_on_change" )->valueint;

  a_CalcTextDimensions( w->label, app.font_type, &w->w, &w->h );
  s->x = w->x + w->w + 50;
	s->y = w->y;
	s->w = w->w;
	s->h = w->h;
}

static void CreateInputWidget( aWidget_t* w, cJSON* root )
{
  aInputWidget_t* input;

  input = malloc( sizeof( aInputWidget_t ) );
  memset( input, 0, sizeof( aInputWidget_t ) );

  w->data = input;

  input->max_length = cJSON_GetObjectItem( root, "max_length" )->valueint;
  input->text = malloc(  input->max_length + 1 );

  STRNCPY( input->text, "Player", MAX_INPUT_LENGTH );

  a_CalcTextDimensions( w->label, app.font_type, &w->w, &w->h );
  input->x = w->x + w->w + 50;
  input->y = w->y;
  a_CalcTextDimensions( input->text, app.font_type, &input->w, &input->h );
}

static void CreateControlWidget( aWidget_t* w, cJSON* root )
{
  aControlWidget_t* control;

  control = malloc( sizeof( aControlWidget_t ) );
  memset( control, 0, sizeof( aControlWidget_t ) );

  w->data = control;
  a_CalcTextDimensions( w->label, app.font_type, &w->w, &w->h );
}

static void CreateContainerWidget( aWidget_t* w, cJSON* root )
{
  cJSON* object, *node;
  int i, index;
  int temp_x, temp_y;
  aContainerWidget_t* container;
  aInputWidget_t* input;
  aSliderWidget_t* slider;
  aSelectWidget_t* select;
  
  container = malloc( sizeof( aContainerWidget_t ) );
  memset( container, 0, sizeof( aContainerWidget_t ) );
  
  w->data = container;
  w->flex = cJSON_GetObjectItem( root, "flex" )->valueint;
  container->spacing = cJSON_GetObjectItem( root, "spacing" )->valueint;
  w->w = w->h = 0;

  object = cJSON_GetObjectItem( root, "components" );
  container->num_components = cJSON_GetArraySize( object );

  container->components = malloc( sizeof( aWidget_t ) * container->num_components );
  if ( container->components == NULL )
  {
    printf("Failed to allocate memory for components\n");
    exit(1);
  }

  i = 0;
  temp_x = w->x;
  temp_y = w->y;

  int max_component_x_plus_w = 0;
  int max_component_y_plus_h = 0;

  for ( node = object->child; node != NULL; node = node->next )
  {
    aWidget_t* current = &container->components[i];
    STRCPY( current->name, cJSON_GetObjectItem( node, "name" )->valuestring );
    STRCPY( current->label, cJSON_GetObjectItem( node, "label" )->valuestring );
    current->type = GetWidgetType( cJSON_GetObjectItem( node, "type" )->valuestring );
    current->boxed = cJSON_GetObjectItem( node, "boxed" )->valueint;
    current->hidden = cJSON_GetObjectItem( node, "hidden" )->valueint;
    current->padding = cJSON_GetObjectItem( node, "padding" )->valueint;

    cJSON* object_1, *node_1;
    int j;
    object_1 = cJSON_GetObjectItem( node, "fg");
    j = 0;
    for ( node_1 = object_1->child; node_1 != NULL; node_1 = node_1->next )
    {
      current->fg[j++] = node_1->valueint;
    }

    object_1 = cJSON_GetObjectItem( node, "bg");
    j = 0;
    for ( node_1 = object_1->child; node_1 != NULL; node_1 = node_1->next )
    {
      current->bg[j++] = node_1->valueint;
    }
    
    a_CalcTextDimensions( current->label, app.font_type, &current->w, &current->h );

    if ( w->flex == 1 )
    {
      current->x = temp_x;
      current->y = temp_y;

      temp_x += ( current->w + container->spacing );
    }
  
    else
    {
      current->x = cJSON_GetObjectItem( node, "x" )->valueint;
      current->y = cJSON_GetObjectItem( node, "y" )->valueint;
    }

    int widget_effective_w = current->w;
    int widget_effective_h = current->h;

    int current_widget_max_x_extent = current->x + current->w;
    int current_widget_max_y_extent = current->y + current->h;
    
    switch ( current->type )
    {
      case WT_BUTTON:
        CreateButtonWidget( current, node );
        current_widget_max_x_extent = current->x + current->w;
        current_widget_max_y_extent = current->y + current->h;
        break;

      case WT_SELECT:
        CreateSelectWidget( current, node );
        select = (aSelectWidget_t*)current->data;

        current_widget_max_x_extent = MAX( current_widget_max_x_extent, ( select->x + select->w ) );
        current_widget_max_y_extent = MAX( current_widget_max_y_extent, ( select->y + select->h ) );
        break;
      
      case WT_SLIDER:
        CreateSliderWidget( current, node );
        slider = (aSliderWidget_t*)current->data;

        current_widget_max_x_extent = MAX( current_widget_max_x_extent, ( slider->x + slider->w ) );
        current_widget_max_y_extent = MAX( current_widget_max_y_extent, ( slider->y + slider->h ) );
        break;
      
      case WT_INPUT:
        CreateInputWidget( current, node );
        input = (aInputWidget_t*)current->data;

        current_widget_max_x_extent = MAX( current_widget_max_x_extent, ( input->x + input->w ) );
        current_widget_max_y_extent = MAX( current_widget_max_y_extent, ( input->y + input->h ) );
        break;
      
      case WT_CONTROL:
        CreateControlWidget( current, node );
        break;
      
      case WT_CONTAINER:
        CreateContainerWidget( current, node );
        break;

      default:
        break;
    }

    widget_effective_w = current_widget_max_x_extent - current->x;
    widget_effective_h = current_widget_max_y_extent - current->y;

    if ( w->flex == 1 )
    {
      //temp_x += ( widget_effective_w + container->spacing );
    }

    if ( current_widget_max_x_extent > max_component_x_plus_w )
    {
      max_component_x_plus_w = current_widget_max_x_extent;
    }
    
    if ( current_widget_max_y_extent > max_component_y_plus_h )
    {
      max_component_y_plus_h = current_widget_max_y_extent;
    }
    
    i++;
  }

  w->w = max_component_x_plus_w - w->x;
  w->h = max_component_y_plus_h - w->y;
}

static char* ReadFile( const char* filename )
{
  char *buffer;
  long length;
  FILE* file;

  file = fopen( filename, "rb" );
  if ( file == NULL )
  {
    printf( "Failed to read %s\n", filename );
    return NULL;
  }

  fseek( file, 0, SEEK_END );
  length = ftell( file );
  fseek( file, 0, SEEK_SET );

  buffer = malloc( length );
  memset( buffer, 0, length );
  fread( buffer, 1, length, file );

  fclose( file );

  return buffer;
}

static void DrawButtonWidget( aWidget_t* w )
{
  SDL_Color c;
  
  if ( w == app.active_widget )
  {
    c.g = 255;
    c.r = c.b = 0;
  }
  
  else
  {
    c.r = w->fg[0];
    c.g = w->fg[1];
    c.b = w->fg[2];
  }
  
  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      a_DrawFilledRect( ( w->x - w->padding ), ( w->y - w->padding ),
                        w->w +( 2 * w->padding ), w->h + ( 2 * w->padding ),
                        w->bg[0], w->bg[1], w->bg[2], w->bg[3] );
    }

    a_DrawText( w->label, w->x, w->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );
  }
}

static void DrawSelectWidget( aWidget_t* w )
{
  SDL_Color c;
  char text[128];
  aSelectWidget_t* s;
  s = ( aSelectWidget_t* ) w->data;

  if ( w == app.active_widget )
  {
    c.g = 255;
    c.r = c.b = 0;
  }
  
  else
  {
    c.r = w->fg[0];
    c.g = w->fg[1];
    c.b = w->fg[2];
  }

  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      a_DrawFilledRect( ( w->x - w->padding ), ( w->y - w->padding ),
                        w->w +( 2 * w->padding ), w->h + ( 2 * w->padding ),
                        w->bg[0], w->bg[1], w->bg[2], w->bg[3] );
    }

    a_DrawText( w->label, w->x, w->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );
    sprintf( text, "< %s >", s->options[s->value] );

    a_DrawText( text, s->x + 100, s->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );
  }
}

static void DrawSliderWidget( aWidget_t* w )
{
  SDL_Color c;
  aSliderWidget_t* slider;
  double width;

  slider = ( aSliderWidget_t* )w->data;

  if ( w == app.active_widget )
  {
    c.g = 255;
    c.r = c.b = 0;
  }
  else
  {
    c.r = w->fg[0];
    c.g = w->fg[1];
    c.b = w->fg[2];
  }
  
  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      a_DrawFilledRect( ( w->x - w->padding ), ( w->y - w->padding ),
                        w->w +( 2 * w->padding ), w->h + ( 2 * w->padding ),
                        w->bg[0], w->bg[1], w->bg[2], w->bg[3] );
    }

    width = ( 1.0 * slider->value ) / 100;

    a_DrawText( w->label, w->x, w->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );

    a_DrawRect( slider->x, slider->y, slider->w, slider->h, 255, 255, 255, 255 );
    a_DrawFilledRect( slider->x + 2, slider->y + 2, ( slider->w - 4 ) * width, slider->h - 4, c.r, c.g, c.b, 255 );
  }

}

static void DrawInputWidget( aWidget_t* w )
{
  SDL_Color c;
  aInputWidget_t* input;
  int width, height;

  input = ( aInputWidget_t* )w->data;

  if ( w == app.active_widget )
  {
    c.g = 255;
    c.r = c.b = 0;
  }

  else
  {
    c.r = w->fg[0];
    c.g = w->fg[1];
    c.b = w->fg[2];
  }
  
  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      a_DrawFilledRect( ( w->x - w->padding ), ( w->y - w->padding ),
                        w->w +( 2 * w->padding ), w->h + ( 2 * w->padding ),
                        w->bg[0], w->bg[1], w->bg[2], w->bg[3] );
    }

    a_DrawText( w->label, w->x, w->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );

    a_DrawText( input->text, input->x, input->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );

    if ( handle_input_widget && app.active_widget == w && ( (int)cursor_blink % (int)FPS < ( FPS / 2 ) ) )
    {
      a_CalcTextDimensions( input->text, app.font_type, &width, &height );
      a_DrawFilledRect( input->x + width + 4, input->y + 14, 32, 32, 0, 255, 0, 255 );
    }
  }
}

static void DrawControlWidget( aWidget_t* w )
{
  SDL_Color c;
  aControlWidget_t* control;
  char text[32];

  control = ( aControlWidget_t* )w->data;

  if ( w == app.active_widget )
  {
    c.g = 255;
    c.r = c.b = 0;
  }

  else
  {
    c.r = w->fg[0];
    c.g = w->fg[1];
    c.b = w->fg[2];
  }

  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      a_DrawFilledRect( ( w->x - w->padding ), ( w->y - w->padding ),
                        w->w +( 2 * w->padding ), w->h + ( 2 * w->padding ),
                        w->bg[0], w->bg[1], w->bg[2], w->bg[3] );
    }

    a_DrawText( w->label, w->x, w->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );

    if ( handle_control_widget && app.active_widget == w )
    {
      a_DrawText( "...", control->x, control->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );
    }

    else
    {
      sprintf( text, "%s", SDL_GetScancodeName( control->value ) );
      a_DrawText( text, control->x, control->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );
    }
  }
}

static void DrawContainerWidget( aWidget_t* w )
{
  aContainerWidget_t* container;

  container = ( aContainerWidget_t* )w->data;
  
  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      a_DrawFilledRect( ( w->x - w->padding ), ( w->y - w->padding ) ,
                        w->w + ( 2 * w->padding ), w->h + ( 2 * w->padding ),
                        w->bg[0], w->bg[1], w->bg[2], w->bg[3] );
    }

    //a_DrawText( w->label, w->x, w->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );

    for ( int i = 0; i < container->num_components; i++ )
    {
      aWidget_t current;
      current = container->components[i];
      
      if ( current.hidden != 1 )
      {
        if ( current.boxed == 1 )
        {
          a_DrawFilledRect( ( current.x - current.padding ), ( current.y - current.padding ),
                           current.w +( 2 * current.padding ), current.h + ( 2 * current.padding ),
                           current.bg[0], current.bg[1], current.bg[2], current.bg[3] );
        }

        switch ( current.type ) {
          case WT_BUTTON:
            DrawButtonWidget( &current );
            break;

          case WT_SLIDER:
            DrawSliderWidget( &current );
            break;

          case WT_INPUT:
            DrawInputWidget( &current );
            break;

          case WT_SELECT:
            DrawSelectWidget( &current );
            break;

          case WT_CONTROL:
            DrawControlWidget( &current );
            break;

          default:
            break;
        } 
      }
    }
  }
}

