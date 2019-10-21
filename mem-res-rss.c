#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int i, res, hold;
  size_t mega = 1024 * 1024;
  struct timespec t1, t2;
  double td;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <mem res in megabytes> <hold seconds>\n", argv[0]);
    exit(1);
  }

  res = abs(atoi(argv[1]));
  hold = abs(atoi(argv[2]));
  printf("Reserving %d megabytes (RSS)... ", res);
  fflush(stdout);
  if (clock_gettime(CLOCK_MONOTONIC, &t1) == -1) {
    perror("clock_gettime");
    exit(1);
  }
  for (i = 0; i < res; i++) {
    void *mem = malloc(mega);
    if (mem == NULL) {
      break;
    }
    memset(mem, 0, mega);
  }
  if (clock_gettime(CLOCK_MONOTONIC, &t2) == -1) {
    perror("clock_gettime");
    exit(1);
  }
  if (i == res) {
    printf("done.\n");
  } else {
    printf("failed!\n");
  }
  td = (double)(t2.tv_sec - t1.tv_sec) + 1.0e-9 * (double)(t2.tv_nsec - t1.tv_nsec);
  printf("Reserved %i megabytes in %.2f seconds (~%.2f MB/s).\n", i, td, i / td);
  printf("Holding reservation for %d seconds... ", hold);
  fflush(stdout);
  sleep((unsigned)hold);
  printf("done.\n");

  return 0;
}
