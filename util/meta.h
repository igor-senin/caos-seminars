#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define SAFE_CALL(function_name, expression)      \
  do {                                            \
    if (unlikely((expression) < 0)) {             \
      perror(#function_name);                     \
      dprintf(STDERR_FILENO,                      \
              "expression: %s \t on line %d\n",   \
              #expression, __LINE__);             \
      exit(EXIT_FAILURE);                         \
    }                                             \
  } while (0)
