
#include "md5_speed.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    printf("USAGE: %s <file>\n", argv[0]);
    return -1;
  }

  unsigned char buf[16];
  md5FileSpeed(argv[1], buf);

  for (size_t i = 0; i < sizeof(buf); i++) {
    printf("%02x", buf[i]);
  }
  printf("\n");

  return 0;
}
