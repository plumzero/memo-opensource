
#include "sha256.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("USAGE: %s <file>\n", argv[0]);
    return -1;
  }

  FILE* fin = fopen(argv[1], "rb");
  if (fin == NULL) {
    printf("fopen: %s\n", strerror(errno));
    return -1;
  }

  unsigned char buf[32];
  sha256_file(fin, buf);

  for (size_t i = 0; i < sizeof(buf); i++) {
    printf("%02x", buf[i]);
  }
  printf("\n");

  fclose(fin);

  return 0;
}

