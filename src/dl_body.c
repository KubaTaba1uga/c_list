#include <stddef.h>

#include "dl_body.h"

dl_body *init_dl_body(dl_body *b, void *value) {
  /* On success return pointer to initialized dl_body. */
  /* On failure return NULL pointer. */

  // Value cannot be NULL, so dereferencing NULL value
  //  is not an issue.
  if (!set_value_dl_body(b, value))
    return NULL;

  set_next_dl_body(b, NULL);
  set_prev_dl_body(b, NULL);

  return b;
};

/* Getters */
dl_body *get_next_dl_body(dl_body *b) { return b->next; }

dl_body *get_prev_dl_body(dl_body *b) { return b->prev; }

dl_body *get_value_dl_body(dl_body *b) { return b->value; }

/* Setters */
void set_next_dl_body(dl_body *b, dl_body *next) { b->next = next; }

void set_prev_dl_body(dl_body *b, dl_body *prev) { b->prev = prev; }

void *set_value_dl_body(dl_body *b, void *value) {
  if (!value)
    return NULL;

  b->value = value;

  return b;
}
