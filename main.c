#include "alloc.c"
#include <assert.h>
#include <stdio.h>

int *get_me_mem() { return (int *)my_malloc(sizeof(int)); }

int main() {
  long long *something = (long long *)my_malloc(sizeof(long long));
  int *something2 = get_me_mem();
  // int *something = malloc(4);
  // int *something2 = malloc(4);
  *(something) = 0xffffffffffffffff;
  *(something2) = 20;
  printf("%lld\n", *something);
  printf("%d\n", *something2);
  long long *x = something;
  my_free(something);
  my_free(something2);
  assert(x == something);
  int *something3 = (int *)my_malloc(4);
  printf("%d\n", something3[1]);
  /* my_free(something3); */
  /* int *something4 = (int *)my_malloc(4); */
  char *str = (char *)my_malloc(10);
  str[0] = 'a';
  /* printf("%d\n", *something4); */
  printf("%s\n", str);
  my_free(str);
  str = (char *)my_malloc(100000000);
  my_free(str);
}
