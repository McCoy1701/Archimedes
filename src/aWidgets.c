/* 
 * src/aViewport.c
 *
 * This file defines the functions used to create, draw,
 * and handle widget inputs.
 *
 * Copyright (c) 2025 Jacob Kellum <jkellum819@gmail.com>
 *                    Mathew Storm <smattymat@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include "Archimedes.h"

// Function prototypes with static linkage (internal to this compilation unit)
static void LoadWidgets( const char* filename );
static void ChangeWidgetValue( const int value );

static void CreateWidget( aAUF_Node_t* root );
static void CreateButtonWidget( aWidget_t* w, aAUF_Node_t* root );
static void CreateSelectWidget( aWidget_t* w, aAUF_Node_t* root );
static void CreateSliderWidget( aWidget_t* w, aAUF_Node_t* root );
static void CreateInputWidget( aWidget_t* w, aAUF_Node_t* root );
static void CreateControlWidget( aWidget_t* w, aAUF_Node_t* root );
static void CreateContainerWidget( aWidget_t* w, aAUF_Node_t* root );

static void DrawButtonWidget( aWidget_t* w );
static void DrawSelectWidget( aWidget_t* w );
static void DrawSliderWidget( aWidget_t* w );
static void DrawInputWidget( aWidget_t* w );
static void DrawControlWidget( aWidget_t* w );
static void DrawContainerWidget( aWidget_t* w );

static void DoInputWidget( void );
static void DoControlWidget( void );

static aWidget_t widget_head;
static aWidget_t* widget_tail;

static double slider_delay;
static double cursor_blink;
static int handle_input_widget;
static int handle_control_widget;

void a_DoWidget( void )
{
  // aWidget_t* temp;
  //int r,g,b = 0;

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
          if ( app.mouse.x >= current->rect.x &&
               app.mouse.y >= current->rect.y &&
               app.mouse.x <= ( current->rect.x + current->rect.w ) && 
               app.mouse.y <= ( current->rect.y + current->rect.h ) )
          {
            app.active_widget = current;

            if ( current->type == WT_CONTAINER )
            {
              aContainerWidget_t* container =
                ( aContainerWidget_t* )current->data;

              for ( int i = 0; i < container->num_components; i++ )
              {
                aWidget_t* component = &container->components[i];

                if ( component->hidden == 0 )
                {
                  if ( app.mouse.x >= component->rect.x &&
                       app.mouse.y >= component->rect.y &&
                       app.mouse.x <= ( component->rect.x + component->rect.w ) &&
                       app.mouse.y <= ( component->rect.y + component->rect.h ) )
                  {
                    if ( component->action != NULL )
                    {
                      component->action();
                    }
                    app.active_widget = component;
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

    if ( app.keyboard[SDL_SCANCODE_SPACE] ||
         app.keyboard[SDL_SCANCODE_RETURN] )
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

aWidget_t* a_GetWidget( const char* name )
{
  aWidget_t* w;

  for ( w = widget_head.next; w != NULL; w = w->next )
  {
    if ( strcmp( w->name, name ) == 0 )
    {
      return w;
    }
  }

  SDL_LogMessage( SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN,
                  "No such widget: '%s'", name);

  return NULL;
}

aContainerWidget_t* a_GetContainerFromWidget( const char* name )
{
  aWidget_t* widget = NULL;
  aContainerWidget_t* container = NULL;

  widget = a_GetWidget( name );
  if ( widget == NULL )
  {
    printf("Failed to get widget: %s\n", name);
    return NULL;
  }

  container = ( aContainerWidget_t* )widget->data;
  if ( container == NULL )
  {
    printf( "Failed to find container in %s\n", name );
    return NULL;
  }

  return container;
}

static void LoadWidgets( const char* filename )
{
  aAUF_t* root;
  aAUF_Node_t* node;

  root = a_AUFParser( filename );

  for ( node = root->head; node != NULL; node = node->next )
  {
    CreateWidget( node );
  }

  a_AUFFree( root );
}

/**
 * @brief Handles input logic for an active input widget.
 *
 * This function is called when `handle_input_widget` is true. It appends
 * characters from `app.input_text` to the active input widget's text buffer,
 * respecting its `max_length`. It also handles backspace to delete characters
 * and Escape/Return keys to exit input mode and potentially trigger the
 * widget's action.
 */
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

/**
 * @brief Handles input logic for an active control widget.
 *
 * This function is called when `handle_control_widget` is true. It captures
 * the last key pressed (excluding Escape), assigns its scancode value to the
 * active control widget, and then potentially triggers the widget's action.
 * After processing, it exits control mode.
 */
static void DoControlWidget( void )
{
  if ( app.last_key_pressed != -1 )
  {
    if ( app.last_key_pressed != SDL_SCANCODE_ESCAPE )
    {
      ( ( aControlWidget_t* )app.active_widget->data )->value =
        app.last_key_pressed;

      if ( app.active_widget->action != NULL )
      {
        app.active_widget->action();
      }
    }
    handle_control_widget = 0;

    app.keyboard[app.last_key_pressed] = 0;
  }
}

/**
 * @brief Changes the value of the active widget based on its type.
 *
 * This function is typically called in response to left/right arrow key presses.
 * It updates the `value` for `WT_SELECT` widgets (cycling through options) and
 * `WT_SLIDER` widgets (adjusting the slider value within bounds). It also
 * manages `slider_delay` for sliders and triggers the widget's action.
 *
 * @param value The amount by which to change the widget's value (e.g., -1 for left, 1 for right).
 */
static void ChangeWidgetValue( const int value )
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
          app.keyboard[SDL_SCANCODE_LEFT] = 
            app.keyboard[SDL_SCANCODE_RIGHT] = 0;
        }

        slider->value = MIN(
          MAX( slider->value + ( slider->step * value ), 0 ), 100 );

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

/**
 * @brief Creates a new widget and adds it to the global widget list.
 *
 * This function parses a AUF object (`aAUF_Node_t* root`) representing a widget's
 * configuration. It allocates memory for a new `aWidget_t`, populates its
 * common properties (name, label, type, position, colors, etc.), and links
 * it into a global linked list of widgets. It then calls a specialized
 * creation function based on the widget's `type` to handle type-specific data.
 *
 * @param root A aAUF_Node_t object containing the configuration for the widget to be created.
 */
static void CreateWidget( aAUF_Node_t* root )
{
  aWidget_t* w;
  aAUF_Node_t* object, *node;
  int type, i;
  uint8_t fg[4] = {0};
  uint8_t bg[4] = {0};

  type = root->type;

  if ( type != 0 )
  {
    w = malloc( sizeof( aWidget_t ) );
    memset( w, 0, sizeof( aWidget_t ) );

    widget_tail->next = w;
    w->prev = widget_tail;
    widget_tail = w;

    STRCPY( w->name, root->value_string );
    STRCPY( w->label, a_AUFGetObjectItem( root, "label" )->value_string );
    w->type = root->type;

    w->rect.x = a_AUFGetObjectItem( root, "x" )->value_int;
    w->rect.y = a_AUFGetObjectItem( root, "y" )->value_int;
    w->boxed = a_AUFGetObjectItem( root, "boxed" )->value_int;
    w->hidden = a_AUFGetObjectItem( root, "hidden" )->value_int;
    w->padding = a_AUFGetObjectItem( root, "padding" )->value_int;
    w->action = NULL;

    object = a_AUFGetObjectItem( root, "fg");
    i = 0;
    for ( node = object->child; node != NULL; node = node->next )
    {
      fg[i++] = node->value_int;
    }
    w->fg.r = fg[0];
    w->fg.g = fg[1];
    w->fg.b = fg[2];
    w->fg.a = fg[3];

    object = a_AUFGetObjectItem( root, "bg");
    i = 0;
    for ( node = object->child; node != NULL; node = node->next )
    {
      bg[i++] = node->value_int;
    }
    w->bg.r = bg[0];
    w->bg.g = bg[1];
    w->bg.b = bg[2];
    w->bg.a = bg[3];

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

/**
 * @brief Creates type-specific data for a Button widget.
 *
 * This function calculates and sets the width (`w`) and height (`h`) of the
 * given button widget based on the dimensions of its label text. It primarily
 * uses `a_CalcTextDimensions` for this purpose.
 *
 * @param w A pointer to the `aWidget_t` structure for the button.
 * @param root A aAUF_Node_t object containing the configuration for the button.
 */
static void CreateButtonWidget( aWidget_t* w, aAUF_Node_t* root )
{
  a_CalcTextDimensions( w->label, app.font_type, &w->rect.w, &w->rect.h );
}

/**
 * @brief Creates type-specific data for a Slider widget.
 *
 * This function allocates and initializes an `aSliderWidget_t` structure,
 * linking it to the `data` member of the base widget. It retrieves the
 * slider's `step` and `wait_on_change` properties from the aAUF_Node_t root and
 * calculates the slider's position and dimensions relative to its label.
 *
 * @param w A pointer to the `aWidget_t` structure for the slider widget.
 */
static void CreateSelectWidget( aWidget_t* w, aAUF_Node_t* root )
{
  aAUF_Node_t* options, *node;
  int i, len, temp_w, temp_h;
  float width, height;
  char* temp_string;
  aSelectWidget_t* s;

  s = malloc( sizeof( aSelectWidget_t ) );
  memset( s, 0, sizeof( aSelectWidget_t ) );
  w->data = s;

  options = a_AUFGetObjectItem( root, "options" );

  s->num_options = options->value_int;
  s->value = 0;
  
  temp_w = temp_h = width = height = 0;

  if ( s->num_options > 0 )
  {
    i = 0;

    s->options = malloc( sizeof(char*) * s->num_options );

    for( node = options->child; node != NULL; node = node->next )
    {
      len = strlen( node->value_string ) + 1;

      s->options[i] = malloc( len );
      temp_string = malloc( len + 4 );

      snprintf(temp_string, ( len + 4 ), "< %s >", node->value_string );

      STRNCPY( s->options[i], node->value_string, len );
      a_CalcTextDimensions( temp_string, app.font_type, &width, &height );
      
      if ( width > temp_w )
      {
        temp_w = width; //Get the largest width
      }
      
      if ( height > temp_h )
      {
        temp_h = height;
      }
      
      free( temp_string );
      i++;
    }
  }

  a_CalcTextDimensions( w->label, app.font_type, &w->rect.w, &w->rect.h );

  s->rect.x = w->rect.x + 100;
  s->rect.y = w->rect.y;
  s->rect.w = temp_w + 100;
  s->rect.h = temp_h;
}


/**
 * @brief Creates type-specific data for a Slider widget.
 *
 * This function allocates and initializes an `aSliderWidget_t` structure,
 * linking it to the `data` member of the base widget. It retrieves the
 * slider's `step` and `wait_on_change` properties from the aAUF_Node_t root and
 * calculates the slider's position and dimensions relative to its label.
 *
 * @param w A pointer to the `aWidget_t` structure for the slider widget.
 * @param root A aAUF_Node_t object containing the configuration for the slider widget.
 */
static void CreateSliderWidget( aWidget_t* w, aAUF_Node_t* root )
{
  aSliderWidget_t* s;
  s = malloc( sizeof( aSliderWidget_t ) );
  memset( s, 0, sizeof( aSliderWidget_t ) );
  w->data = s;

  s->step = a_AUFGetObjectItem( root, "step" )->value_int;
  s->wait_on_change = a_AUFGetObjectItem( root, "wait_on_change" )->value_int;
  s->value = 0;

  a_CalcTextDimensions( w->label, app.font_type, &w->rect.w, &w->rect.h );
  s->rect.x = w->rect.x + w->rect.w + 50;
	s->rect.y = w->rect.y;
	s->rect.w = w->rect.w;
	s->rect.h = w->rect.h;
}

/**
 * @brief Creates type-specific data for an Input widget.
 *
 * This function allocates and initializes an `aInputWidget_t` structure,
 * linking it to the `data` member of the base widget. It retrieves the
 * `max_length` for the input text from the aAUF_Node_t root and allocates a buffer
 * for the text. It also initializes a default text and calculates the
 * input field's dimensions.
 *
 * @param w A pointer to the `aWidget_t` structure for the input widget.
 * @param root A aAUF_Node_t object containing the configuration for the input widget.
 */
static void CreateInputWidget( aWidget_t* w, aAUF_Node_t* root )
{
  aInputWidget_t* input;

  input = malloc( sizeof( aInputWidget_t ) );
  memset( input, 0, sizeof( aInputWidget_t ) );

  w->data = input;

  input->max_length = a_AUFGetObjectItem( root, "max_length" )->value_int;
  input->text = malloc(  input->max_length + 1 );

  STRNCPY( input->text, "...", MAX_INPUT_LENGTH );

  a_CalcTextDimensions( w->label, app.font_type, &w->rect.w, &w->rect.h );
  input->rect.x = w->rect.x + w->rect.w + 50;
  input->rect.y = w->rect.y;
  a_CalcTextDimensions( input->text, app.font_type,
                        &input->rect.w, &input->rect.h );
}

/**
 * @brief Creates type-specific data for a Control (key binding) widget.
 *
 * This function allocates and initializes an `aControlWidget_t` structure,
 * linking it to the `data` member of the base widget. It primarily calculates
 * the dimensions of the base widget based on its label text.
 *
 * @param w A pointer to the `aWidget_t` structure for the control widget.
 * @param root A aAUF_Node_t object containing the configuration for the control widget.
 */
static void CreateControlWidget( aWidget_t* w, aAUF_Node_t* root )
{
  aControlWidget_t* control;

  control = malloc( sizeof( aControlWidget_t ) );
  memset( control, 0, sizeof( aControlWidget_t ) );

  w->data = control;
  a_CalcTextDimensions( w->label, app.font_type, &w->rect.w, &w->rect.h );
}

/**
 * @brief Creates type-specific data for a Container widget.
 *
 * This function allocates and initializes an `aContainerWidget_t` structure,
 * linking it to the `data` member of the base widget. It parses the "components"
 * array from the aAUF_Node_t root, recursively creating and positioning child widgets
 * within the container. It supports different flexing modes (horizontal/vertical)
 * for component arrangement and calculates the overall dimensions of the container
 * based on its components.
 *
 * @param w A pointer to the `aWidget_t` structure for the container widget.
 * @param root A aAUF_Node_t object containing the configuration for the container widget.
 */
static void CreateContainerWidget( aWidget_t* w, aAUF_Node_t* root )
{
  aAUF_Node_t* object, *node;
  int i;
  int temp_x, temp_y;
  aContainerWidget_t* container;
  aInputWidget_t* input;
  aSliderWidget_t* slider;
  aSelectWidget_t* select;
  uint8_t fg[4] = {0};
  uint8_t bg[4] = {0};
  
  container = ( aContainerWidget_t* )malloc( sizeof( aContainerWidget_t ) );
  if ( container == NULL )
  {
    printf("Failed to allocate memory for container\n");
    exit(1);
  }

  memset( container, 0, sizeof( aContainerWidget_t ) );
  
  w->data = container;
  w->flex = a_AUFGetObjectItem( root, "flex" )->value_int;
  container->spacing = a_AUFGetObjectItem( root, "spacing" )->value_int;
  w->rect.w = w->rect.h = 0;
  w->action = NULL;

  object = a_AUFGetObjectItem( root, "container" );
  container->num_components = object->value_int;

  container->components = ( aWidget_t* )malloc( sizeof( aWidget_t ) *
                                               container->num_components );

  if ( container->components == NULL )
  {
    printf("Failed to allocate memory for components\n");
    exit(1);
  }

  i = 0;
  temp_x = w->rect.x;
  temp_y = w->rect.y;

  int max_component_x_plus_w = 0;
  int max_component_y_plus_h = 0;

  for ( node = object->child; node != NULL; node = node->next )
  {
    aWidget_t* current = &container->components[i];
    STRCPY( current->name, node->value_string );
    STRCPY( current->label,
           a_AUFGetObjectItem( node, "label" )->value_string );

    current->type = node->type;

    current->boxed = a_AUFGetObjectItem( node, "boxed" )->value_int;
    current->hidden = a_AUFGetObjectItem( node, "hidden" )->value_int;
    current->padding = a_AUFGetObjectItem( node, "padding" )->value_int;
    current->action = NULL;

    aAUF_Node_t* object_1, *node_1;
    int j;
    object_1 = a_AUFGetObjectItem( node, "fg");
    j = 0;
    for ( node_1 = object_1->child; node_1 != NULL; node_1 = node_1->next )
    {
      fg[j++] = node_1->value_int;
    }
    current->fg.r = fg[0];
    current->fg.g = fg[1];
    current->fg.b = fg[2];
    current->fg.a = fg[3];

    object_1 = a_AUFGetObjectItem( node, "bg");
    j = 0;
    for ( node_1 = object_1->child; node_1 != NULL; node_1 = node_1->next )
    {
      bg[j++] = node_1->value_int;
    }
    current->bg.r = bg[0];
    current->bg.g = bg[1];
    current->bg.b = bg[2];
    current->bg.a = bg[3];
    
    a_CalcTextDimensions( current->label, app.font_type,
                          &current->rect.w, &current->rect.h );

    if ( w->flex == 1 || w->flex == 2 )
    {
      current->rect.x = temp_x;
      current->rect.y = temp_y;

      //temp_x += ( current->w + container->spacing );
    }
  
    else
    {
      current->rect.x = a_AUFGetObjectItem( node, "x" )->value_int;
      current->rect.y = a_AUFGetObjectItem( node, "y" )->value_int;
    }

    int widget_effective_w = current->rect.w;
    int widget_effective_h = current->rect.h;

    int current_widget_max_x_extent = current->rect.x + current->rect.w;
    int current_widget_max_y_extent = current->rect.y + current->rect.h;
    
    switch ( current->type )
    {
      case WT_BUTTON:
        CreateButtonWidget( current, node );
        current_widget_max_x_extent = current->rect.x + current->rect.w;
        current_widget_max_y_extent = current->rect.y + current->rect.h;
        break;

      case WT_SELECT:
        CreateSelectWidget( current, node );
        select = (aSelectWidget_t*)current->data;

        current_widget_max_x_extent = MAX( current_widget_max_x_extent,
                                         ( select->rect.x + select->rect.w ) );
        current_widget_max_y_extent = MAX( current_widget_max_y_extent,
                                         ( select->rect.y + select->rect.h ) );
        break;
      
      case WT_SLIDER:
        CreateSliderWidget( current, node );
        slider = (aSliderWidget_t*)current->data;

        current_widget_max_x_extent = MAX( current_widget_max_x_extent,
                                         ( slider->rect.x + slider->rect.w ) );
        current_widget_max_y_extent = MAX( current_widget_max_y_extent,
                                         ( slider->rect.y + slider->rect.h ) );
        break;
      
      case WT_INPUT:
        CreateInputWidget( current, node );
        input = (aInputWidget_t*)current->data;

        current_widget_max_x_extent = MAX( current_widget_max_x_extent,
                                           ( input->rect.x + input->rect.w ) );
        current_widget_max_y_extent = MAX( current_widget_max_y_extent,
                                           ( input->rect.y + input->rect.h ) );
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

    widget_effective_w = current_widget_max_x_extent - current->rect.x;
    widget_effective_h = current_widget_max_y_extent - current->rect.y;

    if ( w->flex == 1 )
    {
      temp_x += ( widget_effective_w + container->spacing );
    }
    
    if ( w->flex == 2 )
    {
      temp_y += ( widget_effective_h + container->spacing );
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

  w->rect.w = max_component_x_plus_w - w->rect.x;
  w->rect.h = max_component_y_plus_h - w->rect.y;
}

static void DrawButtonWidget( aWidget_t* w )
{
  aColor_t c;
  
  if ( strcmp( w->name, app.active_widget->name ) == 0 )
  {
    c.g = 255;
    c.r = c.b = 0;
  }
  
  else
  {
    c.r = w->fg.r;
    c.g = w->fg.g;
    c.b = w->fg.b;
  }
  
  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      aRectf_t rect = (aRectf_t){ .x = ( w->rect.x - w->padding ),
                                  .y = ( w->rect.y - w->padding ),
                                  .w = ( w->rect.w + ( 2 * w->padding ) ),
                                  .h = ( w->rect.h + ( 2 * w->padding ) ) };
      
      a_DrawFilledRect( rect, w->bg );
    }

    a_DrawText( w->label, w->rect.x, w->rect.y, c, black,
                app.font_type, TEXT_ALIGN_LEFT, 0 );
  }
}

static void DrawSelectWidget( aWidget_t* w )
{
  aColor_t c;
  char text[128];
  aSelectWidget_t* s;
  s = ( aSelectWidget_t* ) w->data;

  if ( strcmp( w->name, app.active_widget->name ) == 0 )
  {
    c.g = 255;
    c.r = c.b = 0;
  }
  
  else
  {
    c.r = w->fg.r;
    c.g = w->fg.g;
    c.b = w->fg.b;
  }

  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      aRectf_t rect = (aRectf_t){ .x = ( w->rect.x - w->padding ),
                                  .y = ( w->rect.y - w->padding ),
                                  .w = ( w->rect.w + ( 2 * w->padding ) ),
                                  .h = ( w->rect.h + ( 2 * w->padding ) ) };
      
      a_DrawFilledRect( rect, w->bg );
    }

    a_DrawText( w->label, w->rect.x, w->rect.y, c, black,
                app.font_type, TEXT_ALIGN_LEFT, 0 );
    sprintf( text, "< %s >", s->options[s->value] );

    a_DrawText( text, s->rect.x + 100, s->rect.y, c, black,
                app.font_type, TEXT_ALIGN_LEFT, 0 );
  }
}

static void DrawSliderWidget( aWidget_t* w )
{
  aColor_t c;
  aSliderWidget_t* slider;
  double width;

  slider = ( aSliderWidget_t* )w->data;

  if ( strcmp( w->name, app.active_widget->name ) == 0 )
  {
    c.g = 255;
    c.r = c.b = 0;
  }
  else
  {
    c.r = w->fg.r;
    c.g = w->fg.g;
    c.b = w->fg.b;
  }
  
  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      aRectf_t rect = (aRectf_t){ .x = ( w->rect.x - w->padding ),
                                  .y = ( w->rect.y - w->padding ),
                                  .w = ( w->rect.w + ( 2 * w->padding ) ),
                                  .h = ( w->rect.h + ( 2 * w->padding ) ) };
      
      a_DrawFilledRect( rect, w->bg );
    }

    width = ( 1.0 * slider->value ) / 100;

    a_DrawText( w->label, w->rect.x, w->rect.y, c, black,
                app.font_type, TEXT_ALIGN_LEFT, 0 );

    aRectf_t slider_bg_rect = (aRectf_t){ .x = slider->rect.x,
                                          .y = slider->rect.y,
                                          .w = slider->rect.w,
                                          .h = slider->rect.h };

    a_DrawRect( slider_bg_rect, white );
    aRectf_t slider_rect = (aRectf_t){ .x = ( slider->rect.x + 2 ),
                                       .y = ( slider->rect.y + 2 ),
                                       .w = ( ( slider->rect.w - 4 ) * width ),
                                       .h = ( slider->rect.h - 4 ) };
    a_DrawFilledRect( slider_rect, c );
  }

}

static void DrawInputWidget( aWidget_t* w )
{
  aColor_t c;
  aInputWidget_t* input;
  float width, height;

  input = ( aInputWidget_t* )w->data;

  if ( strcmp( w->name, app.active_widget->name ) == 0 )
  {
    c.g = 255;
    c.r = c.b = 0;
  }

  else
  {
    c.r = w->fg.r;
    c.g = w->fg.g;
    c.b = w->fg.b;
  }
  
  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      aRectf_t rect = (aRectf_t){ .x = ( w->rect.x - w->padding ),
                                  .y = ( w->rect.y - w->padding ),
                                  .w = ( w->rect.w + ( 2 * w->padding ) ),
                                  .h = ( w->rect.h + ( 2 * w->padding ) ) };
      
      a_DrawFilledRect( rect, w->bg );
    }

    a_DrawText( w->label, w->rect.x, w->rect.y, c, black,
                app.font_type, TEXT_ALIGN_LEFT, 0 );

    a_DrawText( input->text, input->rect.x, input->rect.y, c, black,
                app.font_type, TEXT_ALIGN_LEFT, 0 );

    if ( handle_input_widget && app.active_widget == w &&
         ( (int)cursor_blink % (int)FPS_CAP < ( FPS_CAP / 2 ) ) )
    {
      a_CalcTextDimensions( input->text, app.font_type, &width, &height );
      aRectf_t cursor_rect = ( aRectf_t ){ .x = ( input->rect.x + width + 4 ),
                                         .y = ( input->rect.y + 14 ),
                                         .w = 32,
                                         .h = 32 };
      a_DrawFilledRect( cursor_rect, green );
    }
  }
}

static void DrawControlWidget( aWidget_t* w )
{
  aColor_t c;
  aControlWidget_t* control;
  char text[32];

  control = ( aControlWidget_t* )w->data;

  if ( strcmp( w->name, app.active_widget->name ) == 0 )
  {
    c.g = 255;
    c.r = c.b = 0;
  }

  else
  {
    c.r = w->fg.r;
    c.g = w->fg.g;
    c.b = w->fg.b;
  }

  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      aRectf_t rect = (aRectf_t){ .x = ( w->rect.x - w->padding ),
                                  .y = ( w->rect.y - w->padding ),
                                  .w = ( w->rect.w + ( 2 * w->padding ) ),
                                  .h = ( w->rect.h + ( 2 * w->padding ) ) };
      
      a_DrawFilledRect( rect, w->bg );
    }

    a_DrawText( w->label, w->rect.x, w->rect.y, c, black,
                app.font_type, TEXT_ALIGN_LEFT, 0 );

    if ( handle_control_widget && app.active_widget == w )
    {
      a_DrawText( "...", control->x, control->y, c, black,
                  app.font_type, TEXT_ALIGN_LEFT, 0 );
    }

    else
    {
      sprintf( text, "%s", SDL_GetScancodeName( control->value ) );
      a_DrawText( text, control->x, control->y, c, black,
                  app.font_type, TEXT_ALIGN_LEFT, 0 );
    }
  }
}

/**
 * @brief Draws a Container widget and all its visible components on the screen.
 *
 * This function handles the rendering of a container widget. It first draws
 * a background box for the container itself if it's `boxed`. Then, it iterates
 * through all the components (child widgets) within the container and recursively
 * calls their respective drawing functions, ensuring that only visible components
 * are drawn.
 *
 * @param w A pointer to the `aWidget_t` structure representing the container widget to draw.
 */
static void DrawContainerWidget( aWidget_t* w )
{
  aContainerWidget_t* container;

  container = ( aContainerWidget_t* )w->data;
  
  if ( w->hidden != 1 )
  {
    if ( w->boxed == 1 )
    {
      aRectf_t rect = (aRectf_t){ .x = ( w->rect.x - w->padding ),
                                  .y = ( w->rect.y - w->padding ),
                                  .w = ( w->rect.w + ( 2 * w->padding ) ),
                                  .h = ( w->rect.h + ( 2 * w->padding ) ) };
      
      a_DrawFilledRect( rect, w->bg );
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
          aRectf_t current_rect = (aRectf_t){ 
            .x = ( current.rect.x - current.padding ),
            .y = ( current.rect.y - current.padding ),
            .w = ( current.rect.w + ( 2 * current.padding ) ),
            .h = ( current.rect.h + ( 2 * current.padding ) )
          };

          a_DrawFilledRect( current_rect, current.bg );
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

int a_FreeWidgetCache( void )
{
  if ( widget_head.next == NULL )
  {
    printf( "No Widgets loaded in cache\n" );
    return 1;
  }

  else
  {
    aWidget_t* current = &widget_head;
    aWidget_t* next = NULL;

    while ( current != NULL )
    {
      next = current->next;
      if ( current->action != NULL )
      {
        current->action = NULL;
      }
      
      if ( current->data != NULL )
      {
        free( current->data );
        current->data = NULL;
      }

      free( current );
      current = next;
    }
    
    memset( &widget_head, 0, sizeof(aWidget_t) );
    widget_tail = &widget_head;
  }

  return 0;
}

