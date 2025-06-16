#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "Archimedes.h"

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
static void DrawButtonWidget( aWidget_t* w );
static void DrawSelectWidget( aWidget_t* w );
static void DrawSliderWidget( aWidget_t* w );
static void DrawInputWidget( aWidget_t* w );
static void DrawControlWidget( aWidget_t* w );

static void DoInputWidget( void );
static void DoControlWidget( void );

static double slider_delay;
static double cursor_blink;
static int handle_input_widget;
static int handle_control_widget;

void a_DoWidget( void )
{
  slider_delay = MAX( slider_delay - a_GetDeltaTime(), 0 );

  cursor_blink += a_GetDeltaTime();

  if ( !handle_input_widget && !handle_control_widget )
  {
    if ( app.keyboard[SDL_SCANCODE_UP] )
    {
      app.keyboard[SDL_SCANCODE_UP] = 0;

      app.active_widget = app.active_widget->prev;

      if ( app.active_widget == &widget_head )
      {
        app.active_widget = widget_tail;
      }
    }

    if ( app.keyboard[SDL_SCANCODE_DOWN] )
    {
      app.keyboard[SDL_SCANCODE_DOWN] = 0;
      
      app.active_widget = app.active_widget->next;

      if ( app.active_widget == NULL )
      {
        app.active_widget = widget_head.next;
      }
    }

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
  int h;
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

      default:
        break;
    }

    if ( w == app.active_widget )
    {
      h = w->h / 2;

      a_DrawRect( w->x - ( h * 2 ), w->y + ( h / 2 ), h, h, 0, 255, 0, 255 );
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
  int type;

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
  int i, len;
  aSelectWidget_t* s;

  s = malloc( sizeof( aSelectWidget_t ) );
  memset( s, 0, sizeof( aSelectWidget_t ) );
  w->data = s;

  options = cJSON_GetObjectItem( root, "options" );

  s->num_options = cJSON_GetArraySize( options );

  if ( s->num_options > 0 )
  {
    i = 0;

    s->options = malloc( sizeof(char*) * s->num_options );

    for( node = options->child; node != NULL; node = node->next )
    {
      len = strlen( node->valuestring ) + 1;

      s->options[i] = malloc(len);

      STRNCPY( s->options[i], node->valuestring, len );

      i++;
    }
  }

  a_CalcTextDimensions( w->label, app.font_type, &w->w, &w->h );

  s->x = w->x + 50;
  s->y = w->y;
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

  a_CalcTextDimensions( w->label, app.font_type, &w->w, &w->h );
  input->x = w->x + w->w + 50;
}

static void CreateControlWidget( aWidget_t* w, cJSON* root )
{
  aControlWidget_t* control;

  control = malloc( sizeof( aControlWidget_t ) );
  memset( control, 0, sizeof( aControlWidget_t ) );

  w->data = control;
  a_CalcTextDimensions( w->label, app.font_type, &w->w, &w->h );
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
    c.r = c.g = c.b = 255;
  }

  a_DrawText( w->label, w->x, w->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );
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
    c.r = c.g = c.b = 255;
  }

  a_DrawText( w->label, w->x, w->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );
  sprintf( text, "< %s >", s->options[s->value] );

  a_DrawText( text, s->x, s->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );
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
    c.r = c.g = c.b = 255;
  }

  width = ( 1.0 * slider->value ) / 100;

  a_DrawText( w->label, w->x, w->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );

  a_DrawRect( slider->x, slider->y, slider->w, slider->h, 255, 255, 255, 255 );
  a_DrawRect( slider->x + 2, slider->y + 2, ( slider->w - 4 ) * width, slider->h - 4, c.r, c.g, c.b, 255 );
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
    c.r = c.g = c.b = 255;
  }

  a_DrawText( w->label, w->x, w->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );
  
  a_DrawText( input->text, input->x, input->y, c.r, c.g, c.b, app.font_type, TEXT_ALIGN_LEFT, 0 );

  if ( handle_input_widget && app.active_widget == w && ( (int)cursor_blink % (int)FPS < ( FPS / 2 ) ) )
  {
    a_CalcTextDimensions( input->text, app.font_type, &width, &height );
    a_DrawRect( input->x + width + 4, input->y + 14, 32, 32, 0, 255, 0, 255 );
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
    c.r = c.g = c.b = 255;
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

