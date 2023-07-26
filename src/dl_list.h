#include "dl_body.h"

typedef struct {
  dl_body *head;
  dl_body *tail;
  int length;
} dl_list;

void init_dl_list(dl_list *l);
