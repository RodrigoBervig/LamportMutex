#include <pthread.h>

#include <iostream>

#include "libmutexsoftware.h"

constexpr int NUM_THREADS = 3;
constexpr int INCREMENTS = 100;

int shared_counter = 0;

void* thread_func(void* arg) {
  int id = *(int*)arg;
  for (int i = 0; i < INCREMENTS; ++i) {
    lamport_mutex_lock(id);
    ++shared_counter;
    lamport_mutex_unlock(id);
  }
  return nullptr;
}

int main() {
  pthread_t threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; ++i) {
    thread_ids[i] = i;
    pthread_create(&threads[i], nullptr, thread_func, &thread_ids[i]);
  }

  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_join(threads[i], nullptr);
  }

  std::cout << "Final counter: " << shared_counter << std::endl;
  return 0;
}
