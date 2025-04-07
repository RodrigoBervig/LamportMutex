#ifndef LIBMUTEXSOFTWARE_H
#define LIBMUTEXSOFTWARE_H

#define NUMBER_OF_THREADS 3

void lamport_mutex_lock(int thread_id);
void lamport_mutex_unlock(int thread_id);

#endif  // LIBMUTEXSOFTWARE_H