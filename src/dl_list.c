#include <stddef.h>

#include "dl_list.h"

void init_dl_list(dl_list *l) {
  l->head = NULL;
  l->tail = NULL;
  l->length = 0;
}
