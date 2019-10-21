#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int e, i, j, p, s, sd;
  size_t mega = 1024 * 1024;
  struct timespec t1, t2;
  double td;
  struct addrinfo h, *r;
  const char *host, *ip;
  void *mem;

  if (argc != 4) {
    fprintf(stderr, "Usage: %s <address> <port> <write size in megabytes>\n", argv[0]);
    exit(1);
  }

  p = abs(atoi(argv[2]));
  if (p < 1 || p > 65535) {
    fprintf(stderr, "%s: invalid port\n", argv[0]);
    exit(1);
  }
  memset(&h, 0, sizeof(struct addrinfo));
  h.ai_flags = AI_CANONNAME|AI_ADDRCONFIG;
  h.ai_family = AF_INET;
  h.ai_socktype = SOCK_DGRAM;
  h.ai_protocol = IPPROTO_UDP;
  if ((e = getaddrinfo(argv[1], argv[2], &h, &r)) != 0) {
    if (e == EAI_SYSTEM) {
      perror("getaddrinfo");
    } else {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(e));
    }
    exit(1);
  }
  s = abs(atoi(argv[3]));
  if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
  if ((mem = malloc(mega)) == NULL) {
    perror("malloc");
    exit(1);
  }
  ip = inet_ntop(AF_INET, &((struct sockaddr_in *)r->ai_addr)->sin_addr, mem, r->ai_addrlen);
  if (r->ai_canonname && strcmp(argv[1], r->ai_canonname)) {
    printf("Resolved %s to %s (%s).\n", argv[1], r->ai_canonname, ip);
    host = r->ai_canonname;
  } else {
    printf("Resolved %s to %s.\n", argv[1], ip);
    host = argv[1];
  }
  memset(mem, 0, mega);
  printf("Writing %d megabytes to %s:%d... ", s, host, p);
  fflush(stdout);
  if (clock_gettime(CLOCK_MONOTONIC, &t1) == -1) {
    perror("clock_gettime");
    exit(1);
  }
  e = 0;
  for (i = 0; !e && i < s; i++) {
    for (j = 0; !e && j < 1024; j++) {
      if (sendto(sd, mem, 1024, 0, r->ai_addr, r->ai_addrlen) != 1024) {
        e = 1;
      }
    }
  }
  if (clock_gettime(CLOCK_MONOTONIC, &t2) == -1) {
    perror("clock_gettime");
    exit(1);
  }
  if (e) {
    printf("failed!\n");
  } else {
    printf("done.\n");
  }
  td = (double)(t2.tv_sec - t1.tv_sec) + 1.0e-9 * (double)(t2.tv_nsec - t1.tv_nsec);
  printf("Wrote %i megabytes in %.2f seconds (~%.2f MB/s).\n", i, td, i / td);
  freeaddrinfo(r);
  free(mem);
  close(sd);

  return e;
}
