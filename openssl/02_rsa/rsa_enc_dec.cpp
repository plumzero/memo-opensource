
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <stdio.h>
#include <string.h>

// 功能: 加解密文件(每段都填充)

const char* prikey = "pri.pem";
const char* pubkey = "pub.pem";

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

  int padding = RSA_PKCS1_PADDING;

  // 公钥加密
  if (mode == 1) {
    RSA* r;
    BIO* in = BIO_new_file(pubkey, "r");
    r = PEM_read_bio_RSAPublicKey(in, NULL, NULL, NULL);
    int rlen = RSA_size(r);
    rlen -= 11;

    unsigned char inbuf[1024] = {0};
    unsigned char outbuf[1024] = {0};
    for (long offset = 0; offset < fsize; offset += rlen) {
      long n = (fsize - offset >= rlen ? rlen : fsize - offset);
      if (fread(inbuf, 1, n, fin) != (size_t)n) {
        perror("fread");
        return -1;
      }
      int len = RSA_public_encrypt(n, inbuf, outbuf, r, padding);
      if (len <= 0) {
        printf("RSA_public_encrypt err!\n");
        return -1;
      }
      if (fwrite(outbuf, 1, len, fout) != (size_t)len) {
        perror("fwrite");
        return -1;
      }
      memset(inbuf, 0, sizeof(inbuf));
      memset(outbuf, 0, sizeof(outbuf));
    }
    
    BIO_free(in);
    RSA_free(r);
  }

  // 私钥解密
  if (mode == 2) {
    RSA* r;
    BIO* in = BIO_new_file(prikey, "r");
    r = PEM_read_bio_RSAPrivateKey(in, NULL, NULL, NULL);
    int rlen = RSA_size(r);
    
    unsigned char inbuf[1024] = {0};
    unsigned char outbuf[1024] = {0};
    for (long offset = 0; offset < fsize; offset += rlen) {
      long n = (fsize - offset >= rlen ? rlen : fsize - offset);
      if (fread(inbuf, 1, n, fin) != (size_t)n) {
        perror("fread");
        return -1;
      }
      int len = RSA_private_decrypt(n, inbuf, outbuf, r, padding);
      if (fwrite(outbuf, 1, len, fout) != (size_t)len) {
        perror("fwrite");
        return -1;
      }
      memset(inbuf, 0, sizeof(inbuf));
      memset(outbuf, 0, sizeof(outbuf));
    }

    BIO_free(in);
    RSA_free(r);
  }

  printf("===== test success =====\n");

  return 0;
}