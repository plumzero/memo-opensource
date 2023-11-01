
#include <openssl/aes.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
  if (argc != 4) {
    printf("%s <e/d> <infile> <outfile>\n", argv[0]);
    return -1;
  }

  int mode = strncmp(argv[1], "e", 1) == 0 ? 1 :
            (strncmp(argv[1], "d", 1) == 0 ? 2 : -1);
  if (mode == -1) {
    printf("%s <e/d> <infile> <outfile>\n", argv[0]);
    return -1;
  }

  if (strcmp(argv[2], argv[3]) == 0) {
    printf("plain file and cipher file must different\n");
    return -1;
  }

  FILE* fin = fopen(argv[2], "rb");
  if (fin == NULL) {
    perror("fopen");
    return -1;
  }

  FILE* fout = fopen(argv[3], "wb+");
  if (fout == NULL) {
    perror("fopen");
    return -1;
  }

  fseek(fin, 0, SEEK_END);
  long fsize = ftell(fin);
  fseek(fin, 0, SEEK_SET);

  if (fsize % 16 != 0) {
    printf("file size(%lu) could not divided by 16\n", fsize);
    return -1;
  }

  AES_KEY key_ctx;
  unsigned char key[16] = { 0x64, 0x1d, 0xa0, 0x0d, 0x7f, 0xb1, 0x4b, 0x51, 0x8d, 0xa4, 0x2b, 0xf4, 0xef, 0x0c, 0x24, 0xf4 };

  // 加密
  if (mode == 1) {
    int ret = AES_set_encrypt_key(key, 128, &key_ctx);
    if (ret < 0) {
      printf("AES_set_encrypt_key failed\n");
      return -1;
    }
    unsigned char inbuf[1024] = {0};
    unsigned char outbuf[1024] = {0};
    for (long offset = 0; offset < fsize; offset += 16) {
      long n = (fsize - offset >= 16 ? 16 : fsize - offset);
      if (fread(inbuf, 1, 16, fin) != (size_t)n) {
        perror("fread");
        return -1;
      }
      AES_ecb_encrypt(inbuf, outbuf, &key_ctx, AES_ENCRYPT);
      if (fwrite(outbuf, 1, n, fout) != (size_t)n) {
        perror("fwrite");
        return -1;
      }
      memset(inbuf, 0, sizeof(inbuf));
      memset(outbuf, 0, sizeof(outbuf));
    }
  }
  // 解密 
  else {
    int ret = AES_set_decrypt_key(key, 128, &key_ctx);
    if (ret < 0) {
      printf("AES_set_decrypt_key failed\n");
      return -1;
    }
    unsigned char inbuf[1024] = {0};
    unsigned char outbuf[1024] = {0};
    for (long offset = 0; offset < fsize; offset += 16) {
      long n = (fsize - offset >= 16 ? 16 : fsize - offset);
      if (fread(inbuf, 1, 16, fin) != (size_t)n) {
        perror("fread");
        return -1;
      }
      AES_ecb_encrypt(inbuf, outbuf, &key_ctx, AES_DECRYPT);
      if (fwrite(outbuf, 1, n, fout) != (size_t)n) {
        perror("fwrite");
        return -1;
      }
      memset(inbuf, 0, sizeof(inbuf));
      memset(outbuf, 0, sizeof(outbuf));
    }
  }

  fclose(fin);
  fclose(fout);

  return 0;
}
