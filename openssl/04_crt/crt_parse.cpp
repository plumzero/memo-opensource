
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <string>

// 功能: 解析证书

// 生成用于测试的证书: 
//  openssl x509 -req -days 365 -in csr.pem -signkey pri.pem -out crt.pem
//  openssl x509 -req -days 365 -in csr.pem -signkey pri.pem -outform DER -out crt.der

// 查看证书信息(用于验证):
//  openssl x509 -text -in crt.pem -noout
//  openssl x509 -text -in crt.der -noout -inform DER

// 查看公钥结构(用于验证):
//  openssl rsa -pubin -text -noout -RSAPublicKey_in -in pub.pem

const char* crtpem = "crt.pem";
const char* crtder = "crt.der";
std::string crtstr = "-----BEGIN CERTIFICATE-----\n"
"MIIDsTCCApmgAwIBAgIUS7YOnVQoHI9fkRQV+eSJSOpk7G8wDQYJKoZIhvcNAQEL"
"BQAwaDELMAkGA1UEBhMCQ04xEjAQBgNVBAgMCUd1YW5nRG9uZzENMAsGA1UEBwwE"
"QXV0bzEQMA4GA1UECgwHQ29tcGFueTETMBEGA1UECwwKRGVwYXJ0bWVudDEPMA0G"
"A1UEAwwGQ2FDZXJ0MB4XDTIzMDUyNTA5MzI1NVoXDTMzMDUyMjA5MzI1NVowaDEL"
"MAkGA1UEBhMCQ04xEjAQBgNVBAgMCUd1YW5nRG9uZzENMAsGA1UEBwwEQXV0bzEQ"
"MA4GA1UECgwHQ29tcGFueTETMBEGA1UECwwKRGVwYXJ0bWVudDEPMA0GA1UEAwwG"
"Q2FDZXJ0MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA0KYLkcyqtjfL"
"4JGBSYcD9CzOi+NCcAvKgeek7+Zzxch6P/Xl4t8Ws9Mba+ITlXDCpeLc2n+hdiO0"
"xXoViLk7/JdvK4pWoeg5VI/d9mW2guIGZUfsP2hctLmoDVHHYbIJWP9ytCzEPjl7"
"dZnejgwjbX4nFFipuap9v7jyGydCjZBC/sVnI8fCSJAhOQwagmHz1W3B0RiTHkJG"
"wNIRkuumW/jnephChKwcIkMCuzeiHDlYlVqdvMMW+YEKfqObdiItTG1G+ujzmHcv"
"1aTlvi332fqXFYRhA1CzAHv/HkJuCuvKnfhRAlY480kFju7df33dVuZSzILqwEYb"
"Q560fShofQIDAQABo1MwUTAdBgNVHQ4EFgQUmc08TIz0Qo9qkv8bmrH0/P5+Rl0w"
"HwYDVR0jBBgwFoAUmc08TIz0Qo9qkv8bmrH0/P5+Rl0wDwYDVR0TAQH/BAUwAwEB"
"/zANBgkqhkiG9w0BAQsFAAOCAQEAN4YMpX2KztUh+QCtzjul8w+d4QqTMxQg7DvC"
"YbWFs0xb/tNx9GKFtgZyzVncll9aZ8irRvcVMWay4f2vg4MXKlRV7vK0/hMvIc6Z"
"trgcNpTGqSjDokbNUoSjByP0ZXPqMZTnrmvzFj6bD4yvSi3/5NFhoJbAgkCZY8iK"
"IS6Mk1zVRlXkRAhM9I8u+Pg1pgRrwyqR5bVf7TOUpG173f7FOgPgaaOim0HCahG6"
"8NaimAE/7G4QatbaHNl/NQMfzskW4Ifp+hrk6TDEf6MupvkU67mul3LgA9oRZUiu"
"dCdjd8jRLZko6AEKSNRj51M85V2TvgsWuJ8THQCLpav11FB4tA==\n"
"-----END CERTIFICATE-----";

int main()
{
  // pem 格式解析
  {
    BIO *b = BIO_new_file(crtpem, "r");
    X509 *x = PEM_read_bio_X509(b, NULL, NULL, NULL);
  
    // 获取公钥
    EVP_PKEY* pubkey = X509_get_pubkey(x);
    RSA* r = EVP_PKEY_get0_RSA(pubkey);
    RSA_print_fp(stdout, r, 11);

    BIO_free(b);
    X509_free(x);
  }

  // der 格式解析
  {
    FILE* fp = fopen(crtder, "rb");
    if (!fp)
      return -1;
    
    unsigned char buf[4096] = {0};
    int len = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);

    unsigned char* p = buf;
    X509* x = d2i_X509(NULL, (const unsigned char **)&p, len);

    BIO* b = BIO_new(BIO_s_file());
    
    // 打印证书信息
    BIO_set_fp(b, stdout, BIO_NOCLOSE);
    // X509_print(b, x);
    
    // 获取公钥
    EVP_PKEY* pubkey = X509_get_pubkey(x);
    RSA* r = EVP_PKEY_get0_RSA(pubkey);
    RSA_print_fp(stdout, r, 11);

    BIO_free(b);
    X509_free(x);
  }

  {
    BIO *b = BIO_new(BIO_s_mem());
    BIO_write(b, crtstr.c_str(), crtstr.size());
    X509 *x = PEM_read_bio_X509(b, NULL, NULL, NULL);
    if (x == NULL) {
      printf("----\n");
    }
    // 获取公钥
    EVP_PKEY* pubkey = X509_get_pubkey(x);
    RSA* r = EVP_PKEY_get0_RSA(pubkey);
    
    RSA_print_fp(stdout, r, 11);

    BIO_free(b);
    X509_free(x);
  }
  
  printf("===== test success =====\n");

  return 0;
}