/*
 * Demo of libpq epoll.
 * Build: g++ libpq-epoll.cpp -o libpq-epoll -lpq
 * Run: ./libpq-epoll
 */

#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <libpq-fe.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXEVENTS 64

int main (int argc, char *argv[])
{
  int efd;
  epoll_event events[MAXEVENTS];

  efd = epoll_create1(0);
  if (efd == -1)
  {
    std::cout << "epoll_create" << std::endl;
    abort ();
  }

  const char conninfo[] =
    "postgresql://postgres@localhost?port=5432&dbname=postgres";
  PGconn *conn = PQconnectdb(conninfo);
  const ConnStatusType &connStatusType = PQstatus(conn);
  if(connStatusType == CONNECTION_BAD)
  {
    std::cout << "pg connection not OK" << std::endl;
    if (conn)
    {
      PQfinish(conn);
    }
    exit(1);
  }
  else
  {
    std::cout << "pg connection OK" << std::endl;
  }
  const char *query = "SELECT * FROM tb1";
  int querySent = PQsendQuery(conn, query);
  if (querySent == 0)
  {
    std::cout << PQerrorMessage(conn);
    PQfinish(conn);
    exit(1);
  }
  else
  {
    std::cout << "Send query " << query << std::endl;
  }
  int sock = PQsocket(conn);
  if (sock < 0)
  {
    std::cout << "pg no sock" << std::endl;
    PQfinish(conn);
    exit(1);
  }
  else
  {
    std::cout << "get socket of pg connection OK" << std::endl;
  }
  epoll_event event;
  event.events = EPOLLIN | EPOLLERR | EPOLLET ;
  event.data.fd = sock;

  // add the socket to the epoll file descriptors
  if(epoll_ctl(efd, EPOLL_CTL_ADD, sock, &event) != 0)
  {
    std::cout << "epoll_ctl, adding socket fail" << std::endl;
    PQfinish(conn);
    exit(1);
  }
  else
  {
    std::cout << "epoll_ctl, adding socket OK" << std::endl;
  }

  /* The event loop */
  for (;;)
  {
    int n, i;
    n = epoll_wait(efd, events, MAXEVENTS, -1);
    for (i = 0; i < n; i++)
    {
      if (events[i].events & EPOLLERR)
      {
        std::cout << "epoll_ctl, socket error" << std::endl;
        PQfinish(conn);
        exit(1);
      }
      else
      {
        if (events[i].events & EPOLLIN) //socket is ready for reading
        {
          std::cout << "epoll_ctl, socket ready to read" << std::endl;
          int rc = PQconsumeInput(conn);
          if (rc == 0)
          {
            std::cout << PQerrorMessage(conn);
            PQfinish(conn);
            exit(1);
          }
          PGnotify *notify;
          while ((notify = PQnotifies(conn)) != NULL)
          {
            fprintf(stderr,
                "ASYNC NOTIFY of '%s' received from backend PID %d\n",
                notify->relname, notify->be_pid);
            PQfreemem(notify);
          }
          if (PQisBusy(conn) == 0)
          {
            PGresult *result;
            while ((result = PQgetResult(conn)) != NULL)
            {
              if (PQresultStatus(result) != PGRES_TUPLES_OK)
              {
                fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
                PQclear(result);
                PQfinish(conn);
                exit(1);
              }
              else
              {
                int nFields = PQnfields(result);
                for (i = 0; i < nFields; i++)
                {
                  int ptype = PQftype(result, i);
                  std::cout << PQfname(result, i) << "\t";
                }
                std::cout << "\n------------\n";
                /* next, print out the rows */
                for (i = 0; i < PQntuples(result); i++)
                {
                  for (int j = 0; j < nFields; j++)
                    std::cout << PQgetvalue(result, i, j) << "\t";
                  std::cout << "\n";
                }
                std::cout << "------------\n";
              }
              PQclear(result);
            }
            PQfinish(conn);
            exit(0);
          }
        }
      }
    }
  }
}
