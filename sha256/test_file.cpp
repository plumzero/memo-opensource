
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

  fseek(fin, 0, SEEK_END);
  size_t filesize = ftell(fin);
  fseek(fin, 0, SEEK_SET);

  const size_t step = 1024;
  unsigned char buf[step];
  size_t offset = 0, n;

  sha256_context c;
  sha256_init(&c);
  for (offset = 0; offset < filesize; offset += step) {
    n = (filesize - offset > step) ? step : (filesize - offset);
    if (fread(buf, 1, n, fin) != n) {
      printf("fread: %s\n", strerror(errno));
      fclose(fin);
      return -1;
    }
    sha256_hash(&c, buf, n);
  }

  uint8_t hash[32];
  sha256_done(&c, hash);

  for (size_t i = 0; i < sizeof(hash); i++) {
    printf("%02x", hash[i]);
  }
  printf("\n");

  fclose(fin);

  return 0;
}

