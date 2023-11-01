
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <stdio.h>

// 功能: 从文件中加载密钥

const char* prikey = "pri.pem";
const char* pubkey = "pub.pem";

int main()
{
  // 加载私钥
  {
    RSA* r;
    BIO* in = BIO_new_file(prikey, "r");
    r = PEM_read_bio_RSAPrivateKey(in, NULL, NULL, NULL);

    // 打印密钥信息
    RSA_print_fp(stdout, r, 11);
    
    BIO_free(in);
    RSA_free(r);
  }

  // 加载公钥
  {
    RSA* r;
    BIO* in = BIO_new_file(pubkey, "r");
    r = PEM_read_bio_RSAPublicKey(in, NULL, NULL, NULL);

    // 打印密钥信息
    RSA_print_fp(stdout, r, 11);
    
    BIO_free(in);
    RSA_free(r);
  }

  printf("===== test success =====\n");

  return 0;
}