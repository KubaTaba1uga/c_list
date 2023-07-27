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

ParameterizedTestParameters(dl_body_ready, single) {
  dl_body *ready_body;
  int *value_p;
  int value = 5;

  ready_body = (dl_body *)cr_malloc(sizeof(dl_body));
  value_p = (int *)cr_malloc(sizeof(int));

  ready_body->next = NULL;
  ready_body->prev = NULL;
  ready_body->value = value_p;

  *value_p = value;

  return cr_make_param_array(dl_body, ready_body, 1);
}

#include <criterion/new/assert.h>
ParameterizedTest(dl_body *val, dl_body_ready, single) {
  /* cr_fatal("Parameter: %p", val->value); */
  cr_fatal("Parameter: %i", *(int *)val->value);
}
