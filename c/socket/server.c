/*
 * Demo of socket server.
 * Build: gcc server.c -o server
 * Run: ./server
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int listenfd = 0, connfd = 0;
  struct sockaddr_in serv_addr;

  char sendBuff[1025];

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000);

  bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  printf("Bind success\n");
  listen(listenfd, 3);
  printf("listening port 5000...\n");
  while (1) {
    connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
    printf("Accepted...\n");
    int record_count = 10;
    int count = record_count;
    strcpy(sendBuff, "1,luo\n");
    while (count--) {
      write(connfd, sendBuff, strlen(sendBuff));
    }
    close(connfd);
    printf("Send count: %d\n", record_count);
    break;
  }
  return 0;
}
