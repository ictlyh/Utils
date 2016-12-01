#include <stdio.h>

#define DEBUG_FORMAT "%s:%d %s\n"
#define DEBUG_VALUE __FILE__, __LINE__, __FUNCTION__

int main() {
  printf(DEBUG_FORMAT, DEBUG_VALUE);
  return 0;
}
