#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <criterion/assert.h>
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/new_asserts.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>

// including source file instead of header file allows
//   for static functions testing.
// Do not forget adding `-zmuldefs` to gcc.
#include "../src/ar_list.c"

ar_list l;

/* FIXTURES */
void initiate_arl_1000(void) { arl_init(&l, 1000); }
void initiate_arl_1000_filled(void) {
  arl_init(&l, 1000);
  int i, *jp;
  for (i = 0; i < l.capacity / 2; i++) {
    l.array[i] = jp = malloc(sizeof(int));
    *jp = i;
  }
  l.size = i;
}

void cleanup_arl_1000(void) { free(l.array); }
void cleanup_arl_1000_filled(void) {
  int i;
  for (i = 0; i < l.capacity / 2; i++) {
    free(l.array[i]);
  }
  free(l.array);
  l.size = 0;
}

/* TestSuite(arl_1000, .init = initiate_arl_1000, .fini = cleanup_arl_1000);
 */

/*  */

Test(arl_extern, test_arl_init_success) {
  ar_list l;
  size_t default_capacity = 255;

  arl_init(&l, default_capacity);

  cr_expect_not_null(l.array);
  cr_assert(eq(ulong, (unsigned long int)l.size, (unsigned long int)0));
  cr_assert(eq(ulong, (unsigned long int)l.capacity, (unsigned long int)255));
}

Test(arl_extern, test_arl_get_success, .init = initiate_arl_1000_filled,
     .fini = cleanup_arl_1000_filled) {
  void *value_p;
  size_t i;
  int i_value;

  for (i = 0; i < l.size / 2; i++) {
    value_p = arl_get(&l, i);
    cr_assert_not_null(value_p);

    i_value = *(size_t *)value_p;
    cr_assert(i_value == i, "Value should be %i, not %i\n", i, i_value);
  }
}

Test(arl_extern, test_arl_get_failure, .init = initiate_arl_1000,
     .fini = cleanup_arl_1000) {
  void *value;

  value = arl_get(&l, 5);
  cr_expect_null(value);
}

Test(arl_extern, test_arl_set_success, .init = initiate_arl_1000,
     .fini = cleanup_arl_1000) {
  int i;

  for (i = 0; (size_t)i < l.size / 2; i++) {
    cr_assert(eq(int, *(int *)(l.array[i]), i));
  }
}

Test(arl_extern, test_arl_set_failure, .init = initiate_arl_1000,
     .fini = cleanup_arl_1000) {
  void *value;
  int i = 3;

  value = arl_set(&l, 5, &i);
  cr_assert_null(value);
}

Test(arl_static, test_arl_count_new_capacity_base_doc) {
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

/* TO DO */
/* Corner cases : */
/* - list fully filled */
/* - list half filled */
/* - empty list */
Test(arl_static, test_arl_move_indexes_by_positive_number_success,
     .init = initiate_arl_1000_filled, .fini = cleanup_arl_1000_filled) {
  puts("TEST START!");

  void *value;
  int i, received_i;
  size_t size_before = l.size, move_by = 3, start_i = 33;

  value = arl_move_indexes_by_positive_number(&l, start_i, move_by);
  cr_assert_not_null(value);

  cr_assert(eq(int, l.size, size_before + move_by));

  for (i = 0; (size_t)i < l.size; i++) {
    cr_assert_not_null(l.array[i]);
    received_i = *(int *)l.array[i];

    if (i <= start_i) {
      cr_assert(eq(int, received_i, i));
    } else {
      cr_assert(eq(int, received_i, i - move_by));
    }
  }
}

/* Test(arl_static, test_arl_move_indexes_by_positive_number_failure, */
/*      .init = initiate_arl_1000_filled, .fini = cleanup_arl_1000_filled) { */

/*   size_t move_by = 3, start_i = 33; */

/*   value = arl_move_indexes_by_positive_number(&l, start_i, move_by); */
/*   cr_assert_not_null(value); */

/*   cr_assert(eq(int, l.size, size_before + move_by)); */

/*   for (i = 0; (size_t)i < l.size; i++) { */
/*     cr_assert_not_null(l.array[i]); */
/*     received_i = *(int *)l.array[i]; */

/*     if (i <= start_i) { */
/*       cr_assert(eq(int, received_i, i)); */
/*     } else { */
/*       cr_assert(eq(int, received_i, i - move_by)); */
/*     } */
/*   } */
/* } */
