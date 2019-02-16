CC_FLAGS   = -Wall -coverage -O0 -std=c++0x
CC          = g++
MAKE        = make

targets=$(patsubst %.cc,%.out,$(wildcard *.cc))

all: ${targets} concurrent-queue

auto-delete.out: auto-delete.cc
	${CC} ${CC_FLAGS} -o $@ $^
 
construct.out: construct.cc
	${CC} ${CC_FLAGS} -o $@ $^
 
file_rw.out: file_rw.cc
	${CC} ${CC_FLAGS} -o $@ $^
 
gen-data.out: gen-data.cc
	${CC} ${CC_FLAGS} -o $@ $^
 
#libpq-demo.out: libpq-demo.cc
#	${CC} ${CC_FLAGS} -o $@ $^ -lpq
#
#libpq-epoll.out: libpq-epoll.cc
#	${CC} ${CC_FLAGS} -o $@ $^ -lpq    
 
row_split.out: row_split.cc
	${CC} ${CC_FLAGS} -o $@ $^

smart-pointer.out: smart-pointer.cc
	${CC} ${CC_FLAGS} -o $@ $^
    
square.out: square.cc
	${CC} ${CC_FLAGS} -o $@ $^
 
test-default-value.out: test-default-value.cc
	${CC} ${CC_FLAGS} -o $@ $^
 
test-delete.out: test-delete.cc
	${CC} ${CC_FLAGS} -o $@ $^
 
#test-gflags.out: test-gflags.cc
#	${CC} ${CC_FLAGS} -o $@ $^ -lgflags
# 
#test-glog.out: test-glog.cc
#	${CC} ${CC_FLAGS} -o $@ $^ -lglog
 
test-modulo.out: test-modulo.cc
	${CC} ${CC_FLAGS} -o $@ $^
 
test-reinterpret-cast.out: test-reinterpret-cast.cc
	${CC} ${CC_FLAGS} -o $@ $^
 
test-string-size.out: test-string-size.cc
	${CC} ${CC_FLAGS} -o $@ $^

.PHONY: concurrent-queue

concurrent-queue:
	${MAKE} all -C concurrent-queue

clean:
	rm -rf *.out *.o *.gcno *.gcov *.gcda && ${MAKE} clean -C concurrent-queue
