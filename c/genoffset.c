#include <stdio.h>
#include <stdlib.h>

int main() {
  int num;
  int *offset;
  printf("Input number:");
  scanf("%d", &num);
  offset = (int *)malloc(sizeof(int) * (num + 1));
  offset[0] = 0;
  printf("\nInput sizes:\n");
  int i;
  for (i = 1; i <= num; i++) {
    scanf("%d", &offset[i]);
  }
  for (i = 1; i <= num; i++)
    offset[i] = offset[i] + offset[i - 1];
  for (i = 0; i <= num; i++) {
    printf("%4d", offset[i]);
    if ((i + 1) % 10 == 0)
      printf("\n");
  }
  printf("\n");
  return 0;
}
