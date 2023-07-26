#include <stddef.h>

typedef struct body {
  void *value;
  struct body *next;
  struct body *prev;
} body;

typedef struct {
  struct body *head;
  struct body *tail;
  struct body *body;
  int length;
} dl_list;

void init_dl_list(dl_list *l) {
  l->body = NULL;
  l->head = NULL;
  l->tail = NULL;
  l->length = 0;
}
