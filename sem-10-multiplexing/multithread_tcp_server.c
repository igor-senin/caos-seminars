#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BACKLOG   20
#define BUFF_SIZE 4096
#define CHLD_CNT  8

pthread_t chld_threads[CHLD_CNT];

static
void handle_request(int cfd) {
  char buff[BUFF_SIZE];
  ssize_t num_read;

  while ((num_read = read(cfd, buff, BUFF_SIZE)) > 0) {
    if (write(cfd, buff, num_read) != num_read) {
      printf("error while writing\n");
      break;
    }
  }

  if (num_read < 0) {
    printf("error while reading\n");
  }

  shutdown(cfd, SHUT_RDWR);
  close(cfd);
}

static
void *worker_routine(void *arg) {
  int servfd = *(int *)arg;
  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  for (;;) {
    int client_fd = accept(servfd, (struct sockaddr *) &client_addr, &client_len);
    if (client_fd < 0) {
      printf("error in accept...\n");
      continue;
    }

    handle_request(client_fd);
  }

  shutdown(servfd, SHUT_RDWR);
  close(servfd);

  return NULL;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    dprintf(STDERR_FILENO, "Usage: %s <port-num>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int servfd = socket(AF_INET, SOCK_STREAM, 0);
  if (servfd < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in addr;

  if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) != 1) {
    perror("inet_pton");
    exit(EXIT_FAILURE);
  }

  addr.sin_port = htons(atoi(argv[1]));
  addr.sin_family = AF_INET;

  if (bind(servfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(servfd, BACKLOG) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < CHLD_CNT; ++i) {
    if (pthread_create(&chld_threads[i], NULL, worker_routine, &servfd) != 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  for (size_t i = 0; i < CHLD_CNT; ++i) {
    pthread_join(chld_threads[i], NULL);
  }

  close(servfd);
  exit(EXIT_SUCCESS);
}
