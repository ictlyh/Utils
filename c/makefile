C_FLAGS      = -Wall -coverage -O0
C           = gcc
MAKE        = make

targets=$(patsubst %.c,%.out,$(wildcard *.c))
all: ${targets} socket zeromq

curl-get.out: curl-get.c
	${C} ${C_FLAGS} -o $@ $^ -lcurl

curl-post.out: curl-post.c
	${C} ${C_FLAGS} -o $@ $^ -lcurl
 
genoffset.out: genoffset.c
	${C} ${C_FLAGS} -o $@ $^
 
gethostname.out: gethostname.c
	${C} ${C_FLAGS} -o $@ $^
 
hashfunc.out: hashfunc.c
	${C} ${C_FLAGS} -o $@ $^
 
list-dir.out: list-dir.c
	${C} ${C_FLAGS} -o $@ $^
 
print-bits.out: print-bits.c
	${C} ${C_FLAGS} -o $@ $^
 
start_turbo.out: start_turbo.c
	${C} ${C_FLAGS} -o $@ $^
 
test-argp.out: test-argp.c
	${C} ${C_FLAGS} -o $@ $^
 
test-endian.out: test-endian.c
	${C} ${C_FLAGS} -o $@ $^
 
test-fork.out: test-fork.c
	${C} ${C_FLAGS} -o $@ $^
 
test-handlezombie.out: test-handlezombie.c
	${C} ${C_FLAGS} -o $@ $^
 
test-macro.out: test-macro.c
	${C} ${C_FLAGS} -o $@ $^
 
test-memcpy.out: test-memcpy.c
	${C} ${C_FLAGS} -o $@ $^
 
test-orphan.out: test-orphan.c
	${C} ${C_FLAGS} -o $@ $^
 
test-sprintf.out: test-sprintf.c
	${C} ${C_FLAGS} -o $@ $^
 
test-strcpy.out: test-strcpy.c
	${C} ${C_FLAGS} -o $@ $^
 
test-zombie.out: test-zombie.c
	${C} ${C_FLAGS} -o $@ $^
 
test-signal.out: test-signal.c
	${C} ${C_FLAGS} -o $@ $^
 
.PHONY: socket zeromq

zeromq:
	${MAKE} all -C zeromq

socket:
	${MAKE} all -C socket

clean:
	rm -rf *.out *.o *.gcno *.gcov *.gcda && ${MAKE} clean -C zeromq && ${MAKE} clean -C socket
