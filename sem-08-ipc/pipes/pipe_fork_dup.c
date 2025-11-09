#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void error_exit(const char* reason) {
  perror(reason);
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {

  int pfd[2];

  if (pipe(pfd) == -1) {
    error_exit("pipe");
  }

  switch (fork()) {
    case -1:
      error_exit("fork");
    case 0: /* Первый ребёнок делает ls -l и записывает результат в канал */
      close(pfd[0]);

      /* проверка на всякий случай */
      if (pfd[1] != STDOUT_FILENO) {
        dup2(pfd[1], STDOUT_FILENO);

        close(pfd[1]);
      }

      execlp("ls", "ls", "-l", NULL);

      error_exit("execlp ls");
    default:
      break;
  }

  switch (fork()) {
    case -1:
      error_exit("fork");
    case 0:
      close(pfd[1]);

      if (pfd[0] != STDIN_FILENO) {
        dup2(pfd[0], STDIN_FILENO);

        close(pfd[0]);
      }

      execlp("wc", "wc", "-l", NULL);

      error_exit("execlp wc");

    default:
      break;
  }

  close(pfd[0]);
  close(pfd[1]);
  wait(NULL);
  wait(NULL);

  exit(EXIT_SUCCESS);
}
