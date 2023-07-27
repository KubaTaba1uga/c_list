#include <criterion/criterion.h>
#include <criterion/internal/assert.h>

#include <stddef.h>
#include <stdio.h>

#include "../src/list.h"

Test(init_list, success) {
  list l;
  size_t ds = 255;

  l_init_list(&l, ds);

  cr_expect_not_null(l.body);
  cr_expect(l.max_i == 0, "Expected \"l.length\" to have a value of 0.");
  cr_expect(l.size == 255, "Expected \"l.size\" to have a value of 255.");
}

/* Test(count_new_size_list, 0and1000) { */
/*   int len = 0; */
/*   size_t size = 1000; */

/*   size = count_new_size_list(len, size); */
/*   printf("%i\n", size); */
/* } */
