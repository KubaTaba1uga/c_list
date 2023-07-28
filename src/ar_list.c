/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */
#include <stdlib.h>

#include "ar_list.h"
#include "utils/memory.h"

void *arl_init(ar_list *l, size_t default_capacity) {
  /* `default_capacity` tells maximum amount of pointers
   *     which list can store until realloc. */
  /*  `default_capacity` is parametrized for flexibility. */
  /* User is responsible for knowing the data. */

  void *arl_array = arl_init_array(l, default_capacity);

  if (!arl_array)
    return NULL;

  l->array = arl_array;
  l->capacity = default_capacity;
  l->size = 0;

  return l;
}

static void *arl_init_array(ar_list *l, size_t default_capacity) {
  /* return NULL if not enough memory. */
  return malloc(arl_count_new_capacity(0, default_capacity));
}

static size_t arl_count_new_capacity(int max_i, size_t size) {
  return arl_count_new_capacity_base(max_i, size) * get_pointer_size();
}

static size_t arl_count_new_capacity_base(int max_i, size_t size) {
  /* List's growth ratio. */
  return (size_t)(3 * max_i / 2 + (int)size);
}
