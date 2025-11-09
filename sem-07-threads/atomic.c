#include <stdatomic.h>
#include <pthread.h>
#include <stdio.h>


#define THREAD_COUNT 16

typedef struct {
  pthread_t thread;
  atomic_int* counter;
} thr_args;


void* thread_routine(void* void_args) {
  thr_args* args = void_args;

  for (size_t i = 0; i < 1000000; ++i) {
    atomic_fetch_add(args->counter, 1);
  }

  return 0;
}

int main() {
  thr_args thread_args[THREAD_COUNT];

  atomic_int shared_counter = 0;

  for (size_t i = 0; i < THREAD_COUNT; ++i) {
    thread_args[i].counter = &shared_counter;
    pthread_create(&thread_args[i].thread, NULL, thread_routine, thread_args);
  }

  for (size_t i = 0; i < THREAD_COUNT; ++i) {
    pthread_join(thread_args[i].thread, NULL);
  }

  printf("shared counter = %d\n", shared_counter);

  return 0;
}
