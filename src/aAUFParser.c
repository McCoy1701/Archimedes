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

static char* ReadFile( const char* filename, int* file_size );

static int CountNewLines( const char* file_string, const int file_size );
static char** ParseLinesInFile( const char* file_string, const int file_size,
                                const int nl_count );

static int ParserLineToRoot( aAUF_t* root, char** line, int nl_count );
static int ParserWidgetToNode( aAUF_Node_t* node, char** line, int nl_count, int idx );
static char* ParseString( char delimiter, char* str, int str_len );
static char* ParseStringDoubleDelim( char delimiter1, char delimiter2, char* str, int str_len );
static int handle_square_bracket( aAUF_Node_t* root, char* string, int str_len, 
                                  char** line, int nl_count, int idx );
static int handle_parenthesis( aAUF_Node_t* root, char* string, int str_len );
static int handle_char( aAUF_Node_t* root, char* string, int str_len );

aAUF_t* a_AUFParser( const char* filename )
{
  char** line;
  char* file_string;
  int file_size = 0;
  int newline_count = 0;
 
  aAUF_t* new_root = a_AUFCreation();

  file_string = ReadFile( filename, &file_size );

  newline_count = CountNewLines( file_string, file_size );

  line = ParseLinesInFile( file_string, file_size, newline_count );

  ParserLineToRoot( new_root, line, newline_count );
  
  free( file_string );
  a_FreeAUF( line, newline_count );

  return new_root;
}

int a_SaveAUF( aWidget_t* widget_head, const char* filename )
{
  aWidgetFileHeader_t header = { .magic_number = MAGIC_NUMBER,
                                 .filename     = "",
                                 .version      = 0,
                                 .num_widgets  = 0 };

  STRNCPY( header.filename, filename, MAX_FILENAME_LENGTH );

  printf( "%s\n", widget_head->name );
  

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

static int ParserLineToRoot( aAUF_t* root, char** line, int nl_count )
{
  for ( int i = 0; i < nl_count; i++ )
  {
    if ( line[i] != NULL )
    {
      char* string = line[i];
      size_t str_len  = strlen( string );

      if ( string[0] == '[' && string[1] != '[' )
      {
        char* str_start = strstr( string, "WT_" );
        char* str_end = strstr( string, "." );
        
        aAUF_Node_t* new_AUF = a_AUFNodeCreation();

        new_AUF->string = ParseString( '.', str_start, str_len );
        new_AUF->value_string = ParseString( ']', str_end+1, str_len );

        if ( a_AUFAddNode( root, new_AUF ) < 0 )
        {
          printf( "Failed to add %s to root\n", new_AUF->string );
        }

        int list_offset = ParserWidgetToNode( new_AUF, line, nl_count, i );
        printf( "offset: %d\n", list_offset );
      }

    }
  }

  return 0;
}

static int ParserWidgetToNode( aAUF_Node_t* node, char** line, int nl_count, int idx )
{
  int count = 0;
  for ( int i = idx; i < nl_count; i++ )
  {
    if ( line[i] != NULL )
    {
      char* string = line[i];
      int str_len  = strlen( string );

      switch ( string[0] )
      {
        case '[':
          if ( string[1] == '[' )
          {
            aAUF_Node_t* container = a_GetObjectItem( node, "container" );
            if ( container ==  NULL )
            {
              container = a_AUFNodeCreation();

              container->string = malloc( sizeof( char ) * MAX_NAME_LENGTH );
              STRNCPY( container->string, "container0", MAX_NAME_LENGTH );
              a_AUFAddChild( node, container );
              handle_square_bracket( container, string, str_len, line, nl_count, i );
            }
       
            else 
            {
              while ( container->next != NULL )
              {
                container = container->next;
              }

              aAUF_Node_t* new_container = a_AUFNodeCreation();
              container->string = malloc( sizeof( char ) * MAX_NAME_LENGTH );
              STRNCPY( container->string, "container1", MAX_NAME_LENGTH );
              container->next = new_container;
              new_container->prev = container;
              new_container->next = NULL;
              handle_square_bracket( container->next, string, str_len, line, nl_count, i );
            }

          }
          continue;
        
        case '(':
          handle_parenthesis( node, string, str_len );
          continue;

        default:
          handle_char( node, string, str_len );
          continue;
      }

    }

    count = i;
    return i;
  }

  return count;
}

static char* ParseString( char delimiter, char* str, int str_len )
{
  char* return_str = (char*)malloc( sizeof(char) * str_len );
  if ( return_str == NULL )
  {
    printf( "Failed to allocate memory for return_str\n" );
    return NULL;
  }

  for ( int i = 0; i < str_len; i++ )
  {
    if ( str[i] != delimiter )
    {
      return_str[i] = str[i];

    }

    else
    {
      return_str[i] = '\0';
      return return_str;
    }
  }

  return NULL;
}

static int handle_parenthesis( aAUF_Node_t* root, char* string, int str_len )
{
  if ( root == NULL || string == NULL || str_len == 0 )
  {
    printf( "Handle parenthesis ran into a problem\
             root/string/str_len is NULL/0: %s, %d\n", __FILE__, __LINE__ );
    return 1;
  }

  aAUF_Node_t* x_AUF = a_AUFNodeCreation();
  aAUF_Node_t* y_AUF = a_AUFNodeCreation();
  x_AUF->string = ParseString( ',', string+1, str_len );
  char* str_y_start  = strchr( string, ',' );
  size_t str_y_len = strlen( str_y_start );
  y_AUF->string = ParseString( ')', str_y_start+1, str_y_len );
  
  char* x_value_start = strchr( string, ':' );
  size_t str_x_value_len = strlen( x_value_start );
  char* x_value = ParseString( ',', x_value_start+2, str_x_value_len );
  char* y_value_start = strchr( x_value_start, ',' );
  size_t str_y_value_len = strlen( y_value_start );
  char* y_value = ParseString( ')', y_value_start+1, str_y_value_len );

  if ( strchr( x_value, '.' ) || strchr( y_value, '.' ) )
  {
    x_AUF->value_double = atof( x_value );
    y_AUF->value_double = atof( y_value );
  }
  
  else
  {
    x_AUF->value_int = atoi( x_value );
    y_AUF->value_int = atoi( y_value );
  }

  if ( a_AUFAddChild( root, x_AUF ) < 0 )
  {
    printf( "Failed to add %s to root\n", x_AUF->string );
    free(x_AUF);
    free(y_AUF);
    return 1;
  }
  
  if ( a_AUFAddChild( root, y_AUF ) < 0 )
  {
    printf( "Failed to add %s to root\n", y_AUF->string );
    free(x_AUF);
    free(y_AUF);
    return 1;
  }
  
  return 0;
}

static int handle_square_bracket( aAUF_Node_t* root, char* string, int str_len,
                                  char** line, int nl_count, int idx )
{
  if ( root == NULL || string == NULL || str_len == 0 )
  {
    printf( "Handle square bracket ran into a problem \
             root/string/str_len is NULL/0: %s, %d \n", __FILE__, __LINE__ );
    return 1;
  }

  char* str_start = strstr( string, "WT_" );
  char* str_end = strstr( string, "." );

  aAUF_Node_t* new_AUF = a_AUFNodeCreation();

  new_AUF->string = ParseString( '.', str_start, str_len );
  new_AUF->value_string = ParseString( ']', str_end+1, str_len );

  if ( a_AUFAddChild( root, new_AUF ) < 0 )
  {
    printf( "Failed to add %s to root\n", new_AUF->string );
  }
  
  int count = 0;
  for ( int i = idx; i < nl_count; i++ )
  {
    if ( line[i] != NULL )
    {
      char* string = line[i];
      int str_len  = strlen( string );

      switch ( string[0] )
      {
        case '[':
          if ( string[1] == '[' )
          {
            continue;
          }
          return 0;
        
        case '(':
          handle_parenthesis( root, string, str_len );
          continue;

        default:
          handle_char( root, string, str_len );
          continue;
      }

    }

    count = i;
  }

  return count;
}

static int handle_char( aAUF_Node_t* root, char* string, int str_len )
{
  char* str_end = strchr( string, ':' );
  size_t str_end_len = strlen( str_end );
  aAUF_Node_t* new_AUF = a_AUFNodeCreation();
  char* num_value = NULL;
  int count = 0;

  new_AUF->string = ParseString( ':', string, str_len );
  switch ( str_end[1] )
  {
    case '"':
      new_AUF->value_string = ParseString( '"', str_end+2, str_end_len-1 );
  
      if ( a_AUFAddChild( root, new_AUF ) < 0 )
      {
        printf( "Failed to add %s to root\n", new_AUF->string );
        return 1;
      }

      break;

    case '[':
      if ( str_end[2] == '"' )
      {
        count = 0;
        for ( size_t i = 3; i <= str_end_len; i++ )
        {
          char* str_value = ParseString( '"', str_end+i, str_end_len );
          if ( str_value != NULL )
          {
            size_t str_len = strlen( str_value );
            str_value[str_len] = '\0';
            i += str_len;
            
            if ( strchr( str_value, ',') ) continue;

            aAUF_Node_t* new_num = a_AUFNodeCreation();
            
            new_num->string = malloc( sizeof( char ) * MAX_LINE_LENGTH );
            if ( new_num->string == NULL )
            {
              printf("Failed to allocate memory for new_num->string: %s, %d\n", __FILE__, __LINE__ );
              return 1;
            }
            
            sprintf( new_num->string, "%d", count );
           
            new_num->value_string = str_value;

            a_AUFAddChild( new_AUF, new_num );
            count++;

          }

        }
        new_AUF->value_int = count;
        a_AUFAddChild( root, new_AUF );
      }
  
      else
      {
        count = 0;
        for ( size_t i = 2; i <= str_end_len; i++ )
        {
          char* num_value = ParseStringDoubleDelim( ',', ']', str_end+i, str_end_len );
          if ( num_value != NULL )
          {
            size_t num_len = strlen( num_value );
            num_value[num_len] = '\0';
            i += num_len;
            
            aAUF_Node_t* new_num = a_AUFNodeCreation();
            
            new_num->string = malloc( sizeof( char ) * MAX_LINE_LENGTH );
            if ( new_num->string == NULL )
            {
              printf("Failed to allocate memory for new_num->string: %s, %d\n", __FILE__, __LINE__ );
              return 1;
            }
            
            sprintf( new_num->string, "%d", count );
            
            if ( strchr( num_value, '.' ) )
            {
              new_num->value_double = atof( num_value );
            }

            else
            {
              new_num->value_int = atoi( num_value );
            }

            a_AUFAddChild( new_AUF, new_num );
            count++;

          }

        }
        new_AUF->value_int = count;
        a_AUFAddChild( root, new_AUF );
      }
      break;

    default:
      num_value = strchr( string, ':' );

      if ( strchr( num_value+1, '.' ) )
      {
        new_AUF->value_double = atof( num_value+1 );
      }

      else
      {
        new_AUF->value_int = atoi( num_value+1 );
      }
      
      if ( a_AUFAddChild( root, new_AUF ) < 0 )
      {
        printf( "Failed to add %s to root\n", new_AUF->string );
        return 1;
      }

      break;
  }

  return 0;
}

static char* ReadFile( const char* filename, int* file_size )
{
  long fileSize;
  char* fileString;
  FILE* file;

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

static char* ParseStringDoubleDelim( char delimiter1, char delimiter2, char* str, int str_len )
{
  char* return_str = (char*)malloc( sizeof(char) * str_len );
  if ( return_str == NULL )
  {
    printf( "Failed to allocate memory for return_str\n" );
    return NULL;
  }

  for ( int i = 0; i < str_len; i++ )
  {
    if ( str[i] != delimiter1 && str[i] != delimiter2 )
    {
      return_str[i] = str[i];

    }

    else
    {
      return_str[i] = '\0';
      return return_str;
    }
  }

  return NULL;
}
