#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
  // cat
  if (argc < 2) {
    printf("Usage: %s <path>\n", argv[0]);
    return 1;
  }

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }

  struct stat statbuf;
  if (fstat(fd, &statbuf) < 0) {
    perror("fstat");
    return 1;
  }

  void* mapping = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if (mapping == MAP_FAILED) {
    perror("mmap");
    return 1;
  }

  write(STDOUT_FILENO, mapping, statbuf.st_size);

  munmap(mapping, statbuf.st_size);
  close(fd);

  return 0;
}
