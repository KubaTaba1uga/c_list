#include <criterion/alloc.h>
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/parameterized.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/dl_body.h"

Test(init_dl_body, test_valid_value) {
  dl_body b;
  int value = 9;

  dl_body *b_p = init_dl_body(&b, &value);
  cr_assert_not_null(b_p);

  cr_assert_null(b.next);
  cr_assert_null(b.prev);

  int received_value = *((int *)b.value);

  cr_expect(received_value == value,
            "Expected \"b.value\" to have a value of 9.");
}

Test(init_dl_body, test_invalid_value) {
  dl_body b;
  int *value = NULL;

  dl_body *b_p = init_dl_body(&b, value);

  cr_assert_null(b_p);
}

Test(set_dl_body, test_valid_value) {
  dl_body b, next_b, prev_b;
  int value = 9;

  set_next_dl_body(&b, &next_b);
  set_prev_dl_body(&b, &prev_b);
  set_value_dl_body(&b, &value);

  cr_expect(b.next == &next_b,
            "Expected \"b.next\" to have address of \"next_b\".");
  cr_expect(b.prev == &prev_b,
            "Expected \"b.prev\" to have address of \"prev_b\".");
  cr_expect(b.value == &value,
            "Expected \"b.value\" to have address of \"value_b\".");
}

void dl_body_in_list_ctx_cleanup(struct criterion_test_params *crp) {
  dl_body *b = crp->params;
  dl_body *b_next = b->next, *b_prev = b->prev;

  dl_body *b_to_clean[] = {b, b_next, b_prev};

  int max_i = (int)(sizeof(b_to_clean) / sizeof(dl_body *));

  while (max_i--) {
    cr_free(b_to_clean[max_i]->value);
    cr_free(b_to_clean[max_i]);
  }
}

ParameterizedTestParameters(dl_body_in_list_ctx, test_getters) {
  /* Body chunk in list context has setted up attributes: */
  /*   - next */
  /*   - prev */
  /*   - value */

  dl_body *b, *b_next, *b_prev;
  dl_body **b_to_alloc[] = {&b, &b_next, &b_prev};
  int i;

  for (i = 0; i < (int)(sizeof(b_to_alloc) / sizeof(dl_body *)); i++) {
    dl_body *b_local;

    *b_to_alloc[i] = b_local = (dl_body *)cr_malloc(sizeof(dl_body));

    int *v_local = (int *)cr_malloc(sizeof(int));
    *v_local = i;

    init_dl_body(b_local, v_local);
  }

  set_prev_dl_body(b, b_prev);
  set_next_dl_body(b_prev, b);

  set_prev_dl_body(b_next, b);
  set_next_dl_body(b, b_next);

  return cr_make_param_array(dl_body, b, 1, dl_body_in_list_ctx_cleanup);
}

ParameterizedTest(dl_body *b, dl_body_in_list_ctx, test_getters) {
  dl_body *b_next, *b_prev;
  int received_value;

  b_next = get_next_dl_body(b);
  cr_assert_not_null(b_next);

  b_prev = get_prev_dl_body(b);
  cr_assert_not_null(b_prev);

  received_value = *(int *)get_value_dl_body(b);

  cr_expect(received_value == 0,
            "Expected \"*b->value\" to have value of \"0\".");

  received_value = *(int *)get_value_dl_body(b_next);

  cr_expect(received_value == 1,
            "Expected \"*b_next->value\" to have value of \"1\".");

  received_value = *(int *)get_value_dl_body(b_prev);

  cr_expect(received_value == 2,
            "Expected \"*b_prev->value\" to have value of \"2\".");
}
