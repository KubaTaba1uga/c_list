/* List structure: */
/*      void *array_p */
/*      int length */

/* Append example: */
/*      1. take pointer to value from user */
/*	2. enlarge array_p by size of the pointer */
/*	3. assign the pointer to array_p[length] */
/*      4. increase length */

/* A lot of reallocs: */
/*   To not do realloc on each append/insert/delete */
/*   malloc for array_p and realloc only if size not enough */
/*   or decreased significantly. */
/* Idea came from here
 * https://stackoverflow.com/questions/19206005/array-of-structs-memory-realloc-stderr/19206121#19206121
 */

#include <stddef.h>

typedef struct {

  /* `body` is an array of generic pointers. */
  /* Values do not need to be of the same type. */
  void *body;

  /* `max_i` is the list's iteration upper boundary. */
  /* Useful when read from the list. */
  int max_i;

  /* `size` is maximum amount of pointers that */
  /*   body can hold. */
  /* Useful when write to the list. */
  size_t size;
} list;

extern void *l_init_body(list *l, size_t default_size);
extern void *l_init_list(list *l, size_t default_size);
