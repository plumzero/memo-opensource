
#include "md5.h"
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

  unsigned char buf[16];
  md5File(fin, buf);

  for (size_t i = 0; i < sizeof(buf); i++) {
    printf("%02x", buf[i]);
  }
  printf("\n");


  // fseek(fin, 0, SEEK_END);
  // size_t filesize = ftell(fin);
  // fseek(fin, 0, SEEK_SET);

  // const size_t step = 1024;
  // unsigned char buf[step];
  // size_t offset = 0, n;

  // MD5Context c;
  // md5Init(&c);
  // for (offset = 0; offset < filesize; offset += step) {
  //   n = (filesize - offset > step) ? step : (filesize - offset);
  //   if (fread(buf, 1, n, fin) != n) {
  //     printf("fread: %s\n", strerror(errno));
  //     fclose(fin);
  //     return -1;
  //   }
  //   md5Update(&c, buf, n);
  // }

  // md5Finalize(&c);

  // for (size_t i = 0; i < sizeof(c.digest); i++) {
  //   printf("%02x", c.digest[i]);
  // }
  // printf("\n");

  fclose(fin);

  return 0;
}

