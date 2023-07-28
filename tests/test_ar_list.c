#include <criterion/assert.h>
#include <criterion/internal/assert.h>
#include <stddef.h>
#include <stdio.h>

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>
#include <stdlib.h>

// including source file instead of header file allows
//   for static functions testing.
// Do not forget adding `-zmuldefs` to gcc.
#include "../src/ar_list.c"

ar_list ready_l;

Test(empty, test_arl_init_success) {
  ar_list l;
  size_t ds = 255;

  arl_init(&l, ds);

  cr_expect_not_null(l.array);
  cr_expect(l.size == 0, "Expected \"l.length\" to have a value of 0.");
  cr_expect(l.capacity == 255, "Expected \"l.size\" to have a value of 255.");
}

Test(empty, test_arl_count_new_capacity_base_doc) {
  /* Purpose of this function is mainly documentational. */
  size_t size = 0;
  size_t capacity = 1000;

  capacity = arl_count_new_capacity_base(size, capacity);
  size = capacity;

  cr_assert(eq(ulong, (unsigned long int)capacity, (unsigned long int)1000));

  capacity = arl_count_new_capacity_base(size, capacity);
  size = capacity;

  cr_assert(eq(ulong, (unsigned long int)capacity, (unsigned long int)2500));

  capacity = arl_count_new_capacity_base(size, capacity);
  size = capacity;

  cr_assert(eq(ulong, (unsigned long int)capacity, (unsigned long int)6250));

  capacity = arl_count_new_capacity_base(size, capacity);
  size = capacity;

  cr_assert(eq(ulong, (unsigned long int)capacity, (unsigned long int)15625));
}

void setup_suite(void) { arl_init(&ready_l, 1000); }

void teardown_suite(void) { free(ready_l.array); }

TestSuite(ready_list, .init = setup_suite, .fini = teardown_suite);

Test(ready_list, test_arl_get_success) {
  // start list prep
  int expected_values[] = {6, 99, 699};
  size_t size = sizeof(expected_values) / sizeof(int);

  int i;
  for (i = 0; (size_t)i < size; i++) {
    ready_l.array[i] = &expected_values[i];
  }
  ready_l.size = size;
  // end list prep

  i = *(int *)arl_get(&ready_l, 0);
  cr_assert(eq(int, i, 6));

  i = *(int *)arl_get(&ready_l, 1);
  cr_assert(eq(int, i, 99));

  i = *(int *)arl_get(&ready_l, 2);
  cr_assert(eq(int, i, 699));
}

Test(ready_list, test_arl_get_failure) {
  void *value;

  value = arl_get(&ready_l, 0);
  cr_expect_null(value);

  value = arl_get(&ready_l, -5);
  cr_expect_null(value);

  // start list prep
  int expected_values[] = {1, 2, 3, 4, 5};
  size_t size = sizeof(expected_values) / sizeof(int);

  int i;
  for (i = 0; (size_t)i < size; i++) {
    ready_l.array[i] = &expected_values[i];
  }
  ready_l.size = size;
  // end list prep

  value = arl_get(&ready_l, size + 1);
  cr_expect_null(value);
}
