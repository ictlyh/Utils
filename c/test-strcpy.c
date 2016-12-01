/*
 * Demo of strcpy and strncpy
 * Build: gcc test-strcpy.c -o test-strcpy.out
 * Run: ./test-strcpy.out
 */

#include <string.h>

int main() {
  char name[32];
  // strcpy(name, NULL); // Segment fault
  // strncpy(name, NULL, 0); // Segment fault
  // int len = strlen(NULL); // Segment fault
  memcpy(name, NULL, 0); // Warning
  return 0;
}
