
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

// 功能: RSA-SHA256 签名验签

const char* prikey = "pri.pem";
const char* pubkey = "pub.pem";

int main(int argc, char* argv[])
{
  if (argc != 4) {
    printf("%s <s/v> <infile> <sigfile>\n", argv[0]);
    return -1;
  }

  int mode = strncmp(argv[1], "s", 1) == 0 ? 1 :
            (strncmp(argv[1], "v", 1) == 0 ? 2 : -1);
  if (mode == -1) {
    printf("%s <s/v> <infile> <sigfile>\n", argv[0]);
    return -1;
  }

  if (strcmp(argv[2], argv[3]) == 0) {
    printf("in file and sig file must different\n");
    return -1;
  }

  FILE* fin = fopen(argv[2], "rb");
  if (fin == NULL) {
    perror("fopen");
    return -1;
  }

  fseek(fin, 0, SEEK_END);
  long fsize = ftell(fin);
  fseek(fin, 0, SEEK_SET);

  // 计算 SHA256 值
  unsigned char sha256sum[SHA256_DIGEST_LENGTH] = {0};
  {
    // TODO: 
    SHA256_CTX c;
    SHA256_Init(&c);
    unsigned char buf[1024] = {0};
    for (long offset = 0; offset < fsize; offset += 1024) {
      long n = (fsize - offset >= 1024 ? 1024 : fsize - offset);
      if (fread(buf, 1, 1024, fin) != (size_t)n) {
        perror("fread");
        return -1;
      }
      SHA256_Update(&c, buf, n);
    }
    SHA256_Final(sha256sum, &c);
    fseek(fin, 0, SEEK_SET);
  }

  int nid = NID_hmacWithSHA256;

  // 私钥签名
  if (mode == 1) {
    RSA* r;
    BIO* in = BIO_new_file(prikey, "r");
    r = PEM_read_bio_RSAPrivateKey(in, NULL, NULL, NULL);
    
    unsigned char signret[256] = {0};
    unsigned int signlen = 0;
    int ret = RSA_sign(nid, sha256sum, sizeof(sha256sum), signret, &signlen, r);
    if (ret != 1) {
      printf("RSA_sign err!\n");
      return -1;
    }
    FILE* fsig = fopen(argv[3], "wb");
    if (fsig == NULL) {
      perror("fopen");
      return -1;
    }
    if (fwrite(signret, 1, signlen, fsig) != signlen) {
      perror("fwrite");
      return -1;
    }

    fclose(fsig);
    BIO_free(in);
    RSA_free(r);
  }

  // 公钥验签
  if (mode == 2) {
    RSA* r;
    BIO* in = BIO_new_file(pubkey, "r");
    r = PEM_read_bio_RSAPublicKey(in, NULL, NULL, NULL);

    unsigned char signret[256] = {0};
    int signlen = 0;

    FILE* fsig = fopen(argv[3], "rb");
    if (fsig == NULL) {
      perror("fopen");
      return -1;
    }

    signlen = fread(signret, 1, sizeof(signret), fsig);
    if (signlen <= 0) {
      perror("fread");
      return -1;
    }
    
    int ret = RSA_verify(nid, sha256sum, sizeof(sha256sum), signret, (unsigned int)signlen, r);
    if (ret != 1) {
      printf("RSA_verify err!\n");
      return -1;
    }

    fclose(fsig);    
    BIO_free(in);
    RSA_free(r);
  }

  printf("===== test success =====\n");

  return 0;
}