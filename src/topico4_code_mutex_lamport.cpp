#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "libmutexsoftware.h"

#define handle_error_en(en, msg) \
  do {                           \
    errno = en;                  \
    perror(msg);                 \
    exit(EXIT_FAILURE);          \
  } while (0)

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0)


int num_threads = 10;
int shared_var = 0;
int num_rep = 100;

struct thread_info {
  pthread_t id;
  int num;
};

static void *thread_start(void *arg) {
  struct thread_info *tinfo = (thread_info *)arg;

  //printf("Hello! I'm thread %d, id %lu!\n", tinfo->num, tinfo->id);
  for (int i = 0; i < num_rep; i++) {
    // MUTEX CODE BEGIN
    lamport_mutex_lock(tinfo->num);
    shared_var = shared_var + 1;
    lamport_mutex_unlock(tinfo->num);
    // MUTEX CODE END
  }

  return 0x0;
}

int main(int argc, char **argv) {
  int thread_num, ret;
  struct thread_info tinfo[num_threads];
  pthread_attr_t attr;
  void *res;

  if (argc > 1) num_rep = strtol(argv[1], NULL, 10);
  if (argc > 2) num_threads = strtol(argv[2], NULL, 10);

  ret = pthread_attr_init(&attr);
  if (ret != 0) handle_error_en(ret, "pthread_attr_init");

  /* Create one thread for each command-line argument */
  for (thread_num = 0; thread_num < num_threads; thread_num++) {
    tinfo[thread_num].num = thread_num + 1;

    ret = pthread_create(&tinfo[thread_num].id, &attr, &thread_start,
                         &tinfo[thread_num]);
    if (ret != 0) handle_error_en(ret, "pthread_create");
  }

  ret = pthread_attr_destroy(&attr);
  if (ret != 0) handle_error_en(ret, "pthread_attr_destroy");

  for (thread_num = 0; thread_num < num_threads; thread_num++) {
    ret = pthread_join(tinfo[thread_num].id, &res);
    if (ret != 0) handle_error_en(ret, "pthread_join");

    /* printf("Joined with thread %d, id %lu\n", tinfo[thread_num].num,
           tinfo[thread_num].id); */
    free(res); /* Free memory allocated by thread */
  }

  // prints shared_var final value
  printf("%d\n", shared_var);

  exit(EXIT_SUCCESS);
}
