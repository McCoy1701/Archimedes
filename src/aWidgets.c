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

