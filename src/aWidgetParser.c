/*
 * aWdigetParser.c:
 *
 * Copyright (c) 2025 Jacob Kellum <jkellum819@gmail.com>
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

aWidgetList_t* a_WidgetParser( const char* filename )
{
  aWidgetList_t* new_list = ( aWidgetList_t* )malloc( sizeof( aWidgetList_t ) );
  if ( new_list == NULL )
  {
    printf("Failed to allocate memory for aWidgetList_t\n");
    return NULL;
  }


  return new_list;
}

int a_SaveWidgetsToFile( aWidget_t* widget_head, const char* filename )
{
  aWidgetFileHeader_t header = { .magic_number = MAGIC_NUMBER,
                                 .filename = "",
                                 .version = 0,
                                 .num_widgets = 0 };

  STRNCPY( header.filename, filename, MAX_FILENAME_LENGTH );
  

  return 0;
}

