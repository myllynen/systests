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
  struct sockaddr_in addr;
  void *mem;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <port> <read size in megabytes>\n", argv[0]);
    exit(1);
  }

  p = abs(atoi(argv[1]));
  if (p < 1 || p > 65535) {
    fprintf(stderr, "%s: invalid port\n", argv[0]);
    exit(1);
  }
  s = abs(atoi(argv[2]));
  if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons((unsigned short)p);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) != 0) {
    perror("bind");
    exit(1);
  }
  if ((mem = malloc(mega)) == NULL) {
    perror("malloc");
    exit(1);
  }
  printf("Reading %d megabytes from port %d... ", s, p);
  fflush(stdout);
  e = 0;
  for (i = 0; !e && i < s; i++) {
    for (j = 0; !e && j < 1024; j++) {
      if (recvfrom(sd, mem, 1024, MSG_WAITALL, NULL, NULL) != 1024) {
        e = 1;
      }
    }
    if (i == 0 && clock_gettime(CLOCK_MONOTONIC, &t1) == -1) {
      perror("clock_gettime");
      exit(1);
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
  printf("Read %d megabytes in %.2f seconds (~%.2f MB/s).\n", i, td, i / td);
  free(mem);
  close(sd);

  return e;
}
