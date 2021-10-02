#include "list.h"

List *lscreate(int size)
{
  List list;
  list.length = 0;
  list.first_element = NULL;
  list.esize = size;

  List *pointer = (List *)malloc(sizeof(list));
  memcpy(pointer, &list, sizeof(list));
  return pointer;
}

Node *lsget(List *list, int index)
{
  if (index < 0)
  {
    printf("lsget: Value canno't be negative.");
    return NULL;
  }

  Node *current = list->first_element;
  int i = 0;
  while (current)
  {
    if (i == index)
      return current;
    i++;
    current = current->next;
  }
  return NULL;
}

void lspush(List *list, void *value)
{
  if (list->esize == LSMODE_DYNAMIC)
  {
    printf("%s\n", M_LS_DYNAMIC_PUSH);
    return;
	}

  lsdpush(list, value, list->esize);
}

void lsdpush(List *list, void *value, int size)
{
  Node *node_pointer = (Node *)malloc(sizeof(Node));
  node_pointer->next = NULL;
  node_pointer->value = malloc(size);
  memcpy(node_pointer->value, value, size);

  if (!list->first_element)
  {
    list->first_element = node_pointer;
    list->length++;
    return;
  }

  Node *current = list->first_element;
  while (current)
  {
    Node *next = current->next;
    if (!next)
    {
      current->next = node_pointer;
      list->length++;
      return;
    }

    current = next;
  }

  // If fails, free both allocated memory.
  free(node_pointer->value);
  free(node_pointer);
}

void lsremove(List *list, int index)
{
  Node *prev_node = lsget(list, index - 1);
  if (prev_node)
  {
    if (index != list->length - 1)
    {
      Node *next_node = prev_node->next->next;
      lsfreenode(prev_node->next);
      prev_node->next = next_node;
      return;
    }

    lsfreenode(prev_node->next);
    prev_node->next = NULL;
    return;
  }
  else if (index == 0)
  {
    if (index != list->length - 1)
    {
      lsfreenode(list->first_element);
      list->first_element = NULL;
      return;
    }

    Node *next_node = list->first_element->next;
    lsfreenode(list->first_element);
    list->first_element = next_node;
    return;
  }
}

void lsinsert(List *list, int index, void *value)
{
  if (list->esize == LSMODE_DYNAMIC)
  {
    printf("%s\n", M_LS_DYNAMIC_PUSH);
    return;
  }

  lsdinsert(list, index, value, list->esize);
}

void lsdinsert(List *list, int index, void *value, int size)
{
  if (index < 0)
  {
    printf("lsdinsert: Value cannot be negative.");
    return;
  }

  Node *prev_node = lsget(list, index - 1);
  if (index <= list->length && prev_node)
  {
    Node *current = prev_node->next;
    Node *node_pointer = (Node *)malloc(sizeof(Node));
    node_pointer->next = current;
    node_pointer->value = malloc(size);
    memcpy(node_pointer->value, value, size);
    prev_node->next = node_pointer;
    list->length++;
    return;
  }
  else if (index > list->length)
  {
    lsdpush(list, value, size);
  }
}

void lsprint(List *list, char type)
{
  if (list->length > 0)
  {
    printf("=== LIST ===\n");
    Node *current = list->first_element;
    int i = 0;
    while (current)
    {
      switch (type)
			{
        case LSPRINT_INT: {
          int *number = (int *)current->value;
          printf("%i	> %i\n", i, *number);
          break;
        }

        case LSPRINT_STR: {
          char *string = (char *)current->value;
          printf("%i	> %s\n", i, string);
          break;
        }

        default: {
          printf("%i	> %p\n", i, current->value);
          break;
			  }
      }

      current = current->next;
      i++;
    }
  }
  else
  {
    printf("List %p is empty.\n", list);
  }
}

void lsfreenode(Node *node)
{
  free(node->value);
  free(node);
}

void lsfree(List *list)
{
  Node *current = list->first_element;
  while (current)
  {
    Node *tmp = current->next;
    lsfreenode(current);
    current = tmp;
  }

  free(list);
}
