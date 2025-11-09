#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <util/meta.h>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    dprintf(STDERR_FILENO, "Usage: %s command [args...] \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int fd;
  SAFE_CALL(open, fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644));

  SAFE_CALL(dup2, dup2(fd, 1));

  close(fd); // now we can close original descriptor

  execvp(argv[1], argv + 1);
}
