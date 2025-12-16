#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char* argv[]) {

  if (argc < 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 0;
  }

  int serverfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (serverfd < 0) {
    perror("socket");
    return 1;
  }


  struct sockaddr_in addr;

  if (!inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)) {
    perror("inet_pton");
    return 2;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[1]));

  if (bind(serverfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("bind");
    return 3;
  }

  char buff[4096];
  while (1) {
    struct sockaddr client_address;
    socklen_t cl_addrlen;

    ssize_t received = recvfrom(serverfd, buff, 4096, 0, &client_address, &cl_addrlen);

    printf("server received packet\n");

    sendto(serverfd, buff, received, 0, &client_address, cl_addrlen);

    printf("server sent packet\n");
  }

  close(serverfd);

}
