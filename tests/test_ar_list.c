/* /\* Test(arl_extern, test_arl_get_failure, .init = initiate_arl_1000, *\/ */
/* /\*      .fini = cleanup_arl_1000) { *\/ */
/* /\*   void *value; *\/ */

/* /\*   value = arl_get(&l, 5); *\/ */
/* /\*   cr_expect_null(value); *\/ */
/* /\* } *\/ */

/* /\* Test(arl_extern, test_arl_set_success, .init = initiate_arl_1000, *\/ */
/* /\*      .fini = cleanup_arl_1000) { *\/ */
/* /\*   int i; *\/ */

/* /\*   for (i = 0; (size_t)i < l.size / 2; i++) { *\/ */
/* /\*     cr_assert(eq(int, *(int *)(l.array[i]), i)); *\/ */
/* /\*   } *\/ */
/* /\* } *\/ */

/* /\* Test(arl_extern, test_arl_set_failure, .init = initiate_arl_1000, *\/ */
/* /\*      .fini = cleanup_arl_1000) { *\/ */
/* /\*   void *value; *\/ */
/* /\*   int i = 3; *\/ */

/* /\*   value = arl_set(&l, 5, &i); *\/ */
/* /\*   cr_assert_null(value); *\/ */
/* /\* } *\/ */

/* /\* Test(arl_static, test_arl_count_new_capacity_base_doc) { *\/ */
/* /\*   /\\* Purpose of this function is mainly documentational. *\\/ *\/ */
/* /\*   /\\* Show array growth ratio by example. *\\/ *\/ */
/* /\*   size_t size = 0; *\/ */
/* /\*   size_t capacity = 1000; *\/ */

/* /\*   capacity = arl_count_new_capacity_base(size, capacity); *\/ */
/* /\*   size = capacity; *\/ */

/* /\*   cr_assert(eq(ulong, (unsigned long int)capacity, (unsigned long */
/*  * int)1000)); */
/*  *\/ */

/* /\*   capacity = arl_count_new_capacity_base(size, capacity); *\/ */
/* /\*   size = capacity; *\/ */

/* /\*   cr_assert(eq(ulong, (unsigned long int)capacity, (unsigned long */
/*  * int)2500)); */
/*  *\/ */

/* /\*   capacity = arl_count_new_capacity_base(size, capacity); *\/ */
/* /\*   size = capacity; *\/ */

/* /\*   cr_assert(eq(ulong, (unsigned long int)capacity, (unsigned long */
/*  * int)6250)); */
/*  *\/ */

/* /\*   capacity = arl_count_new_capacity_base(size, capacity); *\/ */
/* /\*   size = capacity; *\/ */

/* /\*   cr_assert(eq(ulong, (unsigned long int)capacity, (unsigned long */
/*  * int)15625)); *\/ */
/* /\* } *\/ */

/* /\* /\\* TO DO *\\/ *\/ */
/* /\* /\\* Corner cases : *\\/ *\/ */
/* /\* /\\* - list fully filled *\\/ *\/ */
/* /\* /\\* - list half filled *\\/ *\/ */
/* /\* /\\* - empty list *\\/ *\/ */
/* /\* Test(arl_static, test_arl_move_indexes_by_positive_number_success, *\/ */
/* /\*      .init = initiate_arl_1000_filled, .fini = cleanup_arl_1000_filled) {
 */
/*  *\/ */
/* /\*   puts("TEST START!"); *\/ */

/* /\*   void *value; *\/ */
/* /\*   int i, received_i; *\/ */
/* /\*   size_t size_before = l.size, move_by = 3, start_i = 33; *\/ */

/* /\*   value = arl_move_indexes_by_positive_number(&l, start_i, move_by); *\/
 */
/* /\*   cr_assert_not_null(value); *\/ */

/* /\*   cr_assert(eq(int, l.size, size_before + move_by)); *\/ */

/* /\*   for (i = 0; (size_t)i < l.size; i++) { *\/ */
/* /\*     cr_assert_not_null(l.array[i]); *\/ */
/* /\*     received_i = *(int *)l.array[i]; *\/ */

/* /\*     if (i <= start_i) { *\/ */
/* /\*       cr_assert(eq(int, received_i, i)); *\/ */
/* /\*     } else { *\/ */
/* /\*       cr_assert(eq(int, received_i, i - move_by)); *\/ */
/* /\*     } *\/ */
/* /\*   } *\/ */
/* /\* } *\/ */
#include <limits.h>
#include <stdbool.h>
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
// TO-DO
// parametrized expected values
// count dynamically expected values sign

/* UTILS */
size_t write_values_to_array(size_t size, void *array[], int values[]) {
  /* Array and values has to be the same size at least. */
  /* Return number of elements written to array. */
  void *p;
  size_t i = 0;
  for (i = 0; i < size; i++) {
    p = malloc(sizeof(int));
    if (!p)
      continue;

    *(int *)p = values[i];
    l.array[i] = p;
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

/* FIXTURES */
int arl_5_values[] = {0, 1, 2, 3, 4, 5};
size_t arl_5_size = sizeof(arl_5_values) / sizeof(int);
size_t arl_5_half_size = 3;

void initiate_arl_5(void) { arl_init(&l, arl_5_size); }
void initiate_arl_5_fullfilled(void) {
  void *p;
  size_t i;

  p = arl_init(&l, arl_5_size);
  if (!p)
    exit(1);

  i = write_values_to_array(arl_5_size, l.array, arl_5_values);
  if (i != arl_5_size)
    exit(2);

  l.size = i;
}
void initiate_arl_5_halffilled(void) {
  void *p;
  size_t i;

  p = arl_init(&l, arl_5_size);
  if (!p)
    exit(1);

  i = write_values_to_array(arl_5_half_size, l.array, arl_5_values);
  if (i != arl_5_half_size)
    exit(2);

  l.size = i;
}

void cleanup_arl_5(void) { free(l.array); }
void cleanup_arl_5_fullfilled(void) {
  size_t i;
  for (i = 0; i < l.size; i++) {
    free(l.array[i]);
  }
  free(l.array);

  l.size = 0;
}
void cleanup_arl_5_halffilled(void) {
  size_t i;
  for (i = 0; i < arl_5_half_size; i++) {
    free(l.array[i]);
  }

  l.size = 0;
}

/* TEST SUITS */

TestSuite(arl_empty_array, .init = initiate_arl_5, .fini = cleanup_arl_5);
TestSuite(arl_full_array, .init = initiate_arl_5_fullfilled,
          .fini = cleanup_arl_5_fullfilled);
TestSuite(arl_half_array, .init = initiate_arl_5_halffilled,
          .fini = cleanup_arl_5_halffilled);

/* EXTERN FUNCTIONS' TESTS */

Test(arl_extern, test_arl_init_success) {
  ar_list l;
  size_t default_capacity = 255;

  arl_init(&l, default_capacity);

  cr_expect_not_null(l.array);
  cr_assert(eq(ulong, (unsigned long int)l.size, (unsigned long int)0));
  cr_assert(eq(ulong, (unsigned long int)l.capacity, (unsigned long int)255));
}

/* TO-DO */
/* mock test_arl_init_failure, malloc */

Test(arl_full_array, test_arl_get_success) {
  int *expected_values = arl_5_values;
  size_t i, values_size = arl_5_size;
  void *p;

  cr_assert(
      eq(ulong, (unsigned long int)l.capacity, (unsigned long int)values_size));

  for (i = 0; i < values_size; i++) {
    p = arl_get(&l, i);
    cr_assert_not_null(p);
    cr_assert(eq(int, *(int *)p, expected_values[i]));
  }
}

Test(arl_half_array, test_arl_get_success) {
  int *expected_values = arl_5_values;
  size_t i, values_size = arl_5_half_size;
  void *p;

  for (i = 0; i < values_size; i++) {
    p = arl_get(&l, i);
    cr_assert_not_null(p);
    cr_assert(eq(int, *(int *)p, expected_values[i]));
  }

  for (; i < l.capacity; i++) {
    p = arl_get(&l, i);
    cr_assert_null(p);
  }
}

/* STATIC FUNCTIONS' TESTS */

Test(arl_static, test_arl_count_new_capacity_base_doc) {
  /* Purpose of this function is mainly documentational. */
  /* Show array capacity growth ratio by example. */

  size_t expected_values[] = {1000, 2500, 6250, 15625};
  size_t size = 0;
  size_t capacity = 1000;
  size_t i;

  for (i = 0; i < (sizeof(expected_values) / sizeof(size_t)); i++) {
    capacity = size = arl_count_new_capacity_base(size, capacity);

    cr_assert(eq(ulong, (unsigned long int)capacity,
                 (unsigned long int)expected_values[i]));
  }
}

Test(arl_empty_array, test_arl_is_i_invalid_true) {
  /* Empty array is always invalid. */

  size_t j, i_to_check[] = {0, 2, 4, ULONG_MAX};
  bool received, expected = true;

  for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) {
    received = arl_is_i_invalid(&l, i_to_check[j]);

    cr_expect(received == expected,
              "arl_is_i_invalid(&l, %i) = %s\nl.array={,,,,}\n", i_to_check[j],
              "false");
  }
}

Test(arl_full_array, test_arl_is_i_invalid_false) {
  size_t j, i_to_check[] = {0, 2, 4};
  bool received, expected = false;

  for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) {
    received = arl_is_i_invalid(&l, i_to_check[j]);

    cr_expect(received == expected,
              "arl_is_i_invalid(&l, %lu) = %s"
              "\n"
              "l.array={%lu,%lu,%lu,%lu,%lu}"
              "\n",
              i_to_check[j], "true", *(int *)l.array[0], *(int *)l.array[1],
              *(int *)l.array[2], *(int *)l.array[3], *(int *)l.array[4]);
  }
}

Test(arl_full_array, test_arl_is_i_invalid_true) {

  size_t j, i_to_check[] = {arl_5_size + 1, ULONG_MAX};
  bool received, expected = true;

  for (j = 0; j < (sizeof(i_to_check) / sizeof(size_t)); j++) {
    received = arl_is_i_invalid(&l, i_to_check[j]);

    cr_expect(received == expected,
              "arl_is_i_invalid(&l, %lu) = %s"
              "\n"
              "l.array={%lu,%lu,%lu,%lu,%lu}"
              "\n",
              i_to_check[j], "false", *(int *)l.array[0], *(int *)l.array[1],
              *(int *)l.array[2], *(int *)l.array[3], *(int *)l.array[4]);
  }
}

/* Test(arl_empty_array, test_arl_move_indexes_by_positive_number_failure) { */
/*   // cannot get not initialized value */
/*   void *received_value; */

/*   size_t start = 1, move_by = 2; */

/*   received_value = arl_move_indexes_by_positive_number(&l, start, move_by);
 */

/*   cr_expect_null(received_value); */
/* } */

Test(arl_full_array, test_arl_move_indexes_by_positive_number_failure) {
  // cannot set over size
  void *received_value;

  size_t start = 1, move_by = 2;

  received_value = arl_move_indexes_by_positive_number(&l, start, move_by);

  cr_expect_null(received_value);
}

Test(arl_half_array, test_arl_move_indexes_by_positive_number_success) {
  int expected_values[] = {0, 1, 1};
  size_t i, start = 1, move_by = 2;
  void *p;

  print_array(arl_5_size, l.array);
  print_size_and_capacity(l.size, l.capacity);

  p = arl_move_indexes_by_positive_number(&l, start, move_by);

  print_array(arl_5_size, l.array);
  print_size_and_capacity(l.size, l.capacity);

  cr_assert_not_null(p);

  for (i = 0; i < sizeof(expected_values) / sizeof(int); i++) {
    p = arl_get(&l, i);
    cr_assert_not_null(p);
    cr_assert(eq(int, *(int *)p, expected_values[i]));
  }
}
