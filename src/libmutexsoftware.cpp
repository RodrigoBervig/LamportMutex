#include "libmutexsoftware.h"

#include <algorithm>
#include <utility>

namespace {
    const int MAX_THREADS = 10;
    bool choosing[MAX_THREADS];
    int ticket[MAX_THREADS];
}

int get_max_ticket() {
    int max = 0;
    for (int i = 0; i < MAX_THREADS; ++i) {
        int t = ticket[i];
        if (t > max) max = t;
    }
    return max;
}

void lamport_mutex_lock(int thread_id) {
    choosing[thread_id] = true;

    int my_ticket = get_max_ticket() + 1;
    ticket[thread_id] = my_ticket;

    choosing[thread_id] = false;

    for (int j = 0; j < MAX_THREADS; ++j) {
        if (j == thread_id) continue;

        while (choosing[j]) {
            // busy wait
        }

        while (ticket[j] != 0 &&
               std::make_pair(ticket[j], j) <
               std::make_pair(ticket[thread_id], thread_id)) {
            // busy wait
        }
    }
}

void lamport_mutex_unlock(int thread_id) {
    ticket[thread_id] = 0;
}
