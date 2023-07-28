/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "ar_list.h"
#include "utils/memory.h"

void *arl_init(ar_list *l, size_t default_capacity) {
  /* `default_capacity` tells maximum amount of pointers
   *     which list can store until realloc. */
  /*  `default_capacity` is parametrized for flexibility. */
  /*  Which makes User responsible for knowing the data. */

  void *arl_array = arl_init_array(l, default_capacity);

  if (!arl_array)
    return NULL;

  l->array = arl_array;
  l->capacity = default_capacity;
  l->size = 0;

  return l;
}

void *arl_get(ar_list *l, size_t i) {
  /* Get value under the index. */
  /* Return NULL on failure. */

  // Validate index
  if (arl_is_i_invalid(l, i))
    return NULL;

  return l->array[i];
}

void *arl_set(ar_list *l, size_t i, void *value) {
  /* Set value under the index. */
  /* Index has to be smaller than l.size. */
  /* Do not expand array size. */
  /* Return value on success. */
  /* Return NULL on failure. */

  // Validate index
  if (i < 0)
    return NULL;

  if (i > l->size)
    return NULL;

  l->array[i] = value;

  return value;
}

/* void *arl_append(ar_list *l, void *value) { */
/*   /\* Insert value to the last possible index. *\/ */
/*   /\* If no enough capacity, realloc array. *\/ */
/* } */

/* void *arl_insert(ar_list *l, size_t i, void *value) { */
/*   /\* Insert value to the index. *\/ */
/*   /\* Return NULL on failure. *\/ */
/*   /\* If no enough capacity, realloc array. *\/ */
/* } */

static void *arl_init_array(ar_list *l, size_t default_capacity) {
  /* return NULL if not enough memory. */
  return malloc(arl_count_new_capacity(0, default_capacity));
}

static size_t arl_count_new_capacity(size_t size, size_t capacity) {
  return arl_count_new_capacity_base(size, capacity) * get_pointer_size();
}

static size_t arl_count_new_capacity_base(size_t size, size_t capacity) {
  /* List's growth ratio. */
  return (size_t)(3 * size / 2 + capacity);
}

static bool arl_is_i_invalid(ar_list *l, size_t i) {
  /* Return true if i is not in l->array boundaries. */
  return ((i > (l->size)) || (i < 0));
}
