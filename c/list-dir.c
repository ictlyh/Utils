/*
 * List directory iteratively on Linux system.
 * Build: gcc list-dir.c -o list-dir.out
 * Run: ./list-dir.out
 */

#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>

int main(void) {
  DIR *dp;
  struct dirent *ep;
  dp = opendir("./");
  if (dp != NULL) {
    while ((ep = readdir(dp))) {
      fprintf(stdout, "%d %s\n", ep->d_type, ep->d_name);
    }
    (void)closedir(dp);
  } else
    fprintf(stderr, "Couldn't open the directory");
  return 0;
}
