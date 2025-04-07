This project provides a software-based implementation of the Lamport Bakery algorithm to achieve mutual exclusion.
It defines two primary functions — lamport_mutex_lock(int thread_id) and lamport_mutex_unlock(int thread_id) — packaged within a shared library for integration with other applications.

The objective is to evaluate the performance overhead of software mutexes by comparing them against hardware-assisted solutions such as those provided by the pthreads library.
