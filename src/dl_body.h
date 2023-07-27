typedef struct dl_body_ {
  struct dl_body_ *next;
  struct dl_body_ *prev;
  void *value;
} dl_body;

dl_body *init_dl_body(dl_body *b, void *value);

/* Getters*/
dl_body *get_next_dl_body(dl_body *b);
dl_body *get_prev_dl_body(dl_body *b);
dl_body *get_value_dl_body(dl_body *b);

/* Setters */
void set_next_dl_body(dl_body *b, dl_body *next);
void set_prev_dl_body(dl_body *b, dl_body *prev);
void *set_value_dl_body(dl_body *b, void *value);
