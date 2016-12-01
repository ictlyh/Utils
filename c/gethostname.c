#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int z;
  char buf[32];
  z = gethostname(buf, sizeof(buf));
  if (z == -1) {
    fprintf(stderr, "%s: gethostname(2)\n", strerror(errno));
    return 1;
  }
  printf("host name = '%s'\n", buf);
  z = getdomainname(buf, sizeof buf);
  if (z == -1) {
    fprintf(stderr, "%s: getdomainname(2)\n", strerror(errno));
    return 1;
  }
  printf("domain name = '%s'\n", buf);
  return 0;
}
