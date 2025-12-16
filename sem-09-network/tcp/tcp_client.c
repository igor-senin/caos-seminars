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

  int clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if (clientfd < 0) {
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

  if (connect(clientfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  char buff[4096];
  for (size_t i = 2; i < argc; ++i) {
    ssize_t bytes_written = write(clientfd, argv[i], strlen(argv[i]));

    read(clientfd, buff, sizeof(buff));

    printf("%s\n", buff);
  }

  shutdown(clientfd, SHUT_RDWR);
  close(clientfd);

}
