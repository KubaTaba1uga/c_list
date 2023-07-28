/* Array list implementation, as described here: */
/*   https://en.wikipedia.org/wiki/Dynamic_array   */
#include <stddef.h>

typedef struct {

  /* The number of elements.*/
  size_t size;

  /* The size of the underlying array, */
  /* which is the maximum possible size without relocating data. */
  size_t capacity;

  void **array;
} ar_list;

extern void *arl_init(ar_list *l, size_t default_size);
extern void *arl_get(ar_list *l, size_t i);

static void *arl_init_array(ar_list *l, size_t default_size);
static size_t arl_count_new_capacity(size_t size, size_t capacity);
static size_t arl_count_new_capacity_base(size_t size, size_t capacity);