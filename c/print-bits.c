/*
 * Demo of print bytes in binary.
 * Build: gcc print-bits.c -o print-bits
 * Run: ./print-bits
 */

#include <arpa/inet.h>
#include <stdio.h>

uint16_t htons(uint16_t hostshort);

void printBits(void const * const ptr, size_t size) {
  unsigned char *b = (unsigned char*) ptr;
  size_t i;
  printf("%lu bytes:", size);
  for (i = 0; i < size; i++) {
    printf("%3X", b[i]);
    if((i + 1) % 16 == 0)
      printf("\n");
  }
  printf("\n");
}

int main() {
  char c = '1';
  printBits((void*)&c, 1);
  short s = 171;
  printBits((void*)&s, 2);
  short bs = htons(s);
  printBits((void*)&bs, 2);
  return 0;
}
