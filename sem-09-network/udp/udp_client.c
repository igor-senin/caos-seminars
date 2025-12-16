#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char* argv[]) {

  if (argc < 3) {
    printf("Usage: %s <port> <message>\n", argv[0]);
    return 0;
  }

  int clientfd = socket(AF_INET/*AF_UNIX*/, SOCK_DGRAM/*=UDP socket*/, 0);
  // SOCK_STREAM = TCP socket
  if (clientfd < 0) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in addr;

    // sockaddr
    // |  |  |
    // in un in6

  if (!inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)) {
    // presentation to network
    perror("inet_pton");
    return 2;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[1]));
  // htons = host to network short
  // ntohs = network to host short

  sendto(clientfd, argv[2], strlen(argv[2]), 0, (struct sockaddr*)&addr, sizeof(addr));
  // sendto === write с адресом

  char buff[4096];

  read(clientfd, buff, 4096);
  // recvfrom === read с адресом

  printf("%s\n", buff);

  close(clientfd);
}
