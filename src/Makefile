CXX = g++
CXXFLAGS = -Wall -fPIC -std=c++17
LDFLAGS = -L. -lmutexsoftware -lpthread
OUT = run.out

# Default target
all: libmutexsoftware.so

# Rule to build the shared library
libmutexsoftware.so: libmutexsoftware.cpp libmutexsoftware.h
	$(CXX) $(CXXFLAGS) -shared -o libmutexsoftware.so libmutexsoftware.cpp

# Build the selected file into an executable
# Usage: make build FILE=main.cpp
build: libmutexsoftware.so
	$(CXX) $(CXXFLAGS) $(FILE) -o $(OUT) $(LDFLAGS)

# Build and run
# Usage: make run FILE=main.cpp
run: build
	LD_LIBRARY_PATH=. ./$(OUT)

# Clean up
clean:
	rm -f $(OUT) libmutexsoftware.so
