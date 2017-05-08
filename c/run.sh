#!/bin/bash

ROOT=$(cd $(dirname $0); pwd)
cd ${ROOT}

./curl-get.out
./curl-post.out
#./gen-offset.out
./gethostname.out
./hashfunc.out
./list-dir.out
./print-bits.out
./test-argp.out  -d db1 -f binary -h github.com -p 5432 table file
./test-endian.out
./test-fork.out
./test-handlezombie.out
./test-macro.out
./test-memcpy.out
./test-orphan.out
./test-sprintf.out
./test-strcpy.out
./test-zombie.out
./test-signal.out &
pkill -USR1 test-signal
pkill -INT test-signal
pkill -TERM test-signal
cd ./socket
./server &
# wait server to get ready
sleep 1
./client 127.0.0.1 5000
cd ..
#cd ./zeromq
#./server server.c &
#./client localhost 5557
#cd ..
./test-posix.out

gcov curl-get.c
gcov curl-post.c
#gcov gen-offset.c
gcov gethostname.c
gcov hashfunc.c
gcov list-dir.c
gcov print-bits.c
gcov test-argp.c
gcov test-endian.c
gcov test-fork.c
gcov test-handlezombie.c
gcov test-macro.c
gcov test-memcpy.c
gcov test-orphan.c
gcov test-sprintf.c
gcov test-strcpy.c
gcov test-zombie.c
gcov test-signal.c

cd ./socket
gcov server.c
gcov client.c
cd ..

#cd ./zeromq
#gcov server.c
#gcov client.c
#cd ..
gcov test-posix.c
