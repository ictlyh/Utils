/*
 * Demo to test endian of system
 * Build: gcc test-endian.c -o test-endian
 * Run: ./test-endian
 */

#include <stdio.h>

int main() {
  union w {
    short a;
    char b;
  } c;
  c.a = 1;
  printf("%s\n", c.b == 1 ? "Little endian" : "Big endian");
  return 0;
}
