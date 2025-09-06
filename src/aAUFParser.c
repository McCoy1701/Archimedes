/*
 * aAUFParser.c:
 *
 * Copyright (c) 2025 Jacob Kellum <jkellum819@gmail.com>
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Archimedes.h"

static aAUF_t* NewAUF( void );
static int AddAUFToRoot( aAUF_t* root, aAUF_t* node );
static int AddChildAUF( aAUF_t* root, aAUF_t* child );

static char* ReadFile( const char* filename, int* file_size );

static int CountNewLines( const char* file_string, const int file_size );
static char** ParseLinesInFile( const char* file_string, const int file_size,
                                const int nl_count );

static int ParserLineToRoot( aAUF_t* root, char** line, int nl_count );

aAUF_t* a_AUFParser( const char* filename )
{
  char** line;
  char* file_string;
  int file_size = 0;
  int newline_count = 0;
  
  file_string = ReadFile( filename, &file_size );

  newline_count = CountNewLines( file_string, file_size );

  line = ParseLinesInFile( file_string, file_size, newline_count );

  /*for ( int i = 0; i < newline_count; i++ )
  {
    if ( line[i] != NULL )
    {
      printf( "Line: %s\n", line[i] );

    }
  }*/

  aAUF_t* new_root = NULL;

  ParserLineToRoot( new_root, line, newline_count );

  return new_root;
}

int a_SaveAUF( aWidget_t* widget_head, const char* filename )
{
  aWidgetFileHeader_t header = { .magic_number = MAGIC_NUMBER,
                                 .filename     = "",
                                 .version      = 0,
                                 .num_widgets  = 0 };

  STRNCPY( header.filename, filename, MAX_FILENAME_LENGTH );
  

  return 0;
}

int a_FreeAUF( char** line, const int nl_count )
{
  for ( int j = 0; j < nl_count; j++)
  {
    free( line[j] );
  }

  free( line );

  return 0;
}

static aAUF_t* NewAUF( void )
{
  aAUF_t* new_list = ( aAUF_t* )malloc( sizeof( aAUF_t ) );
  if ( new_list == NULL )
  {
    printf("Failed to allocate memory for aWidgetList_t\n");
    return NULL;
  }

  new_list->next  = NULL;
  new_list->prev  = NULL;
  new_list->child = NULL;

  return new_list;
}

static int ParserLineToRoot( aAUF_t* root, char** line, int nl_count )
{
  for ( int i = 0; i < nl_count; i++ )
  {
    if ( line[i] != NULL )
    {
      char* string = line[i];
      int str_len  = strlen( string );

    }
  }

  return 0;
}

static int AddAUFToRoot( aAUF_t* root, aAUF_t* node )
{
  if ( root == NULL || node == NULL )
  {
    printf("Root or Next is NULL: %s:%d\n", __FILE__, __LINE__ );
    return 1;
  }

  aAUF_t* head = root;

  aAUF_t* new_AUF = node;
  if ( new_AUF == NULL )
  {
    printf("Failed to create new_AUF: %s:%d\n", __FILE__, __LINE__ );
    return 1;
  }

  if ( head == NULL )
  {
    head = new_AUF;
    new_AUF->prev = head;
  }

  else
  {
    aAUF_t* current = head;
    while ( current->next != NULL )
    {
      current = current->next;
    }

    current->next = new_AUF;
    new_AUF->prev = current;
  }

  return 0;
}

static int AddChildAUF( aAUF_t* root, aAUF_t* child )
{
  if ( root == NULL || child == NULL )
  {
    printf("Root or Child is NULL: %s:%d\n", __FILE__, __LINE__ );
    return 1;
  }
  
  root->child = child;

  return 0;
}

static char* ReadFile( const char* filename, int* file_size )
{
  long fileSize;
  char *fileString;
  FILE *file;

  file = fopen( filename, "r" );
  if ( file == NULL )
  {
    printf( "Error loading file: %s\n", filename );
    return NULL;
  }

  fseek( file, 0, SEEK_END );
  fileSize = ftell( file );
  rewind( file );

  fileString= ( char* )malloc( fileSize + 1 );
  if ( fileString == NULL )
  {
    printf( "Error allocating memory for file string: %s\n", filename );
    fclose( file );
    return NULL;
  }

  if ( fread( fileString, fileSize, 1, file ) != 1 )
  {
    printf("Failed to read vertex file: %s\n", filename);
    fclose(file);
    free(fileString);
    return NULL;
  }

  fclose(file);

  fileString[fileSize] = '\0';
  
  *file_size = fileSize;

  return fileString;
}

static int CountNewLines( const char* file_string, const int file_size )
{
  int nl_count = 0;

  for ( int i = 0; i < file_size; i++ )
  {
    if ( file_string[i] == '\n' )
    {
      nl_count++;
    }
  }

  return nl_count;
}

static char** ParseLinesInFile( const char* file_string, const int file_size,
                                const int nl_count )
{
  char** new_line = ( char** )malloc( sizeof( char* ) * nl_count );

  if ( new_line == NULL )
  {
    printf("Failed to allocate memory for line\n");
    return NULL;
  }
  
  int line_start  = 0;
  int line_end    = 0;
  int line_length = 0;
  int line_count  = 0;

  for ( int i = 0; i < file_size; i++ )
  {
    if ( file_string[i] == '\n' )
    {
      line_end = i;
      line_length = line_end - line_start;
      
      if ( line_length == 0 )
      {
        line_start = line_end + 1;

        new_line[line_count] = NULL;

        line_count++;
        continue;
      }
      
      new_line[line_count] = ( char* )malloc( sizeof(char) * ( line_length + 1 ) );
      if ( new_line[line_count] == NULL )
      {
        for ( int j = 0; j < line_count; j++)
        {
          free( new_line[j] );
        }

        free( new_line );
        return NULL;
      }

      for( int j = 0; j < line_length; j++ )
      {
        new_line[line_count][j] = file_string[j+line_start];
      }

      new_line[line_count][line_length] = '\0';

      line_start = line_end + 1;
      
      line_count++;

    }
  }

  return new_line;
}

