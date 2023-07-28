/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */
#include <stdlib.h>

#include "ar_list.h"
#include "utils/memory.h"

static size_t arl_count_new_size(int max_i, size_t size) {
  /* List's growth ratio. */
  return (size_t)(3 * max_i / 2 + (int)size);
}

static size_t arl_count_new_size_with_pointer(int max_i, size_t size) {
  return arl_count_new_size(max_i, size) * get_pointer_size();
}

void *arl_init(ar_list *l, size_t default_size) {
  /* `default_size` tells maximum amount of pointers
   *     which list can store until realloc. */
  /*  `default_size` is parametrized for flexibility. */
  /* User is responsible for knowing the data. */

  void *list_body = arl_init_body(l, default_size);

  if (!list_body)
    return NULL;

  l->body = list_body;
  l->size = default_size;
  l->max_i = 0;

  return l;
}

void *arl_init_body(ar_list *l, size_t default_size) {
  /* return NULL if not enough memory. */
  return malloc(arl_count_new_size_with_pointer(0, default_size));
}
