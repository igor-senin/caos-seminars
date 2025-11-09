#include <pthread.h>
#include <stdio.h>


int arr[4096];


#define thr_count 8

// int res[thr_count];
int result = 0;

struct thr_args {
  int num;
  int start;
};


void* thread_routine(void* void_args) {
  struct thr_args* args = (struct thr_args*)void_args;


  for (int i = args->start; i < args->start + 4096/8; ++i) {
    // res[args->num] += arr[i];
    result += arr[i];
  }

  return NULL;
}


int main() {
  for (int i = 0; i < 4096; ++i) {
    arr[i] = i;
  }

  pthread_t threads[thr_count];
  struct thr_args args[thr_count];

  for (int i = 0; i < thr_count; ++i) {
    struct thr_args arg = {
      .num = i,
      .start = i * (4096/thr_count)
    };
    args[i] = arg;
    pthread_create(threads + i, NULL, thread_routine, &args[i]);
  }

  for (int i = 0; i < thr_count; ++i) {
    pthread_join(threads[i], NULL);
    // pthread_detach();
  }

  // int result = 0;
  // for (int i = 0; i < thr_count; ++i) {
  //   result += res[i];
  // }

  printf("result = %d\n", result);

}
