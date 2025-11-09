#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


void error_exit(const char* reason) {
  perror(reason);
  exit(EXIT_FAILURE);
}


int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <path>\n", argv[0]);
    exit(1);
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    error_exit("open");
  }

  struct stat statbuf;
  if (fstat(fd, &statbuf) < 0) {
    error_exit("fstat");
  }

  void* addr = mmap(
    NULL,
    statbuf.st_size,
    PROT_READ,
    MAP_PRIVATE,
    fd,
    0
  );

  if (addr == MAP_FAILED) {
    error_exit("mmap");
  }

  madvise(addr, statbuf.st_size, MADV_SEQUENTIAL);

  long PAGE_SIZE = sysconf(_SC_PAGESIZE);

  char buff[PAGE_SIZE];

  // sequential
  // for (size_t i = 0; i < statbuf.st_size; i += PAGE_SIZE) {
  //   memcpy(buff, addr + i, PAGE_SIZE);
  // }

  // random
  for (size_t i = 0; i < statbuf.st_size; i += PAGE_SIZE) {
    size_t offset = (((size_t)rand() << 16) | ((size_t)rand())) % statbuf.st_size;
    offset &= ~(PAGE_SIZE - 1);
    memcpy(buff, addr + offset, PAGE_SIZE);
  }

  exit(EXIT_SUCCESS);
}
