#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int pipefd[2];

  if (pipe(pipefd) != 0) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  int flags = fcntl(pipefd[1], F_GETFL);
  flags |= O_NONBLOCK;
  fcntl(pipefd[1], F_SETFL, flags);

  char string[] = "Hello, world!\n";
  ssize_t total = write(pipefd[1], string, sizeof(string));
  // PIPE_BUF
  // ssize_t total = 0;
  // for (size_t i = 0; i < 100000; ++i) {
  //   ssize_t bytes_written = write(pipefd[1], string, sizeof(string));
  //   
  //   if (bytes_written < 0) {
  //     printf("would block; %ld\n", i);
  //     break;
  //   }
  //
  //   total += bytes_written;
  // }

  printf("bytes written: %ld\n", total);

  char buffer[4096];
  ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));

  printf("bytes read: %ld\nstring = %s", bytes_read, buffer);

  exit(EXIT_SUCCESS);

}
