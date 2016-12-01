/*-------------------------------------------------------------------------
 *
 * bload.c
 *
 * This is the zeromq client of bulkload copy. It pulls data from zeromq server
 * and outputs the message to stdout.
 *
 * Copyright (c) 2012-2016, Citus Data, Inc.
 *
 * $Id$
 *
 *-------------------------------------------------------------------------
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>

static int BatchSize = 1024;    /* size of a zeromq message in bytes */
static int MaxRecordSize = 256; /* size of max acceptable record in bytes */

int main(int argc, char *argv[]) {
  FILE *fp = fopen("bload.log", "w");
  if (!fp)
    return errno;
  if (argc < 3) {
    fprintf(fp, "Usage: %s host port\n", argv[0]);
    fflush(fp);
    fclose(fp);
    return 1;
  }

  uint64_t buffer_size = BatchSize + MaxRecordSize + 1;
  char buf[buffer_size];
  char connstr[64];
  int rc;

  void *context = zmq_ctx_new();

  //	Socket to receive messages on
  void *receiver = zmq_socket(context, ZMQ_PULL);
  int port = atoi(argv[2]);
  sprintf(connstr, "tcp://%s:%d", argv[1], port);
  rc = zmq_connect(receiver, connstr);
  if (rc != 0) {
    fprintf(fp, "zmq_connect fail, errno: %d, errmsg: %s\n", errno,
            strerror(errno));
    fflush(fp);
    fclose(fp);
    zmq_close(receiver);
    zmq_ctx_destroy(context);
    return 1;
  }

  // Socket to receive control message
  void *controller = zmq_socket(context, ZMQ_SUB);
  sprintf(connstr, "tcp://%s:%d", argv[1], port + 1);
  rc = zmq_connect(controller, connstr);
  if (rc != 0) {
    fprintf(fp, "zmq_connect fail, errno: %d, errmsg: %s\n", errno,
            strerror(errno));
    fflush(fp);
    fclose(fp);
    zmq_close(receiver);
    zmq_close(controller);
    zmq_ctx_destroy(context);
    return 1;
  }
  zmq_setsockopt(controller, ZMQ_SUBSCRIBE, "", 0);

  int nbytes;
  zmq_pollitem_t items[] = {{receiver, 0, ZMQ_POLLIN, 0},
                            {controller, 0, ZMQ_POLLIN, 0}};
  while (true) {
    /* wait indefinitely for an event to occur */
    rc = zmq_poll(items, 2, -1);
    if (rc == -1) /* error occurs */
    {
      fprintf(fp, "zmq_poll fail, errno %d, errmsg:%s\n", errno,
              strerror(errno));
      fflush(fp);
      break;
    }
    if (items[0].revents & ZMQ_POLLIN) /* receive a message */
    {
      nbytes = zmq_recv(receiver, buf, buffer_size - 1, 0);
      if (nbytes == -1) {
        fprintf(fp, "zmq_recv fail, errno: %d, errmsg: %s\n", errno,
                strerror(errno));
        fflush(fp);
        break;
      }
      fwrite(buf, 1, nbytes, stdout);
      fflush(stdout);
    }
    if (items[1].revents & ZMQ_POLLIN) /* receive signal kill */
    {
      fprintf(fp, "receive signal kill, wait for exhausting all messages\n");
      fflush(fp);
      /* consume all messages before exit */
      while (true) {
        /* wait 100 milliseconds for an event to occur */
        rc = zmq_poll(items, 1, 100);
        if (rc == 0) /* no more messages */
        {
          break;
        } else if (rc == -1) /* error occurs */
        {
          fprintf(fp, "zmq_poll fail, errno %d, errmsg:%s\n", errno,
                  strerror(errno));
          fflush(fp);
          break;
        }
        if (items[0].revents & ZMQ_POLLIN) /* receive a message */
        {
          nbytes = zmq_recv(receiver, buf, buffer_size - 1, 0);
          if (nbytes == -1) {
            fprintf(fp, "zmq_recv fail, errno: %d, errmsg: %s\n", errno,
                    strerror(errno));
            fflush(fp);
            break;
          }
          fwrite(buf, 1, nbytes, stdout);
          fflush(stdout);
        }
      }
      fprintf(fp, "we have consume all messages, exit now\n");
      fflush(fp);
      break;
    }
  }
  zmq_close(receiver);
  zmq_close(controller);
  zmq_ctx_destroy(context);
  fclose(fp);
  return 0;
}
