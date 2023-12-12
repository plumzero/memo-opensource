
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <stdio.h>

// 功能: 生成密钥并写入文件

// 查看私钥结构: openssl rsa -text -noout -in pri.pem

const char* prikey = "pri.pem";
const char* pubkey = "pub.pem";

int main()
{
  int bits = 2048;
  RSA* r = RSA_new();
  BIGNUM* e = BN_new();

  BN_set_word(e, RSA_F4);
  int ret = RSA_generate_key_ex(r, bits, e, NULL);
  if (ret != 1) {
    printf("RSA_generate_key_ex err!\n");
    return -1;
  }

  // 私钥写入文件
  {
    BIO* out = BIO_new_file(prikey, "w");
    ret = PEM_write_bio_RSAPrivateKey(out, r, NULL, NULL, 0, NULL, NULL);
    if (ret != 1) {
      printf("PEM_write_bio_RSAPrivateKey err!\n");
      return -1;
    }
    BIO_flush(out);
    BIO_free(out);
  }

  // 公钥写入文件
  {
    BIO* out = BIO_new_file(pubkey, "w");
    ret = PEM_write_bio_RSAPublicKey(out, r); // 也可以使用 PEM_write_bio_RSA_PUBKEY，实际测试时更建议使用 PEM_write_bio_RSAPublicKey
    if (ret != 1) {
      printf("PEM_write_bio_RSAPublicKey err!\n");
      return -1;
    }
    BIO_flush(out);
    BIO_free(out);
  }

  BN_free(e);
  RSA_free(r);

  printf("===== test success =====\n");

  return 0;
}