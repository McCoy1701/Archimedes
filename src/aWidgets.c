#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
#include "Archimedes.h"

static aWidget_t widget_head;
static aWidget_t* widget_tail;

static void LoadWidgets( char* filename );
static int GetWidgetType( char* type );
static void ChangeWidgetValue( int value );
static void CreateWidget( cJSON* root );

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

static void do_Input_Widget( void );
static void do_Control_Widget( void );


static void create_Widget( cJSON* root )
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

static void CreateButtonWidget( Widget_t* w, cJSON* root )
{
  calcTextDimensions( w->label, app.font_type, &w->w, &w->h );
}

static void create_Select_Widget( Widget_t* w, cJSON* root )
{
  cJSON* options, *node;
  int i, len;
  Select_Widget_t* s;

  s = malloc( sizeof( Select_Widget_t ) );
  memset( s, 0, sizeof( Select_Widget_t ) );
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

  calc_Text_Dimensions( w->label, app.font_type, &w->w, &w->h );

  s->x = w->x + 50;
  s->y = w->y;
}

static void create_Slider_Widget( Widget_t* w, cJSON* root )
{
  Slider_Widget_t* s;
  s = malloc( sizeof( Slider_Widget_t ) );
  memset( s, 0, sizeof( Slider_Widget_t ) );
  w->data = s;

  s->step = cJSON_GetObjectItem( root, "step" )->valueint;
  s->wait_on_change = cJSON_GetObjectItem( root, "wait_on_change" )->valueint;

  calc_Text_Dimensions( w->label, app.font_type, &w->w, &w->h );
}

static void create_Input_Widget( Widget_t* w, cJSON* root )
{
  Input_Widget_t* input;

  input = malloc( sizeof( Input_Widget_t ) );
  memset( input, 0, sizeof( Input_Widget_t ) );

  w->data = input;

  input->max_length = cJSON_GetObjectItem( root, "max_length" )->valueint;
  input->text = malloc(  input->max_length + 1 );

  calc_Text_Dimensions( w->label, app.font_type, &w->w, &w->h );  
}

static void create_Control_Widget( Widget_t* w, cJSON* root )
{
  Control_Widget_t* control;

  control = malloc( sizeof( Control_Widget_t ) );
  memset( control, 0, sizeof( Control_Widget_t ) );

  w->data = control;
  calc_Text_Dimensions( w->label, app.font_type, &w->w, &w->h );
}
