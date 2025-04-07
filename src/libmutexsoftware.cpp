#include "libmutexsoftware.h"
#include <atomic>
#include <algorithm>
#include <utility>

namespace {
    const int MAX_THREADS = 10;
    std::atomic<bool> choosing[MAX_THREADS];
    std::atomic<int> ticket[MAX_THREADS];
}

int get_max_ticket() {
    int max = 0;
    for (int i = 0; i < MAX_THREADS; ++i) {
        int t = ticket[i].load(std::memory_order_relaxed);
        if (t > max) max = t;
    }
    return max;
}

void lamport_mutex_lock(int thread_id) {
    choosing[thread_id].store(true, std::memory_order_relaxed);

    int my_ticket = get_max_ticket() + 1;
    ticket[thread_id].store(my_ticket, std::memory_order_release);

    choosing[thread_id].store(false, std::memory_order_release);

    for (int j = 0; j < MAX_THREADS; ++j) {
        if (j == thread_id) continue;

        while (choosing[j].load(std::memory_order_acquire)) {
            // busy wait
        }

        while (ticket[j].load(std::memory_order_acquire) != 0 &&
               std::make_pair(ticket[j].load(), j) <
               std::make_pair(ticket[thread_id].load(), thread_id)) {
            // busy wait
        }
    }
}

void lamport_mutex_unlock(int thread_id) {
    ticket[thread_id].store(0, std::memory_order_release);
}
