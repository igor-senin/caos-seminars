#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/wait.h>


#define BACKLOG     20
#define BUFF_SIZE   4096
#define CHLD_CNT    8
#define MAX_EVENTS  64

pid_t chld_pids[CHLD_CNT];

/* Обрабатывает новый сегмент данных от клиента */
static
void handle_request(int cfd) {
  char buff[BUFF_SIZE];
  ssize_t num_read;

  if ((num_read = read(cfd, buff, BUFF_SIZE)) > 0) {
    if (write(cfd, buff, num_read) != num_read) {
      printf("error while writing\n");
    }
  }

  if (num_read < 0) {
    printf("error while reading\n");
  }

  if (num_read == 0) {
    shutdown(cfd, SHUT_RDWR);
    close(cfd);
  }
}

/* Обработка одного epoll-события */
static
void process_epoll_event(struct epoll_event event, int serv_fd, int epoll_fd) {
  if (event.data.fd != serv_fd) {
    handle_request(event.data.fd);
    return;
  }

  struct sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);

  int client_fd = accept(serv_fd, (struct sockaddr *) &client_addr, &client_len);
  if (client_fd < 0) {
    perror("accept");
    return;
  }

  memset(&event, 0, sizeof(event));
  event.events = EPOLLIN;
  event.data.fd = client_fd;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) != 0) {
    perror("epoll_ctl");
  }
}

/* Event-loop */
static
void worker_routine(int serv_fd) {
  int epoll_fd = epoll_create1(0);
  if (epoll_fd < 0) {
    perror("epoll_create");
    exit(EXIT_FAILURE);
  }

  struct epoll_event event;
  struct epoll_event events[MAX_EVENTS];

  event.events = EPOLLIN | EPOLLEXCLUSIVE;
  event.data.fd = serv_fd;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_fd, &event) != 0) {
    perror("epoll_ctl");
    exit(EXIT_FAILURE);
  }

  for (;;) {
    int ready = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (ready < 0) {
      if (errno == EINTR) {
        continue;
      }
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ready; ++i) {
      process_epoll_event(events[i], serv_fd, epoll_fd);
    }
  }

  close(serv_fd);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    dprintf(STDERR_FILENO, "Usage: %s <port-num>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (serv_fd < 0) {
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
  if (bind(serv_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(serv_fd, BACKLOG) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < CHLD_CNT; ++i) {
    pid_t pid = fork();
    switch (pid) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);

    case 0:
      worker_routine(serv_fd);
      exit(EXIT_SUCCESS);

    default:
      chld_pids[i] = pid;
      break;
    }
  }

  for (size_t i = 0; i < CHLD_CNT; ++i) {
    waitpid(chld_pids[i], NULL, 0);
  }

  close(serv_fd);
  exit(EXIT_SUCCESS);
}
