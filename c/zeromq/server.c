#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <zmq.h>

const uint64_t MAX_RECORD_SIZE = 256;
const uint64_t BATCH_SIZE = 1024;
const int CLIENT_NUM = 1;

int ZeromqServe(char *filename) {
  uint64_t start = 0, end;
  void *context = zmq_ctx_new();

  //	Socket to send messages on
  void *sender = zmq_socket(context, ZMQ_PUSH);
  if (sender == NULL) {
    fprintf(stdout, "%s", zmq_strerror(errno));
    return 1;
  }
  if (zmq_bind(sender, "tcp://*:5557") != 0) {
    fprintf(stdout, "%s", zmq_strerror(errno));
    return 1;
  }

  //	Socket for control output
  void *controller = zmq_socket(context, ZMQ_PUB);
  if (controller == NULL) {
    fprintf(stdout, "%s", zmq_strerror(errno));
    return 1;
  }
  if (zmq_bind(controller, "tcp://*:5558") != 0) {
    fprintf(stdout, "%s", zmq_strerror(errno));
    return 1;
  }

  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stdout, "Open file error: %s\n", strerror(errno));
    return errno;
  }
  fseek(fp, 0, SEEK_END);
  uint64_t len = ftell(fp);
  fprintf(stdout, "Load file length: %ld\n", len);
  char *m = (char *)mmap(0, len, PROT_READ, MAP_PRIVATE, fileno(fp), 0);
  fclose(fp);
  if (m == (char *)(-1)) {
    fprintf(stdout, "Memory mapping load file error: %s\n", strerror(errno));
    return errno;
  }
  sleep(2);

  while (start < len) {
    if (start + BATCH_SIZE >= len)
      break;
    end = start + BATCH_SIZE - 1;
    uint64_t i;
    for (i = 0; i < MAX_RECORD_SIZE && end + i < len; i++) {
      if (m[end + i] == '\n')
        break;
    }
    if (i == MAX_RECORD_SIZE) {
      char *tmp = (char *)malloc(MAX_RECORD_SIZE + 1);
      strncpy(tmp, m + end, MAX_RECORD_SIZE);
      tmp[MAX_RECORD_SIZE] = '\0';
      fprintf(stdout, "Too large record: %s", tmp);
      free(tmp);
      return 1;
    } else {
      end += i;
      if (end == len)
        end--;
      int rc = zmq_send(sender, m + start, end - start + 1, 0);
      assert(rc == end - start + 1);
      start = end + 1;
    }
  }
  if (start < len) {
    end = len - 1;
    int rc = zmq_send(sender, m + start, end - start + 1, 0);
    assert(rc == end - start + 1);
  }

  fprintf(stdout, "pub kill signal now\n");
  for (int i = 0; i < CLIENT_NUM; i++) {
    fprintf(stdout, "Send kill %d\n", i);
    int rc = zmq_send(controller, "KILL", 4, 0);
    assert(rc == 4);
  }
  zmq_close(sender);
  zmq_close(controller);
  zmq_ctx_destroy(context);
  return 0;
}

int main(int argc, char **argv) { return ZeromqServe(argv[1]); }
