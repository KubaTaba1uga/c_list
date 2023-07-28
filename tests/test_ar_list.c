#include <stddef.h>
#include <stdio.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>

// including source file instead of header file allows
//   for static functions testing.
// Do not forget adding `-zmuldefs` to gcc.
#include "../src/ar_list.c"

Test(init_list, success) {
  ar_list l;
  size_t ds = 255;

  arl_init(&l, ds);

  cr_expect_not_null(l.body);
  cr_expect(l.max_i == 0, "Expected \"l.length\" to have a value of 0.");
  cr_expect(l.size == 255, "Expected \"l.size\" to have a value of 255.");
}

Test(count_new_size_list, 0and1000) {
  /* Purpose of this function is mainly documentational. */
  int len = 0;
  size_t size = 1000;

  size = arl_count_new_size(len, size);
  len = size;

  cr_assert(eq(ulong, (unsigned long int)size, (unsigned long int)1000));

  size = arl_count_new_size(len, size);
  len = size;

  cr_assert(eq(ulong, (unsigned long int)size, (unsigned long int)2500));

  size = arl_count_new_size(len, size);
  len = size;

  cr_assert(eq(ulong, (unsigned long int)size, (unsigned long int)6250));

  size = arl_count_new_size(len, size);
  len = size;

  cr_assert(eq(ulong, (unsigned long int)size, (unsigned long int)15625));
}
