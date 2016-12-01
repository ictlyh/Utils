CC_FLAGS   = -Wall -coverage -O0 -std=c++0x -pthread
CC          = g++
MAKE        = make

all: producer_consumer.out

producer_consumer.out: producer_consumer.cc concurrent-queue.h
	$(CC) $(CC_FLAGS) -o $@ $^

clean:
	rm -rf *.out *.o *.gcno *.gcov *.gcda
