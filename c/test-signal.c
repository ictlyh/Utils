#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sig_handler(int signo) {
  if (signo == SIGUSR1)
    printf("received SIGUSR1\n");
  else if (signo == SIGINT) {
    printf("received SIGINT\n");
    exit(0);
  } else if (signo == SIGKILL)
    printf("received SIGKILL\n");
  else if (signo == SIGSTOP)
    printf("received SIGSTOP\n");
}

int main(void) {
  if (signal(SIGUSR1, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGUSR1\n");
  if (signal(SIGINT, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGINT\n");
  // The signals SIGKILL and SIGSTOP cannot be caught or ignored
  if (signal(SIGKILL, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGKILL\n");
  if (signal(SIGSTOP, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGSTOP\n");
  // A long long wait so that we can easily issue a signal to this process
  while (1)
    sleep(1);
  return 0;
}
