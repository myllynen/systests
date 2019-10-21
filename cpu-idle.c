#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int s;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <idle cpu seconds>\n", argv[0]);
    exit(1);
  }

  s = abs(atoi(argv[1]));
  printf("Idling for %d seconds... ", s);
  fflush(stdout);
  sleep((unsigned)s);
  printf("done.\n");

  return 0;
}
