#include <stddef.h>

typedef struct dl_body {
  void *value;
  struct dl_body *next;
  struct dl_body *prev;
} dl_body;

typedef struct dl_list {
  struct dl_body *head;
  struct dl_body *tail;
  struct dl_body *body;
  int length;
} dl_list;

void init_dl_list(dl_list *l) {
  l->body = NULL;
  l->head = NULL;
  l->tail = NULL;
  l->length = 0;
}
