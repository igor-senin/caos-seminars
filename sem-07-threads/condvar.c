#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>


typedef struct {
  int64_t current_num;
  int32_t num_updated;
  pthread_mutex_t* mutex; // protects thr_args
  pthread_cond_t* ready;
} thr_args;


void* worker(void* void_args) {
  thr_args* args = void_args;

  for (size_t i = 0; i < 10; ++i) {
    pthread_mutex_lock(args->mutex);

    while (args->num_updated == 1) {
      pthread_cond_wait(args->ready, args->mutex);
    }

    args->current_num = i * i;
    args->num_updated = 1;

    pthread_cond_signal(args->ready);

    pthread_mutex_unlock(args->mutex);
  }

  return 0;
}


int main() {
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);

  pthread_cond_t ready;
  pthread_cond_init(&ready, NULL);

  thr_args args = {
    .num_updated = 0,
    .current_num = 0,
    .mutex = &mutex,
    .ready = &ready
  };

  pthread_t thread;
  pthread_create(&thread, NULL, worker, &args);

  for (size_t i = 0; i < 10; ++i) {
    pthread_mutex_lock(&mutex);

    while (args.num_updated == 0) {
      pthread_cond_wait(&ready, &mutex);
    }

    printf("%ld: current_num=%ld\n", i, args.current_num);
    fflush(stdout);

    args.num_updated = 0;

    pthread_cond_signal(&ready);

    pthread_mutex_unlock(&mutex);
  }

  pthread_join(thread, NULL);


  return 0;
}
