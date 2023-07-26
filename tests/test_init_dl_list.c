#include <criterion/criterion.h>
#include <string.h>

#include "../src/double_linked_list.h"

Test(sample, test) {
  cr_expect(strlen("Test") == 4, "Expected \"Test\" to have a length of 4.");
  cr_expect(strlen("Hello") == 4, "This will always fail, why did I add this?");
  cr_assert(strlen("") == 0);
}

Test(init_dl_list, test_values_NULL) {
  struct dl_list l;

  init_dl_list(&l);

  cr_expect(l.body == NULL, "Expected \"l.body\" to have a value of NULL.");
  cr_expect(l.head == NULL, "Expected \"l.head\" to have a value of NULL.");
  cr_expect(l.tail == NULL, "Expected \"l.tail\" to have a value of NULL.");

  cr_expect(strlen("Hello") == 4, "This will always fail, why did I add this?");
  cr_assert(strlen("") == 0);
}
