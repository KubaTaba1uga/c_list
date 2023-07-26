#include <stddef.h>

#include "double_linked_list.h"

struct dl_body {
  void *value;
  struct dl_body *next;
  struct dl_body *prev;
};

struct dl_list {
  struct dl_body *head;
  struct dl_body *tail;
  struct dl_body *body;
  int length;
};

void init_dl_list(struct dl_list *l) {
  l->body = NULL;
  l->head = NULL;
  l->tail = NULL;
  l->length = 0;
}
