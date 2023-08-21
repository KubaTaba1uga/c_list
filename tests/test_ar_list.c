/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test framework dependencies  */
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
/* Test framework */
#include <cmocka.h>

// including source file instead of header file allows
//   for static functions testing.
// Do not forget adding `-zmuldefs` to gcc.
#include "../src/ar_list.c"

/*******************************************************************************
 *    UTILS
 ******************************************************************************/

size_t write_values_to_array(size_t size, void *array[], int values[]) {
  /* Array and values has to be at least the same size. */
  /* Return number of elements written to array. */

  void *p;
  size_t i;

  for (i = 0; i < size; i++) {
    p = malloc(sizeof(int));
    if (!p)
      continue;

    *(int *)p = values[i];
    array[i] = p;
  }
  return i;
}

void print_array(size_t size, void *array[size]) {
  size_t i;
  puts("{");
  for (i = 0; i < size; i++) {
    printf("%p,\n", array[i]);
  }
  puts("}");
}

void print_size_and_capacity(size_t size, size_t capacity) {
  printf("Size: %lu, Capacity: %lu\n", size, capacity);
}

/*******************************************************************************
 *    TESTS DECLARATIONS
 ******************************************************************************/
ar_list l;

int *l_values;
size_t l_values_size;

/*******************************************************************************
 *    TESTS DATA
 ******************************************************************************/
int arl_small_values[] = {0, 1, 2, 3, 4, 5};
int arl_empty_values[] = {};

/*******************************************************************************
 *    FIXTURES
 ******************************************************************************/

static void initiate_l(void) {
  void *p;
  size_t i;

  p = arl_init(&l, l_values_size);
  if (!p)
    exit(1);

  i = write_values_to_array(l_values_size, l.array, l_values);
  if (i != l_values_size)
    exit(2);

  l.size = l_values_size;
}

static void cleanup_l(void) {
  size_t i;
  for (i = l.size; i < l.size; i++) {
    free(l.array[i]);
  }

  l.size = 0;
}

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

static int setup_arl_small_full(void **state) {

  void *p;

  l_values = malloc(sizeof(arl_small_values));
  if (!l_values)
    exit(3);

  l_values_size = sizeof(arl_small_values) / sizeof(int);

  p = memcpy(l_values, arl_small_values, l_values_size);
  if (!p)
    exit(4);

  initiate_l();

  return 0;
}

static int teardown_arl(void **state) {
  cleanup_l();

  return 0;
}

static int setup_arl_small_empty(void **state) {

  void *p;

  l_values = malloc(sizeof(arl_empty_values));
  if (!l_values)
    exit(5);

  l_values_size = 0;

  p = memcpy(l_values, arl_empty_values, l_values_size);
  if (!p)
    exit(6);

  initiate_l();

  return 0;
}

/*******************************************************************************
 *    PUBLIC API TESTS
 ******************************************************************************/

void test_arl_init_success(void **state) {
  size_t default_capacity = 255;

  arl_init(&l, default_capacity);

  assert_non_null(l.array);
  assert_int_equal(l.size, 0);
  assert_int_equal(l.capacity, 255);
}

/* /\* TO-DO *\/ */
/* /\* mock test_arl_init_failure, malloc *\/ */

void test_arl_get_success(void **state) {
  int *expected_values = l_values;
  size_t i, values_size = l_values_size;
  void *p;

  assert_int_equal(l.capacity, values_size);

  for (i = 0; i < values_size; i++) {
    p = arl_get(&l, i);
    assert_non_null(p);
    assert_int_equal(*(int *)p, expected_values[i]);
  }
}

/*******************************************************************************
 *    INTERNAL API TESTS
 ******************************************************************************/

void test_arl_count_new_capacity_base_doc(void **state) {
  /* Show array capacity growth ratio by example. */
  /* Purpose of this function is mainly documentational. */

  size_t expected_values[] = {1000, 2500, 6250, 15625};
  size_t size = 0;
  size_t capacity = 1000;
  size_t i;

  for (i = 0; i < (sizeof(expected_values) / sizeof(size_t)); i++) {
    capacity = size = arl_count_new_capacity_base(size, capacity);

    assert_int_equal(capacity, expected_values[i]);
  }
}

void test_arl_is_i_invalid_true(void **state) {
  size_t j, i_to_check[] = {l_values_size, ULONG_MAX};
  bool is_invalid;

  for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) {
    is_invalid = arl_is_i_invalid(&l, i_to_check[j]);

    if (!is_invalid) {
      print_array(l.size, l.array);
      fail_msg("arl_is_i_invalid(&l, %zu) = %s\n", i_to_check[j], "false");
    }
  }
}

/* Test(arl_full_array, test_arl_is_i_invalid_true) { */

/*   size_t j, i_to_check[] = {l_values_size + 1, ULONG_MAX}; */
/*   bool received, expected = true; */

/*   for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) { */
/*     received = arl_is_i_invalid(&l, i_to_check[j]); */

/*     cr_expect(received == expected, */
/*               "arl_is_i_invalid(&l, %lu) = %s" */
/*               "\n" */
/*               "l.array={%lu,%lu,%lu,%lu,%lu}" */
/*               "\n", */
/*               i_to_check[j], "false", *(int *)l.array[0], *(int *)l.array[1],
 */
/*               *(int *)l.array[2], *(int *)l.array[3], *(int *)l.array[4]); */
/*   } */
/* } */

/* TO-DO */
/* parametrized expected values */
/* count dynamically expected values sign */

/* /\* FIXTURES *\/ */
/* int l_values[] = {0, 1, 2, 3, 4, 5}; */
/* size_t l_values_size = sizeof(l_values) / sizeof(int); */
/* size_t arl_small_half_size = 3; */

/* void initiate_arl_small(void) { arl_init(&l, l_values_size); } */
/* void initiate_arl_small_fullfilled(void) { */
/*   void *p; */
/*   size_t i; */

/*   p = arl_init(&l, l_values_size); */
/*   if (!p) */
/*     exit(1); */

/*   i = write_values_to_array(l_values_size, l.array, l_values); */
/*   if (i != l_values_size) */
/*     exit(2); */

/*   l.size = i; */
/* } */
/* void initiate_arl_small_halffilled(void) { */
/*   void *p; */
/*   size_t i; */

/*   p = arl_init(&l, l_values_size); */
/*   if (!p) */
/*     exit(1); */

/*   i = write_values_to_array(arl_small_half_size, l.array, l_values);
 */
/*   if (i != arl_small_half_size) */
/*     exit(2); */

/*   l.size = i; */
/* } */

/* void cleanup_arl_small(void) { free(l.array); } */
/* void cleanup_arl_small_fullfilled(void) { */
/*   size_t i; */
/*   for (i = l.size; i < l.size; i++) { */
/*     free(l.array[i]); */
/*   } */

/*   l.size = 0; */
/* } */
/* void cleanup_arl_small_halffilled(void) { */
/*   size_t i; */
/*   for (i = 0; i < arl_small_half_size; i++) { */
/*     free(l.array[i]); */
/*   } */

/*   l.size = 0; */
/* } */

/* /\* TEST SUITS *\/ */

/* TestSuite(arl_empty_array, .init = initiate_arl_small, .fini =
 * cleanup_arl_small); */
/* TestSuite(arl_full_array, .init = initiate_arl_small_fullfilled, */
/*           .fini = cleanup_arl_small_fullfilled); */
/* TestSuite(arl_half_array, .init = initiate_arl_small_halffilled, */
/*           .fini = cleanup_arl_small_halffilled); */

/* /\* EXTERN FUNCTIONS' TESTS *\/ */

/* /\* TO-DO *\/ */
/* /\* mock test_arl_init_failure, malloc *\/ */

/* Test(arl_full_array, test_arl_get_success) { */
/*   int *expected_values = l_values; */
/*   size_t i, values_size = l_values_size; */
/*   void *p; */

/*   cr_assert( */
/*       eq(ulong, (unsigned long int)l.capacity, (unsigned long
 * int)values_size)); */

/*   for (i = 0; i < values_size; i++) { */
/*     p = arl_get(&l, i); */
/*     cr_assert_not_null(p); */
/*     cr_assert(eq(int, *(int *)p, expected_values[i])); */
/*   } */
/* } */

/* Test(arl_half_array, test_arl_get_success) { */
/*   int *expected_values = l_values; */
/*   size_t i, values_size = arl_small_half_size; */
/*   void *p; */

/*   for (i = 0; i < values_size; i++) { */
/*     p = arl_get(&l, i); */
/*     cr_assert_not_null(p); */
/*     cr_assert(eq(int, *(int *)p, expected_values[i])); */
/*   } */

/*   for (; i < l.capacity; i++) { */
/*     p = arl_get(&l, i); */
/*     cr_assert_null(p); */
/*   } */
/* } */

/* /\* STATIC FUNCTIONS' TESTS *\/ */

/* Test(arl_full_array, test_arl_is_i_invalid_false) { */
/*   size_t j, i_to_check[] = {0, 2, 4}; */
/*   bool received, expected = false; */

/*   for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) { */
/*     received = arl_is_i_invalid(&l, i_to_check[j]); */

/*     cr_expect(received == expected, */
/*               "arl_is_i_invalid(&l, %lu) = %s" */
/*               "\n" */
/*               "l.array={%lu,%lu,%lu,%lu,%lu}" */
/*               "\n", */
/*               i_to_check[j], "true", *(int *)l.array[0], *(int *)l.array[1],
 */
/*               *(int *)l.array[2], *(int *)l.array[3], *(int *)l.array[4]); */
/*   } */
/* } */

/* Test(arl_full_array, test_arl_is_i_invalid_true) { */

/*   size_t j, i_to_check[] = {l_values_size + 1, ULONG_MAX}; */
/*   bool received, expected = true; */

/*   for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) { */
/*     received = arl_is_i_invalid(&l, i_to_check[j]); */

/*     cr_expect(received == expected, */
/*               "arl_is_i_invalid(&l, %lu) = %s" */
/*               "\n" */
/*               "l.array={%lu,%lu,%lu,%lu,%lu}" */
/*               "\n", */
/*               i_to_check[j], "false", *(int *)l.array[0], *(int *)l.array[1],
 */
/*               *(int *)l.array[2], *(int *)l.array[3], *(int *)l.array[4]); */
/*   } */
/* } */

/* Test(arl_full_array, */
/*      test_arl_move_indexes_by_positive_number_failure_capacity) { */
/*   // cannot set over size */
/*   void *received_value; */

/*   size_t start = 0, move_by = 1; */

/*   received_value = arl_move_indexes_by_positive_number(&l, start, move_by);
 */

/*   cr_expect_null(received_value); */
/* } */

/* Test(arl_half_array, */
/*      test_arl_move_indexes_by_positive_number_failure_elements_to_move) { */
/*   void *received_value; */

/*   size_t start = l.size + 1, move_by = 0; */

/*   received_value = arl_move_indexes_by_positive_number(&l, start, move_by);
 */

/*   cr_expect_null(received_value); */
/* } */

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_0) { */
/*   size_t null_indexes[] = {1}; */
/*   size_t not_null_indexes[] = {0, 2, 3}; */
/*   size_t i, start = 1, move_by = 1; */
/*   size_t expected_size = l.size + move_by; */
/*   void *p; */

/*   p = arl_move_indexes_by_positive_number(&l, start, move_by); */

/*   cr_assert_not_null(p); */

/*   for (i = 0; i < sizeof(null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, null_indexes[i]); */
/*     cr_assert_null(p); */
/*   } */

/*   for (i = 0; i < sizeof(not_null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, not_null_indexes[i]); */
/*     cr_assert_not_null(p); */
/*   } */

/*   cr_assert( */
/*       eq(ulong, (unsigned long int)l.size, (unsigned long
 * int)expected_size)); */
/* } */

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_1) { */
/*   size_t null_indexes[] = {2}; */
/*   size_t not_null_indexes[] = {0, 1, 3}; */
/*   size_t i, start = 2, move_by = 1; */
/*   size_t expected_size = l.size + move_by; */
/*   void *p; */

/*   p = arl_move_indexes_by_positive_number(&l, start, move_by); */

/*   cr_assert_not_null(p); */

/*   for (i = 0; i < sizeof(null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, null_indexes[i]); */
/*     cr_assert_null(p); */
/*   } */

/*   for (i = 0; i < sizeof(not_null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, not_null_indexes[i]); */
/*     cr_assert_not_null(p); */
/*   } */

/*   cr_assert( */
/*       eq(ulong, (unsigned long int)l.size, (unsigned long
 * int)expected_size)); */
/* } */

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_2) { */
/*   size_t null_indexes[] = {1, 2}; */
/*   size_t not_null_indexes[] = {0, 3, 4}; */
/*   size_t i, start = 1, move_by = 2; */
/*   size_t expected_size = l.size + move_by; */
/*   void *p; */

/*   p = arl_move_indexes_by_positive_number(&l, start, move_by); */

/*   cr_assert_not_null(p); */

/*   for (i = 0; i < sizeof(null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, null_indexes[i]); */
/*     cr_assert_null(p); */
/*   } */

/*   for (i = 0; i < sizeof(not_null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, not_null_indexes[i]); */
/*     cr_assert_not_null(p); */
/*   } */

/*   cr_assert( */
/*       eq(ulong, (unsigned long int)l.size, (unsigned long
 * int)expected_size)); */
/* } */

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_3) { */
/*   size_t null_indexes[] = {2}; */
/*   size_t not_null_indexes[] = {0, 1, 4}; */
/*   size_t i, start = 2, move_by = 2; */
/*   size_t expected_size = l.size + move_by; */
/*   void *p; */

/*   p = arl_move_indexes_by_positive_number(&l, start, move_by); */

/*   cr_assert_not_null(p); */

/*   for (i = 0; i < sizeof(null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, null_indexes[i]); */
/*     cr_assert_null(p); */
/*   } */

/*   for (i = 0; i < sizeof(not_null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, not_null_indexes[i]); */
/*     cr_assert_not_null(p); */
/*   } */

/*   cr_assert( */
/*       eq(ulong, (unsigned long int)l.size, (unsigned long
 * int)expected_size)); */
/* } */

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_4) { */
/*   size_t null_indexes[] = {1, 2, 3}; */
/*   size_t not_null_indexes[] = {0, 4, 5}; */
/*   size_t i, start = 1, move_by = 3; */
/*   size_t expected_size = l.size + move_by; */
/*   void *p; */

/*   p = arl_move_indexes_by_positive_number(&l, start, move_by); */

/*   cr_assert_not_null(p); */

/*   for (i = 0; i < sizeof(null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, null_indexes[i]); */
/*     cr_assert_null(p); */
/*   } */

/*   for (i = 0; i < sizeof(not_null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, not_null_indexes[i]); */
/*     cr_assert_not_null(p); */
/*   } */

/*   cr_assert( */
/*       eq(ulong, (unsigned long int)l.size, (unsigned long
 * int)expected_size)); */
/* } */

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_5) { */
/*   size_t null_indexes[] = {2, 3, 4}; */
/*   size_t not_null_indexes[] = {0, 1, 5}; */
/*   size_t i, start = 2, move_by = 3; */
/*   size_t expected_size = l.size + move_by; */
/*   void *p; */

/*   p = arl_move_indexes_by_positive_number(&l, start, move_by); */

/*   cr_assert_not_null(p); */

/*   for (i = 0; i < sizeof(null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, null_indexes[i]); */
/*     cr_assert_null(p); */
/*   } */

/*   for (i = 0; i < sizeof(not_null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, not_null_indexes[i]); */
/*     cr_assert_not_null(p); */
/*   } */

/*   cr_assert( */
/*       eq(ulong, (unsigned long int)l.size, (unsigned long
 * int)expected_size)); */
/* } */

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_6) { */
/*   size_t null_indexes[] = {0, 1, 2}; */
/*   size_t not_null_indexes[] = {3, 4, 5}; */
/*   size_t i, start = 0, move_by = 3; */
/*   size_t expected_size = l.size + move_by; */
/*   void *p; */

/*   p = arl_move_indexes_by_positive_number(&l, start, move_by); */
/*   cr_assert_not_null(p); */

/*   for (i = 0; i < sizeof(null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, null_indexes[i]); */
/*     cr_assert_null(p); */
/*   } */

/*   for (i = 0; i < sizeof(not_null_indexes) / sizeof(size_t); i++) { */
/*     p = arl_get(&l, not_null_indexes[i]); */
/*     cr_assert_not_null(p); */
/*   } */

/*   cr_assert( */
/*       eq(ulong, (unsigned long int)l.size, (unsigned long
 * int)expected_size)); */
/* } */

/* Test(arl_half_array, test_arl_grow_array_capacity_success) { */
/*   void *p, *src_arr; */
/*   size_t expected_capacity = 80; */

/*   src_arr = l.array; */

/*   p = arl_grow_array_capacity(&l); */
/*   cr_assert_not_null(p); */

/*   cr_assert(eq(ulong, (unsigned long int)l.capacity, */
/*                (unsigned long int)expected_capacity)); */

/*   cr_expect(src_arr != l.array, */
/*             "%p shoudl be different than %p, after realloc.", src_arr,
 * l.array); */
/* } */

int main(void) {

  const struct CMUnitTest internal_tests[] = {
      cmocka_unit_test(test_arl_count_new_capacity_base_doc),
      cmocka_unit_test_setup_teardown(test_arl_is_i_invalid_true,
                                      setup_arl_small_empty, teardown_arl),
      cmocka_unit_test_setup_teardown(test_arl_is_i_invalid_true,
                                      setup_arl_small_full, teardown_arl)

      /* teardown_arl_small_empty)}; */
  };
  const struct CMUnitTest public_tests[] = {
      cmocka_unit_test(test_arl_init_success),
      cmocka_unit_test_setup_teardown(test_arl_get_success,
                                      setup_arl_small_full, teardown_arl)

  };

  cmocka_run_group_tests(internal_tests, NULL, NULL);

  cmocka_run_group_tests(public_tests, NULL, NULL);

  return 0;
}
