#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#ifndef osbList
#define osbList

#define LSMODE_DYNAMIC -1

#define LSPRINT_INT 0
#define LSPRINT_STR 1

// MESSAGES
#define M_LS_DYNAMIC_PUSH "WARNING: Using lspush with a dynamic list isn't supported, use lsdpush instead. (skipped)"

/**
 * @brief List head struct, contains general data about a list.
 */
typedef struct List
{
  int esize;
  int length;
  struct Node *first_element;
  struct Node *last_element;
} List;

/**
 * @brief List element struct, each node links to a next node or NULL if not exists.  
 */
typedef struct Node
{
  void *value;
  struct Node *next;
} Node;

/**
 * @brief Creates a new linked list with a given size for its elements.
 * 
 * @param size: Size of list elements.
 * LSMODE_DYNAMIC can be used for as size for dynamic element size (like char*)
 * 
 * @retval List* - Created list
 */
List *lscreate(int size);

/**
 * @brief	Pushes a new element into the list at the end. The list must not be dynamic otherwise the function will be skipped.
 * 
 * @param  *list: list pointer to manipulate
 * @param  *value: the value to insert into the list
 * 
 * @retval None
 */
void lspush(List *list, void *value);

/**
 * @brief  	Pushes a new dynamic sized element into the list at the end of it.
 * 
 * @param  *list: list pointer to manipulate
 * @param  *value: the value to insert into the list
 * @param  size: the size of value the value in bytes
 * 
 * @retval None
 */
void lsdpush(List *list, void *value, int size);

/**
 * @brief Remove an element from the list at a given index. 
 * This will automatically link back previous element to the next one relative to the index given
 * 
 * @param  *list: list pointer to manipulate
 * @param  index: index of the element to remove in the list
 * 
 * @retval None
 */
void lsremove(List *list, int index);

/**
 * @brief  Free the memory allocations in the heap for the given list. 
 * This will free the list itself but also all the elements and their values
 * 
 * @param  *list: list pointer to free
 * 
 * @retval None
 */
void lsfree(List *list);

/**
 * @brief  Insert an element to a given list at a given index. 
 * This will shift the current index to the next position.
 * 
 * @param  *list: list pointer to manipulate
 * @param  *value: the value to insert into the list
 * @param  index: index of the element to insert in the list
 * 
 * @retval None
 */
void lsinsert(List *list, int index, void *value);

/**
 * @brief  Insert a dynamic element at a given index in a list.
 * This will shift the current index to the next position.
 * 
 * @param  *list: list pointer to manipulate
 * @param  index: position where to add the element
 * @param  *value: value to insert
 * @param  size: size of the value to insert
 * 
 * @retval None
 */
void lsdinsert(List *list, int index, void *value, int size);

/**
 * @brief  Out all list values into the console.
 * 
 * @param  *list: list pointer to print
 * @param  type: type of the values to print (LSPRINT_INT, LSPRINT_STR, ...)
 * 
 * @retval None
 */
void lsprint(List *list, char type);

/**
 * @brief  Free node values
 * 
 * @param  *node: Pointer to the node to free 
 * 
 * @retval None
 */
void lsfreenode(Node *node);

/**
 * @brief  Get an element from the list at a given index.
 * 
 * @param  *list: list that contains element to search
 * @param  index: index of the element in the list
 * 
 * @retval Node* || NULL
 */
Node *lsget(List *list, int index);

#endif
