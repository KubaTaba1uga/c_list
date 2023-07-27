/* Test private functions in src/list.c */

#include <criterion/criterion.h>

#include <stdio.h>

#include "../src/list.h"

Test(count_new_size_list, 0and1000) {
  /* Purpose of this function is mainly documentational. */
  int len = 0;
  size_t size = 1000;

  size = l_count_new_size(len, size);
  len = size;
  printf("%i\n", size);
  size = l_count_new_size(len, size);
  len = size;
  printf("%i\n", size);
  size = l_count_new_size(len, size);
  len = size;
  printf("%i\n", size);
}
