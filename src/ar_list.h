/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */
#include <stddef.h>

typedef struct {

  void *array;

  /* The number of elements.*/
  size_t size;

  /* The size of the underlying array, */
  /* which is the maximum possible size without relocating data. */
  size_t capacity;
} ar_list;

extern void *arl_init(ar_list *l, size_t default_size);

static void *arl_init_array(ar_list *l, size_t default_size);
static size_t arl_count_new_capacity(int max_i, size_t size);
static size_t arl_count_new_capacity_base(int max_i, size_t size);
