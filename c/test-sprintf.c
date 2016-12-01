#include <stdio.h>

int main() {
  char str[30];
  sprintf(str, "any extra value", "xxxxx");
  fprintf(stdout, "%s\n", str, "yyyyy");
  return 0;
}
