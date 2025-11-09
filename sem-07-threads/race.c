#include <pthread.h>
#include <stdint.h>
#include <stdio.h>


uint64_t counter = 0;
// 1. mutex
// 2. atomic: _Atomic(uint64_t)

void* thread_routine(void* void_args) {
  for (size_t i = 0; i < 1000000; ++i) {
    counter += 1;
  }
  return 0;
}

int main() {
  pthread_t threads[2];

  pthread_create(&threads[0], NULL, thread_routine, NULL);
  pthread_create(&threads[1], NULL, thread_routine, NULL);

  pthread_join(threads[0], NULL);
  pthread_join(threads[1], NULL);

  printf("%ld\n", counter);
}
