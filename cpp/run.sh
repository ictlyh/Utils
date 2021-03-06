#!/bin/bash

ROOT=$(cd $(dirname $0); pwd)
cd ${ROOT}

./auto-delete.out
./construct.out
./file_rw.out
#./gen-data.out
#./libpq-demo.out
#./libpq-epoll.out
./row_split.out
./smart-pointer.out
./square.out
./test-default-value.out
./test-delete.out
#./test-gflags.out
#./test-glog.out
./test-modulo.out
./test-reinterpret-cast.out
./test-string-size.out

gcov auto-delete.cc
gcov construct.cc
gcov file_rw.cc
#gcov gen-data.out
#gcov libpq-demo.cc
#gcov libpq-epoll.cc
gcov row_split.cc
gcov smart-pointer.cc
gcov square.cc
gcov test-default-value.cc
gcov test-delete.cc
#gcov test-gflags.cc
#gcov test-glog.cc
gcov test-modulo.cc
gcov test-reinterpret-cast.cc
gcov test-string-size.cc

cd concurrent-queue
./producer_consumer.out
gcov producer_consumer.cc
cd ..
