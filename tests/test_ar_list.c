/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test framework dependencies  */
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
/* Replace malloc/realloc/free with cmocka alternatives. */
#define UNIT_TESTING 1
/* Test framework itself */
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
size_t free_values_from_array(size_t size, void *array[size]) {
  /* Array and values has to be at least the same size. */
  /* Return number of elements written to array. */

  size_t i;

  for (i = 0; i < size; i++) {
    free(array[i]);
  }
  return i;
}

void print_array_pointers(size_t size, void *array[size]) {
  size_t i;
  puts("{");
  for (i = 0; i < size; i++) {
    printf("%p,\n", array[i]);
  }
  puts("}");
}

void print_array_values(size_t size, void *array[size]) {
  size_t i;
  puts("{");
  for (i = 0; i < size; i++) {
    printf("%i,\n", *(int *)array[i]);
  }
  puts("}");
}

void print_size_and_capacity(size_t size, size_t capacity) {
  printf("Size: %lu, Capacity: %lu\n", size, capacity);
}

/*******************************************************************************
 *    MOCKS
 ******************************************************************************/

void *__real__test_malloc(const size_t size, const char *file, const int line);

void *__wrap__test_malloc(size_t size);
void *__wrap__test_malloc(size_t size) {

  bool will_return_ = mock();

  if (will_return_)
    return __real__test_malloc(size, __FILE__, __LINE__);

  return NULL;
}

/* void *__wrap__test_malloc(size_t size) { return 1; } */

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

static int initiate_l(size_t size, size_t capacity, void *values) {
  void *p;
  size_t i;

  p = arl_init(&l, capacity);
  if (!p)
    return 1;

  i = write_values_to_array(size, l.array, values);
  if (i != size)
    return 2;

  l.size = size;

  return 0;
}

static void cleanup_l(void) {
  free_values_from_array(l.size, l.array);

  /* free(l.array); */

  l.size = 0;
}
static void cleanup_l_alloc(void) {

  free(l.array);

  l.size = 0;
}

/*******************************************************************************
 *    SETUP, TEARDOWN
 ******************************************************************************/

static int setup_arl_small_full(void **state) {
  will_return_always(__wrap__test_malloc, true);

  l_values_size = sizeof(arl_small_values) / sizeof(int);

  l_values = arl_small_values;

  initiate_l(l_values_size, l_values_size, l_values);

  return 0;
}

static int setup_arl_small_empty(void **state) {
  will_return_maybe(__wrap__test_malloc, true);

  l_values_size = sizeof(arl_small_values) / sizeof(int);

  l_values = arl_empty_values;

  initiate_l(0, l_values_size, l_values);

  return 0;
}

static int setup_arl_small_half(void **state) {
  will_return_always(__wrap__test_malloc, true);

  l_values_size = (sizeof(arl_small_values) / sizeof(int)) / 2;

  l_values = arl_small_values;

  initiate_l(l_values_size, l_values_size, l_values);

  return 0;
}

static int teardown_arl(void **state) {
  cleanup_l();

  return 0;
}

static int teardown_arl_empty(void **state) {
  cleanup_l();

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
 *    PRIVATE API TESTS
 ******************************************************************************/

void test_arl_alloc_array_failue(void **state) {
  /* Purpose of this function is mainly documentational. */
  void *p;

  will_return(__wrap__test_malloc, false);

  p = arl_alloc_array(&l, 5);

  assert_null(p);
}

void test_arl_count_new_capacity_base(void **state) {
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
      print_array_pointers(l.size, l.array);
      fail_msg("arl_is_i_invalid(&l, %zu) = %s\n", i_to_check[j], "false");
    }
  }
}

void test_arl_is_i_invalid_false(void **state) {
  size_t j, i_to_check[] = {0, l_values_size / 2, l_values_size - 1};
  bool is_invalid;

  for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) {
    is_invalid = arl_is_i_invalid(&l, i_to_check[j]);

    if (is_invalid) {
      print_array_pointers(l.size, l.array);
      fail_msg("arl_is_i_invalid(&l, %zu) = %s\n", i_to_check[j], "true");
    }
  }
}

void test_arl_grow_array_capacity_success(void **state) {
  size_t capacity_cp;
  void *received;

  capacity_cp = l.capacity;

  print_size_and_capacity(l.size, l.capacity);
  print_array_pointers(l.size, l.array);

  received = arl_grow_array_capacity(&l);

  assert_non_null(received);

  print_size_and_capacity(l.size, l.capacity);
  print_array_pointers(l.size, l.array);

  if (capacity_cp >= l.capacity) {
    print_size_and_capacity(l.size, l.capacity);
    print_array_pointers(l.size, l.array);
    fail_msg("%li >= %li is True", capacity_cp, l.capacity);
  }
}

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
/*      test_arl_move_indexes_by_positive_number_failure_elements_to_move) {
 */
/*   void *received_value; */

/*   size_t start = l.size + 1, move_by = 0; */

/*   received_value = arl_move_indexes_by_positive_number(&l, start, move_by);
 */

/*   cr_expect_null(received_value); */
/* } */

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_0) {
 */
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

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_1) {
 */
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

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_2) {
 */
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

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_3) {
 */
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

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_4) {
 */
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

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_5) {
 */
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

/* Test(arl_half_array, test_arl_move_indexes_by_positive_number_success_6) {
 */
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

  /* Alias functions so tests reports are easier to read. */
#define test_arl_is_i_invalid_true_empty test_arl_is_i_invalid_true
#define test_arl_is_i_invalid_true_full test_arl_is_i_invalid_true
#define test_arl_is_i_invalid_true_half test_arl_is_i_invalid_true
#define test_arl_is_i_invalid_false_full test_arl_is_i_invalid_false
#define test_arl_is_i_invalid_false_half test_arl_is_i_invalid_false
#define test_arl_grow_array_capacity_success_empty arl_grow_array_capacity
#define test_arl_grow_array_capacity_success_full arl_grow_array_capacity
#define test_arl_grow_array_capacity_success_half arl_grow_array_capacity

  const struct CMUnitTest private_tests[] = {
      cmocka_unit_test(test_arl_alloc_array_failue),
      cmocka_unit_test(test_arl_count_new_capacity_base),
      cmocka_unit_test_setup_teardown(test_arl_is_i_invalid_true_empty,
                                      setup_arl_small_empty, teardown_arl),
      cmocka_unit_test_setup_teardown(test_arl_is_i_invalid_true_full,
                                      setup_arl_small_full, teardown_arl),
      cmocka_unit_test_setup_teardown(test_arl_is_i_invalid_true_half,
                                      setup_arl_small_half, teardown_arl),
      cmocka_unit_test_setup_teardown(test_arl_is_i_invalid_false_full,
                                      setup_arl_small_full, teardown_arl),
      cmocka_unit_test_setup_teardown(test_arl_is_i_invalid_false_half,
                                      setup_arl_small_half, teardown_arl),
      cmocka_unit_test_setup_teardown(
          test_arl_grow_array_capacity_success_empty, setup_arl_small_empty,
          teardown_arl_empty),

  };

  const struct CMUnitTest public_tests[] = {
      cmocka_unit_test(test_arl_init_success),
      cmocka_unit_test_setup_teardown(test_arl_get_success,
                                      setup_arl_small_full, teardown_arl)

  };

  cmocka_run_group_tests(private_tests, NULL, NULL);

  cmocka_run_group_tests(public_tests, NULL, NULL);

  return 0;
}
