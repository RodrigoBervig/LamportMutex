#!/bin/bash

# Ensure we're working in the script's directory
cd "$(dirname "$0")"

# Paths
SRC_DIR="./src"
OUT_DIR="./results"
mkdir -p "$OUT_DIR"

PTHREAD_SRC="$SRC_DIR/topico4_code_mutex_pthread.cpp"
LAMPORT_SRC="$SRC_DIR/topico4_code_mutex_lamport.cpp"

PTHREAD_BIN="pthread_mutex_test"
LAMPORT_BIN="lamport_mutex_test"

# Compile both binaries (quotes added to handle spaces)
g++ -O2 -std=c++17 -o "$PTHREAD_BIN" "$PTHREAD_SRC" -lpthread || exit 1
g++ -O2 -std=c++17 -o "$LAMPORT_BIN" "$LAMPORT_SRC" -L"$SRC_DIR" -lmutexsoftware -lpthread || exit 1

# Output CSV
RESULTS_CSV="$OUT_DIR/mutex_benchmark.csv"
echo "implementation,adds_per_thread,total_threads,time_seconds,result_sum" > "$RESULTS_CSV"

# Benchmark settings
THREAD_COUNTS=(2 4 6)
LOADS=(10000 100000 1000000 10000000)

for THREADS in "${THREAD_COUNTS[@]}"; do
  for LOAD in "${LOADS[@]}"; do
      echo "Running with $THREADS threads and $LOAD additions per thread..."

      # Pthread mutex test
      SUM_PTHREAD=$(./$PTHREAD_BIN "$LOAD" "$THREADS")
      TIME_PTHREAD=$( { /usr/bin/time -f "%e" ./$PTHREAD_BIN "$LOAD" "$THREADS" > /dev/null; } 2>&1 )
      echo "pthread,$LOAD,$THREADS,$TIME_PTHREAD,$SUM_PTHREAD" >> "$RESULTS_CSV"

      # Lamport mutex test
      SUM_LAMPORT=$(LD_LIBRARY_PATH="$SRC_DIR" ./$LAMPORT_BIN "$LOAD" "$THREADS")
      TIME_LAMPORT=$( { LD_LIBRARY_PATH="$SRC_DIR" /usr/bin/time -f "%e" ./$LAMPORT_BIN "$LOAD" "$THREADS" > /dev/null; } 2>&1 )
      echo "lamport,$LOAD,$THREADS,$TIME_LAMPORT,$SUM_LAMPORT" >> "$RESULTS_CSV"
  done
done

echo "✅ Benchmark complete. Results saved to $RESULTS_CSV"
