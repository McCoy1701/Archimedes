
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Archimedes.h"

aAUF_t* a_AUFCreation( void )
{
  aAUF_t* new_AUF = malloc( sizeof( aAUF_t ) );
  if ( new_AUF == NULL )
  {
    printf( "Failed to allocate memory for root: %s, %d\n", __FILE__, __LINE__ );
    return NULL;
  }

  new_AUF->head = NULL;
  new_AUF->tail = NULL;
  new_AUF->size = 0;

  return new_AUF;
}

aAUF_Node_t* a_AUFNodeCreation( void )
{
  aAUF_Node_t* new_AUF = malloc( sizeof( aAUF_Node_t ) );
  if ( new_AUF == NULL )
  {
    printf( "Failed to allocate memory for node: %s, %d\n", __FILE__, __LINE__ );
    return NULL;
  }
  
  new_AUF->next         = NULL;
  new_AUF->prev         = NULL;
  new_AUF->child        = NULL;

  new_AUF->string       = NULL;
  new_AUF->value_string = NULL;
  new_AUF->value_double = 0.0f;
  new_AUF->value_int    = 0;
  new_AUF->type         = 0;
  
  return new_AUF;
}

int a_AUFAddNode( aAUF_t* root, aAUF_Node_t* node )
{
  if ( root == NULL || node == NULL )
  {
    printf( "Invalid input for add_node: %s, %d \n", __FILE__, __LINE__ );
    return 1;
  }

  node->next = NULL;
  node->prev = NULL;

  if ( root->head == NULL )
  {
    root->head = node;
    root->tail = node;
    return 0;
  }

  else
  {
    root->tail->next = node;
    root->tail = node;
  }
  
  root->size++;

  return 0;
}

int a_AUFAddChild( aAUF_Node_t* root, aAUF_Node_t* node )
{
  if ( root == NULL || node == NULL )
  {
    printf( "Head, Child, or node is NULL: %s, %d\n", __FILE__, __LINE__ );
    return 1;
  }
  
  if ( root->child == NULL )
  {
    root->child = node;
    node->prev = NULL;
    node->next = NULL;
    return 0;
  }

  aAUF_Node_t* current = root->child;
  while ( current->next != NULL )
  {
    current = current->next;
  }

  current->next = node;
  node->prev = current;
  node->next = NULL;

  return 0;
}

int a_AUFNodeFree( aAUF_Node_t* head )
{
  aAUF_Node_t* next = NULL;
  while ( head != NULL )
  {
    next = head->next;
    
    if ( head->child != NULL )
    {

      a_AUFNodeFree( head->child );
    }
    
    if ( head->string != NULL )
    {
      free( head->string );
    }

    if ( head->value_string != NULL )
    {
      free( head->value_string );

    }

    free( head );
    head = next;
  }
  
  return 0;
}

int a_AUFFree( aAUF_t* root )
{
  if ( root->head == NULL )
  {
    printf( "Root head is already NULL: %s, %d\n", __FILE__, __LINE__ );
    return 1;
  }

  a_AUFNodeFree( root->head );
  
  root->head = NULL;
  root->tail = NULL;
  root->size = 0;
  
  return 0;
}

aAUF_Node_t* a_GetObjectItem( aAUF_Node_t* node, char* object_str )
{
  if ( node == NULL )
  {
    printf( "Node is NULL: %s, %d\n", __FILE__, __LINE__ );
    return NULL;
  }

  aAUF_Node_t* current = node->child;
  while ( current != NULL )
  {
    if ( current->string != NULL && strcmp( current->string, object_str ) == 0 )
    {
      return current;
    }

    current = current->next;
  }

  return NULL;
}

