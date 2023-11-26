
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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

  unsigned char key[16] = { 0x64, 0x1d, 0xa0, 0x0d, 0x7f, 0xb1, 0x4b, 0x51, 0x8d, 0xa4, 0x2b, 0xf4, 0xef, 0x0c, 0x24, 0xf4 };
	unsigned char iv[16] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };

  // 加密
  if (mode == 1) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    // Don't set key or IV right away; we want to check lengths
    EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, NULL, NULL, AES_ENCRYPT);
    assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
    assert(EVP_CIPHER_CTX_iv_length(ctx) == 16);
    // Now we can set key and IV
    EVP_CipherInit_ex(ctx, NULL, NULL, key, iv, AES_ENCRYPT);
    EVP_CIPHER_CTX_set_padding(ctx, 1); // 启用 padding, 具体指的是 PKCS#7

    unsigned char inbuf[1024] = {0};
    unsigned char outbuf[1024] = {0};
    long outlen = sizeof(outbuf);

    for (long offset = 0; offset < fsize; offset += 16) {
      long inlen = (fsize - offset >= 16 ? 16 : fsize - offset);
      if (fread(inbuf, 1, 16, fin) != (size_t)inlen) {
        perror("fread");
        return -1;
      }
      if (!EVP_CipherUpdate(ctx, outbuf, (int*)&outlen, inbuf, inlen)) {
        printf("EVP_CipherUpdate err!\n");
        return -1;
      }
      if (fwrite(outbuf, 1, outlen, fout) != (size_t)outlen) {
        perror("fwrite");
        return -1;
      }
    }

    if (!EVP_CipherFinal_ex(ctx, outbuf, (int*)&outlen)) {
      printf("EVP_CipherFinal_ex err!\n");
      return -1;
    }
    if (fwrite(outbuf, 1, outlen, fout) != (size_t)outlen) {
      perror("fwrite");
      return -1;
    }

    EVP_CIPHER_CTX_free(ctx);
  }
  // 解密 
  else {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    // Don't set key or IV right away; we want to check lengths
    EVP_CipherInit_ex(ctx, EVP_aes_128_cbc(), NULL, NULL, NULL, AES_DECRYPT);
    assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
    assert(EVP_CIPHER_CTX_iv_length(ctx) == 16);
    // Now we can set key and IV
    EVP_CipherInit_ex(ctx, NULL, NULL, key, iv, AES_DECRYPT);
    EVP_CIPHER_CTX_set_padding(ctx, 1); // 启用 padding, 具体指的是 PKCS#7

    unsigned char inbuf[1024] = {0};
    unsigned char outbuf[1024] = {0};
    long outlen = sizeof(outbuf);

    for (long offset = 0; offset < fsize; offset += 16) {
      long inlen = (fsize - offset >= 16 ? 16 : fsize - offset);
      if (fread(inbuf, 1, 16, fin) != (size_t)inlen) {
        perror("fread");
        return -1;
      }
      if (!EVP_CipherUpdate(ctx, outbuf, (int*)&outlen, inbuf, inlen)) {
        printf("EVP_CipherUpdate err!\n");
        return -1;
      }
      if (fwrite(outbuf, 1, outlen, fout) != (size_t)outlen) {
        perror("fwrite");
        return -1;
      }
    }

    if (!EVP_CipherFinal_ex(ctx, outbuf, (int*)&outlen)) {
      printf("EVP_CipherFinal_ex err!\n");
      return -1;
    }
    if (fwrite(outbuf, 1, outlen, fout) != (size_t)outlen) {
      perror("fwrite");
      return -1;
    }

    EVP_CIPHER_CTX_free(ctx);
  }

  fclose(fin);
  fclose(fout);

  return 0;
}
