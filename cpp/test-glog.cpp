/*
 * Demo of glog.
 * Build: g++ test-glog.cc -o test-glog.out -lglog
 * Run: export GLOG_log_dir=/path/to/logdir && ./test-glog
 */

#include <glog/logging.h>

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  LOG(INFO) << "info message";
  LOG(WARNING) << "warning message";
  LOG(ERROR) << "error message";
  // LOG(FATAL) << "fatal message, program exit immediately";
  for (int i = 0; i < 10; ++i) {
    LOG_IF(INFO, i > 5) << "LOG_IF i > 5 i:" << i;
    LOG_EVERY_N(INFO, 3) << "LOG_EVERY_N 3 i:" << i
                         << " google::COUNTER:" << google::COUNTER;
    LOG_IF_EVERY_N(INFO, (i > 5), 2) << "LOG_IF_EVERY_N i > 5 2 i:" << i;
    LOG_FIRST_N(INFO, 4) << "LOG_FIRST_N 4 i:" << i
                         << " google::COUNTER:" << google::COUNTER;
  }
  return 0;
}
