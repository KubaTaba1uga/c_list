#include <criterion/alloc.h>
#include <criterion/criterion.h>
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

ParameterizedTestParameters(dl_body_in_list_ctx, test_getters) {
  /* Body chunk in list context has setted up attributes: */
  /*   - next */
  /*   - prev */
  /*   - value */
  dl_body *b, *b_next, *p_body;
  int *value_p;
  int value = 5;

  b = (dl_body *)cr_malloc(sizeof(dl_body));
  b_next = (dl_body *)cr_malloc(sizeof(dl_body));
  p_body = (dl_body *)cr_malloc(sizeof(dl_body));
  value_p = (int *)cr_malloc(sizeof(int));

  init_dl_body(b, value_p);
  init_dl_body(b_next, value_p);
  init_dl_body(p_body, value_p);

  set_prev_dl_body(b, p_body);
  set_next_dl_body(p_body, b);

  set_prev_dl_body(b_next, b);
  set_next_dl_body(b, b_next);

  *value_p = value;

  return cr_make_param_array(dl_body, b, 1);
}

#include <criterion/new/assert.h>
ParameterizedTest(dl_body *b, dl_body_in_list_ctx, test_getters) {
  dl_body *b_next, b_prev;
  int value = 5;

  /* cr_fatal("Parameter: %p", val->value); */
  cr_fatal("Parameter: %i", *(int *)b->value);
}
