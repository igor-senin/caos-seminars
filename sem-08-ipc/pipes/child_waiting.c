#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void error_exit(const char* reason) {
  perror(reason);
  exit(EXIT_FAILURE);
}

void do_hard_work(int time_to_sleep, pid_t pid) {
  printf("%ld: do hard work...\n", (long)pid);
  sleep(time_to_sleep);
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    dprintf(STDERR_FILENO,
            "Usage: %s <forks_count> <time_to_sleep>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int pfd[2];

  if (pipe(pfd) == -1) {
    error_exit("pipe");
  }

  setbuf(stdout, NULL);

  int forks_count = atoi(argv[1]);
  int time_to_sleep = atoi(argv[2]);

  for (int i = 1; i <= forks_count; ++i) {

    switch (fork()) {
    case -1:
      error_exit("fork");
    case 0:
      close(pfd[0]);

      do_hard_work(time_to_sleep, getpid());

      close(pfd[1]);

      exit(EXIT_SUCCESS);

    default:
      break;
    }

  }

  /* still in parent */
  close(pfd[1]);

  int smth;
  read(pfd[0], &smth, sizeof(smth));

  printf("Parent ready to go!\n");

  exit(EXIT_SUCCESS);
}
