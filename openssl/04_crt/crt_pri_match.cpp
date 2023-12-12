
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <string>
#include <string.h>
#include <stdint.h>

// 功能: 验证私钥是否与证书中的公钥匹配

// 方法:
// 计算私钥模式哈希值:
// $ openssl rsa -modulus -noout -in pri.pem | openssl md5
// 计算证书 modulus 哈希值:
// $ openssl x509 -modulus -noout -in crt.pem  | openssl md5
// 如果两个哈希字符串相同，则表示密钥对匹配。否则，它不是有效的密钥对。

const char* pripem = "pri.pem";
const char* pubpem = "pub.pem";
const char* crtpem = "crt.pem";

std::string hex2bin(const std::string& hex)
{
  std::string bin(hex.size() / 2, '\x0');
  
  char ch, ck;
  int i = 0;
  
  for (const auto & ele : hex) {
    if (ele >= '0' && ele <= '9') ch = ele - '0'; else
    if (ele >= 'A' && ele <= 'F') ch = ele - '7'; else
    if (ele >= 'a' && ele <= 'f') ch = ele - 'W'; else
        return "";
    
    ck = ((i & 1) != 0) ? ch : ch << 4;
    
    bin[i >> 1] = (unsigned char)(bin[i >> 1] | ck);
    i++;
  }
  
  return bin;
}

int main()
{
  {
    // 解析证书，拿到公钥
    BIO *b = BIO_new_file(crtpem, "r");
    X509 *x = PEM_read_bio_X509(b, NULL, NULL, NULL);
    if (x == NULL) {
      // 如果 x 为 NULL，则证书格式不为 X.509 格式
      printf("PEM_read_bio_X509 err!\n");
      return -1;
    }

    EVP_PKEY* pubkey = X509_get_pubkey(x);
    RSA* r = EVP_PKEY_get0_RSA(pubkey);
    // RSA_print_fp(stdout, r, 11);

    const BIGNUM *n;
    RSA_get0_key(r, &n, NULL, NULL);
    BIO *bm = BIO_new(BIO_s_mem());
    BN_print(bm, n);
    int buflen = RSA_size(r) * 2;
    uint8_t* out = (uint8_t *)OPENSSL_malloc(buflen);
    int outlen = BIO_read(bm, out, buflen);

    printf("buflen=%d,outlen=%d\n", buflen, outlen);

    std::string modulus = hex2bin(std::string((const char*)out, outlen));
    
    for (size_t i = 0; i < modulus.size(); i++) {
      printf("0x%02X ", (int)static_cast<uint8_t>(modulus[i]));
      if ((i + 1) % 16 == 0) {
        printf("\n");
      }
    }

    OPENSSL_free(out);
    BIO_free(bm);
    BIO_free(b);
    X509_free(x);

    // 解析公钥
    {
      BIO* in = BIO_new_file(pubpem, "r");
      RSA* r = PEM_read_bio_RSAPublicKey(in, NULL, NULL, NULL);
      RSA_print_fp(stdout, r, 11);

      const BIGNUM *n;
      RSA_get0_key(r, &n, NULL, NULL);
      BIO *bm = BIO_new(BIO_s_mem());
      BN_print(bm, n);
      int buflen = RSA_size(r) * 2;
      uint8_t* out = (uint8_t *)OPENSSL_malloc(buflen);
      int outlen = BIO_read(bm, out, buflen);

      printf("buflen=%d,outlen=%d\n", buflen, outlen);

      std::string modulus = hex2bin(std::string((const char*)out, outlen));

      for (size_t i = 0; i < modulus.size(); i++) {
        printf("0x%02X ", (int)static_cast<uint8_t>(modulus[i]));
        if ((i + 1) % 16 == 0) {
          printf("\n");
        }
      }

      OPENSSL_free(out);
      BIO_free(bm);     
      BIO_free(in);
      RSA_free(r);
    }
  }
  
  printf("===== test success =====\n");

  return 0;
}