/* List structure: */
/*      void *array_p */
/*      int length */

/* Append example: */
/*      1. take pointer to value from user */
/*	2. enlarge array_p by size of the pointer */
/*	3. assign the pointer to array_p[length] */
/*      4. increase length */

/* A lot of reallocs issue: */
/*   To not do realloc on each append/insert/delete */
/*   malloc for array_p and realloc only if size not enough */
/*   or decreased significantly. */
/* Idea came from here
 * https://stackoverflow.com/questions/19206005/array-of-structs-memory-realloc-stderr/19206121#19206121
 */

#include <stddef.h>
#include <stdlib.h>

#include "list.h"

#ifndef UNITTESTS
#define STATIC
#else
#define STATIC static
#endif

STATIC size_t l_count_new_size(int max_i, size_t size) {

  /* For init max_i=0 and default_size=1000, it goes sth like: */
  /*	1000 -> 2500 -> 6250 -> 8125 -> 13187 -> 20780 */

  return (size_t)(3 * max_i / 2 + (int)size);
}

STATIC size_t l_get_pointer_size(void) { return sizeof(void *); }

STATIC size_t l_count_new_size_with_pointer(int max_i, size_t size) {
  return l_count_new_size(max_i, size) * l_get_pointer_size();
}

void *l_init_body(list *l, size_t default_size) {
  /* return NULL if not enough memory. */
  return malloc(l_count_new_size_with_pointer(0, default_size));
}

void *l_init_list(list *l, size_t default_size) {
  /* `default_size` tells maximum amount of pointers
   *     which list can store until realloc. */
  /*  `default_size` is parametrized for flexibility. */
  /* User is responsible for knowing the data. */

  void *list_body = l_init_body(l, default_size);

  if (!list_body)
    return NULL;

  l->body = list_body;
  l->size = default_size;
  l->max_i = 0;

  return l;
}
