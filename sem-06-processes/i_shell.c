#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define BUFF_SIZE 4096

char* shell_inv = "i_shell > ";

int main(int argc, char* argv[]) {
  char* buff = malloc(BUFF_SIZE);
  ssize_t bytes_read;

  printf("%s", shell_inv);

  size_t n = BUFF_SIZE;
  while ((bytes_read = getline(&buff, &n, stdin)) != -1) {
    char* args[10] = {NULL};
    char* pos = strtok(buff, " \n");

    for (int i = 0; pos != NULL; ++i) {
      args[i] = pos;
      pos = strtok(NULL, " \n");
    }

    pid_t pid = fork();
    if (pid == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      execvp(args[0], args);

      // ...

      perror("execvp");
      exit(EXIT_FAILURE);
    } else {
      waitpid(pid, NULL, 0);
    }

    printf("%s", shell_inv);
  }

  return 0;
}
