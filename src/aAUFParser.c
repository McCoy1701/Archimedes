/*
 * aAUFParser.c:
 *
 * Copyright (c) 2025 Jacob Kellum <jkellum819@gmail.com>
 ************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>

#include "Archimedes.h"

static aAUF_t* NewAUF( void );
static void AddNextAUF( aAUF_t* root, aAUF_t* next );
static void AddPrevAUF( aAUF_t* root, aAUF_t* prev );
static void AddChildAUF( aAUF_t* root, aAUF_t* child );

static char* ReadFile( const char* filename, int* file_size );

aAUF_t* a_AUFParser( const char* filename )
{
  char* file_string;
  int file_size = 0;
  char** line;
  
  int line_start  = 0;
  int line_end    = 0;
  int line_length = 0;
  
  file_string = ReadFile( filename, &file_size );

  //printf( "%d\n%s\n", file_size, file_string );
  
  int newline_count = 0;

  for ( int i = 0; i < file_size; i++ )
  {
    if ( file_string[i] == '\n' )
    {
      line_end = i;
      line_length = line_end - line_start;
      line_start = line_end + 1; 
      
      if ( line_length == 0 )
      {
        continue;
      }
      
      //printf( "i%d\n", i );
      newline_count++;
    }
  }

  line = ( char** )malloc( sizeof( char* ) * newline_count );

  if ( line == NULL )
  {
    printf("Failed to allocate memory for line\n");
    return NULL;
  }
  
  printf( "Line allocated: %d\n", newline_count );

  line_start  = 0;
  line_end    = 0;
  line_length = 0;
  int line_count  = 0;

  for ( int i = 0; i < file_size; i++ )
  {
    if ( file_string[i] == '\n' )
    {
      line_end = i;
      line_length = line_end - line_start;
      
      printf( "LS:%d, LE:%d, LL:%d, LC:%d \n", line_start, line_end, line_length, line_count );
      line_start = line_end + 1; 
      
      if ( line_length == 0 )
      {
        continue;
      }

      line[line_count] = ( char* )malloc( sizeof(char) * ( line_length + 1 ) );
      if ( line[line_count] == NULL )
      {
        printf( "Failed to allocate memory for line at %d\n", line_count );
        for ( int j = 0; j < line_count; j++)
        {
          free( line[j] );
        }

        free( line );
        return NULL;
      }
      
      printf( "Line[%d] allocated: %d\n", line_count, line_length+1 );
      
      for( int j = 0; j <= line_length; j++ )
      {
        line[line_count][j] = file_string[j+line_start];
      }

      line[line_count][line_length+1] = '\0';

      line_count++;
    }
  }

  for ( int i = 0; i < newline_count; i++ )
  {
    printf( "Line: %s\n", line[i] );
  }

  //printf( "nl%d\n", newline_count );

  //aAUF_t* new_list = NewAUF();

  //return new_list;
  return NULL;
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

static void AddNextAUF( aAUF_t* root, aAUF_t* next )
{

}

static void AddPrevAUF( aAUF_t* root, aAUF_t* prev )
{

}

static void AddChildAUF( aAUF_t* root, aAUF_t* child )
{

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

