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

ar_list l;

Test(empty, test_arl_init_success) {
  size_t default_capacity = 255;

  arl_init(&l, default_capacity);

  cr_expect_not_null(l.array);
  cr_assert(eq(ulong, (unsigned long int)l.size, (unsigned long int)0));
  cr_assert(eq(ulong, (unsigned long int)l.capacity, (unsigned long int)255));
}

Test(empty, test_arl_count_new_capacity_base_doc) {
  /* Purpose of this function is mainly documentational. */
  /* Show array growth ratio by example. */
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

void initiate_arl_1000(void) { arl_init(&l, 1000); }

void free_l(void) { free(l.array); }

TestSuite(arl_1000, .init = initiate_arl_1000, .fini = free_l);

Test(arl_1000, test_arl_get_success) {
  void *value;
  int i;

  int expected_values[] = {6, 99, 699};
  size_t size = sizeof(expected_values) / sizeof(int);

  for (i = 0; (size_t)i < size; i++) {
    l.array[i] = &expected_values[i];
  }
  l.size = size;

  for (i = 0; (size_t)i < size; i++) {
    value = arl_get(&l, i);
    cr_assert_not_null(value);
  }
}

Test(arl_1000, test_arl_get_failure) {
  void *value;

  value = arl_get(&l, 5);
  cr_expect_null(value);

  value = arl_get(&l, -5);
  cr_expect_null(value);
}

Test(arl_1000, test_arl_set_success) {
  void *value;
  int i;

  int expected_values[] = {1, 2, 3, 4, 5};
  size_t values_size = sizeof(expected_values) / sizeof(int);

  l.size = values_size;
  for (i = 0; (size_t)i < values_size; i++) {
    value = arl_set(&l, i, &expected_values[i]);
    cr_assert_not_null(value);
  }

  for (i = 0; (size_t)i < values_size; i++) {
    cr_assert(eq(int, *(int *)(l.array[i]), expected_values[i]));
  }
}

Test(arl_1000, test_arl_set_failures) {
  void *value;
  int i = 3;

  value = arl_set(&l, 5, &i);
  cr_assert_null(value);

  value = arl_set(&l, -5, &i);
  cr_assert_null(value);
}
