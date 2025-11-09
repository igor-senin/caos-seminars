#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define BUF_SIZE 10

void error_exit(const char* reason) {
  perror(reason);
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    dprintf(STDERR_FILENO, "Usage: %s <string>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int pfd[2];
  char buf[BUF_SIZE];
  ssize_t bytes_read;

  if (pipe(pfd) == -1) {
    error_exit("pipe");
  }

  switch (fork()) {
    case -1:
      error_exit("fork");

    case 0:
      /* ребёнок */
      close(pfd[1]);

      while (1) {
        bytes_read = read(pfd[0], buf, BUF_SIZE);
        if (bytes_read < 0) {
          error_exit("read in child");
        }
        if (bytes_read == 0) {
          break;
        }

        write(STDOUT_FILENO, buf, bytes_read);
      }

      write(STDOUT_FILENO, "\n", 1);

      close(pfd[0]);

      exit(EXIT_SUCCESS);
    default:
      /* родитель */

      close(pfd[0]);
      
      write(pfd[1], argv[1], strlen(argv[1]));

      close(pfd[1]);

      wait(NULL);
      exit(EXIT_SUCCESS);
  }

}
