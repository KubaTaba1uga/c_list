#include <criterion/criterion.h>

#include "../src/dl_list.h"

Test(init_dl_list, test_values_NULL) {
  dl_list l;

  init_dl_list(&l);

  cr_expect(l.head == NULL, "Expected \"l.head\" to have a value of NULL.");
  cr_expect(l.tail == NULL, "Expected \"l.tail\" to have a value of NULL.");
  cr_expect(l.length == 0, "Expected \"l.length\" to have a value of 0.");
}
