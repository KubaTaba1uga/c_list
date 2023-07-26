#include <criterion/criterion.h>

#include "../src/dl_body.h"

Test(init_dl_body, test_correct_value) {
  dl_body b;
  int value = 9;

  init_dl_body(&b, &value);

  cr_expect(b.next == NULL, "Expected \"b.next\" to have a value of NULL.");
  cr_expect(b.prev == NULL, "Expected \"b.prev\" to have a value of NULL.");

  int received_value = *((int *)b.value);

  cr_expect(received_value == value,
            "Expected \"b.value\" to have a value of 9.");
}

Test(init_dl_body, test_incorrect_value) {
  dl_body b;
  int *value = NULL;

  init_dl_body(&b, value);

  int *received_value = ((int *)b.value);

  cr_expect(received_value == NULL,
            "Expected \"b.value\" to have a value of NULL.");
}
