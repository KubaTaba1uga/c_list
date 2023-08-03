/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ar_list.h"
#include "utils/memory.h"

/* TO-DO */
/* 1. set up errno on failure */

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

  if (arl_is_i_invalid(l, i))
    return NULL;

  return l->array[i];
}

void *arl_set(ar_list *l, size_t i, void *value) {
  /* Set value under the index. */
  /* Index has to be smaller than l.size. */
  /* Do not expand array. */
  /* Return value on success. */
  /* Return NULL on failure. */

  if (arl_is_i_invalid(l, i))
    return NULL;

  l->array[i] = value;

  return l->array[i];
}

/* void *arl_append(ar_list *l, void *value) { */
/*   /\* Insert value to the last possible index. *\/ */
/*   /\* If no enough capacity, realloc array. *\/ */

/*   size_t last_i = l->size; */

/*   if (arl_is_i_invalid(l, last_i)) */
/*     arl_grow_array_capacity(l); */
/* } */

void *arl_insert(ar_list *l, size_t i, void *value) {
  /* Insert value to the index. */
  /* Return NULL on failure. */
  /* If no enough capacity, realloc array. */
  void *p;
  size_t j;

  if (arl_is_i_invalid(l, i))
    arl_grow_array_capacity(l);

  p = arl_move_indexes_by_positive_number(l, i, 1);
  if (!p)
    return NULL;

  p = arl_set(l, i, value);
  if (!p)
    return NULL;

  return p;
}

// SHRINK IS ONLY IN POP
// IF SIZE < CAPACITY / 3
//     CAPACITY = CAPACITY / 2

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
  return i >= (l->size);
}

static void *arl_grow_array_capacity(ar_list *l) {
  void *p;
  size_t new_capacity = arl_count_new_capacity(l->size, l->capacity);

  p = realloc(l->array, new_capacity);

  if (p) {
    l->capacity = new_capacity;
    l->array = p;
  }

  return p;
};

static void *arl_move_indexes_by_positive_number(ar_list *l, size_t start_i,
                                                 size_t move_by) {
  /* Move elements further by `move_by`, starting from `start_i`.
   * Set NULL on source value.  */
  /* Ex: */
  /*    INPUT  l.array {0, 1, 2, , ,}, start_i 1, move_by 2 */
  /*    OUTPUT l.array {0, NULL, NULL, 1, 2} */

  void *p;
  signed long int elements_to_move_no;
  size_t i, old_size, new_size, i_source, i_dest;

  old_size = l->size, new_size = l->size + move_by;
  elements_to_move_no = old_size - start_i;

  // TO-DO
  // set errno on validation error
  if (new_size > l->capacity)
    return NULL;
  if (elements_to_move_no <= 0)
    return NULL;

  l->size = new_size;

  start_i--;

  for (i = elements_to_move_no; i > 0; i--) {
    i_source = start_i + i;
    i_dest = i_source + move_by;

    p = arl_get(l, i_source);
    if (!p)
      goto CLEANUP;

    p = arl_set(l, i_dest, p);
    if (!p)
      goto CLEANUP;

    arl_set(l, i_source, NULL);
  }

  return l->array;

  // this should never be triggered by design
  // TO-DO
  // cleaning up half moved array, revert org
CLEANUP:
  l->size = old_size;
  return NULL;
}
