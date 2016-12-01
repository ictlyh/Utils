/*
 * Demo of socket client.
 * Build: gcc client.c -o client
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
  int sockfd = 0;
  char recvBuff[1024];
  struct sockaddr_in serv_addr;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <ip of server> <port of server>\n", argv[0]);
    return 1;
  }

  memset(recvBuff, '0', sizeof(recvBuff));
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    fprintf(stderr, "Error : Could not create socket \n");
    return 1;
  }

  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
    fprintf(stderr, "inet_pton error occured\n");
    return 1;
  }

  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    fprintf(stderr, "Error : Connect Failed \n");
    return 1;
  }
  while (1) {
    int recvsize = recv(sockfd, recvBuff, 1024, 0);
    if (recvsize == 0)
      break;
    else if (recvsize == -1) {
      fprintf(stderr, "Read error\n");
      break;
    }
    recvBuff[recvsize] = '\0';
    fprintf(stdout, "%s", recvBuff);
  }
  return 0;
}
