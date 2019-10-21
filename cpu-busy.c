#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int s;
  time_t t;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <busy cpu seconds>\n", argv[0]);
    exit(1);
  }

  s = abs(atoi(argv[1]));
  printf("Burning cpu cycles for %d seconds... ", s);
  fflush(stdout);
  t = time(NULL);
  while (1) {
    if (time(NULL) - t >= s) {
      break;
    }
  }
  printf("done.\n");

  return 0;
}
