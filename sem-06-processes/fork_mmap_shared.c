#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


void error_exit(const char* reason) {
  perror(reason);
  exit(EXIT_FAILURE);
}


int main(int argc, char* argv[]) {
  int* addr = mmap(
    NULL,
    sizeof(int),
    PROT_READ | PROT_WRITE,
    MAP_SHARED | MAP_ANONYMOUS,
    -1,
    0
  );
  if (addr == MAP_FAILED) {
    error_exit("mmap");
  }

  *addr = 1;

  switch (fork()) {
    case -1: // fork gone wrong
      error_exit("fork");
      break;

    case 0: // in child
      printf("Child process, value = %d\n", *addr);
      (*addr)++;

      if (munmap(addr, sizeof(int)) == -1) {
        error_exit("munmap");
      }

      exit(EXIT_SUCCESS);

    default: // in parent
      if (wait(NULL) == -1) {
        error_exit("wait");
      }

      printf("Parent process, value = %d\n", *addr);

      if (munmap(addr, sizeof(int)) == -1) {
        error_exit("munmap");
      }
      exit(EXIT_SUCCESS);
  }
}
