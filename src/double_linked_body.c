#include <stddef.h>

#include "dl_body.h"

dl_body *init_dl_body(dl_body *b, void *value) {
  /* On success return pointer to initialized dl_body. */
  /* On failure return NULL pointer. */

  if (!value)
    return NULL;

  b->value = value;
  b->next = NULL;
  b->prev = NULL;

  return b;
};

void set_next_dl_body(dl_body *b, dl_body *next) { b->next = next; }

void set_prev_dl_body(dl_body *b, dl_body *prev) { b->prev = prev; }
