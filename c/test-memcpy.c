/*
 * Test memcpy then source size overflow dest buffer
 * Build: gcc test-memcpy.c -o test-memcpy.out
 * Run: ./test-memcpy.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *dest = (char *)malloc(3);
  memcpy(dest, "hi", 2);
  fprintf(stdout, "size=%lu,strlen=%lu, str:%s\n", sizeof(dest), strlen(dest),
          dest);
  memcpy(dest, "hello,nice", 10);
  fprintf(stdout, "size=%lu,strlen=%lu, str:%s\n", sizeof(dest), strlen(dest),
          dest);
  return 0;
}
