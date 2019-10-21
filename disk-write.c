#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int fd, i, sz;
  size_t mega = 1024 * 1024;
  struct timespec t1, t2;
  double td;
  void *mem;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <path> <file size in megabytes>\n", argv[0]);
    exit(1);
  }

  sz = abs(atoi(argv[2]));
  if ((fd = open(argv[1], O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR)) == -1) {
    fprintf(stderr, "%s: can't open %s: %s\n", argv[0], argv[1], strerror(errno));
    exit(1);
  }
  printf("Writing %d megabytes to file %s... ", sz, argv[1]);
  fflush(stdout);
  if ((mem = malloc(mega)) == NULL) {
    perror("malloc");
    exit(1);
  }
  memset(mem, 0, mega);
  if (clock_gettime(CLOCK_MONOTONIC, &t1) == -1) {
    perror("clock_gettime");
    exit(1);
  }
  for (i = 0; i < sz; i++) {
    if (write(fd, mem, mega) != mega) {
      break;
    }
  }
  fsync(fd);
  if (clock_gettime(CLOCK_MONOTONIC, &t2) == -1) {
    perror("clock_gettime");
    exit(1);
  }
  if (i == sz) {
    printf("done.\n");
  } else {
    printf("failed!\n");
  }
  td = (double)(t2.tv_sec - t1.tv_sec) + 1.0e-9 * (double)(t2.tv_nsec - t1.tv_nsec);
  printf("Wrote %i megabytes in %.2f seconds (~%.2f MB/s).\n", i, td, i / td);
  free(mem);
  close(fd);

  return 0;
}
