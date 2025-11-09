#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/mman.h>


int main(int argc, char* argv[], char* envp[]) {
  // envp = {
  //   KEY=VALUE\0
  // }

  void* mapping = mmap(
    NULL,
    10000000000,
    PROT_READ | PROT_WRITE,
    MAP_SHARED | MAP_ANON,
    -1/*fd*/,
    0
  );

  memset(mapping, 0, 100);

  pid_t pid = fork();

  switch (pid) {
    case -1:
    perror("fork");
    exit(EXIT_FAILURE);
    case 0:
    // in child
    printf("Hello from child, %d\n", pid);

    for (;;) {
      *(int*)mapping = 123;

      if (*(int*)mapping != 123) {
          printf("DATA RACE\n");
          break;
      }
    }

    exit(123123);
    break;

    default:
    // in parent
    printf("Hello from parent, %d\n", pid);
    break;
  }

  for (;;) {
    *(int*)mapping = 321;
    if (*(int*)mapping != 321) {
        printf("DATA RACE\n");
        break;
    }
  }

  int status;
  pid_t returned_pid = wait(&status);

  printf("status = %d\n", WEXITSTATUS(status));

  return 0;
}
