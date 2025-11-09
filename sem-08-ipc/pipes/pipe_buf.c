#define _GNU_SOURCE

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

  printf("PIPE_BUF = %d\n", PIPE_BUF);

  int pfd[2];
  pipe(pfd);

  int pipe_capacity = fcntl(pfd[0], F_GETPIPE_SZ);

  printf("Pipe capacity = %d\n", pipe_capacity);

}
