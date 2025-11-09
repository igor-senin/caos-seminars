#include <pthread.h>
#include <stdint.h>
#include <stdio.h>


#define THREAD_COUNT 16

uint64_t shared_counter = 0;

typedef struct {
  pthread_mutex_t* mutex;
} thr_args;

void* thread_routine(void* void_args) {
  thr_args* args = void_args;

  uint64_t local_counter = 0;
  for (size_t i = 0; i < 1000000; ++i) {
    // pthread_mutex_lock(args->mutex);
    local_counter += 1;
    // pthread_mutex_unlock(args->mutex);
  }

  pthread_mutex_lock(args->mutex);
  shared_counter += local_counter;
  pthread_mutex_unlock(args->mutex);


  return 0;
}


int main() {
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  pthread_t threads[THREAD_COUNT];
  thr_args args = {
    .mutex = &mutex
  };
  for (size_t i = 0; i < THREAD_COUNT; ++i) {
    pthread_create(&threads[i], NULL, thread_routine, &args);
  }

  for (size_t i = 0; i < THREAD_COUNT; ++i) {
    pthread_join(threads[i], NULL);
  }

  printf("%ld\n", shared_counter);

  pthread_mutex_destroy(&mutex);
}
