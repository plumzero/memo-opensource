
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <string>
#include <string.h>

// 功能: 解析证书

// 生成用于测试的证书: 
//  openssl x509 -req -days 365 -in csr.pem -signkey pri.pem -out crt.pem
//  openssl x509 -req -days 365 -in csr.pem -signkey pri.pem -outform DER -out crt.der

// 查看证书信息(用于验证):
//  openssl x509 -text -in crt.pem -noout
//  openssl x509 -text -in crt.der -noout -inform DER

// 查看公钥结构(用于验证):
//  openssl rsa -pubin -text -noout -RSAPublicKey_in -in pub.pem

// 参考
// https://blog.csdn.net/liuxiaoxiaocsdn/article/details/78982976#:~:text=1.7%20X.509%E8%AF%81%E4%B9%A6%E7%9A%84%E7%AD%BE%E5%90%8D%E9%AA%8C%E8%AF%81%20%E7%AD%BE%E5%90%8D%E7%AE%97%E6%B3%95%E4%B8%BA%EF%BC%9ASHA1%20with%20RSA%3A%20SHA1%20%E5%8D%B3%E5%AE%89%E5%85%A8%E5%93%88%E5%B8%8C%E7%AE%97%E6%B3%95%EF%BC%88%20Secure,Hash%20Algorithm%EF%BC%89%EF%BC%8C%E7%94%A8%E4%BA%8E%E7%AD%BE%E5%90%8D%EF%BC%9BRSA%E6%98%AF%E7%9B%AE%E5%89%8D%E6%9C%80%E6%9C%89%E5%BD%B1%E5%93%8D%E5%8A%9B%E7%9A%84%E5%85%AC%E9%92%A5%20%28%E9%9D%9E%E5%AF%B9%E7%A7%B0%29%E5%8A%A0%E5%AF%86%E7%AE%97%E6%B3%95%E3%80%82%20SHA1%20with%20RSA%20%E5%B0%B1%E6%98%AF%E5%8F%91%E9%80%81%E7%AB%AF%E7%94%A8Sha1%20%E8%BF%9B%E8%A1%8C%E7%AD%BE%E5%90%8D%EF%BC%8C%E7%84%B6%E5%90%8E%E7%94%A8RSA%E7%AE%97%E6%B3%95%E5%AF%B9%E7%AD%BE%E5%90%8D%E5%80%BC%E5%8A%A0%E5%AF%86%EF%BC%88%E7%A7%81%E9%92%A5%E5%8A%A0%E5%AF%86%EF%BC%89%E3%80%82
// https://blog.csdn.net/ayang1986/article/details/81025611

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

bool check_validity(X509* cert)
{
  if (cert == NULL)
    return false;
  ASN1_TIME* not_before = X509_get_notBefore(cert);
  ASN1_TIME* not_after = X509_get_notAfter(cert);
  int day, sec;
  if (!ASN1_TIME_diff(&day, &sec, NULL, not_before)) {
    printf("ASN1_TIME_diff err!\n");
    return false;
  }
  if (day > 0 || sec > 0) {
      return false;
  }
  if (!ASN1_TIME_diff(&day, &sec, NULL, not_after)) {
    printf("ASN1_TIME_diff err!\n");
    return false;
  }
  if (day < 0 || sec < 0) {
    return false;
  }
  return true;
}

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
  // pem 格式解析
  {
    BIO *b = BIO_new_file(crtpem, "r");
    X509 *x = PEM_read_bio_X509(b, NULL, NULL, NULL);
    if (x == NULL) {
      // 如果 x 为 NULL，则证书格式不为 X.509 格式
      printf("PEM_read_bio_X509 err!\n");
      return -1;
    }

    // 证书版本
    // 0代表V1；1代表V2；2代表V3。目前绝大多数证书都是V3版本
    BIO_set_fp(b, stdout, BIO_NOCLOSE);
    X509_print(b, x);

    // 版本信息
    int version = X509_get_version(x);
    printf("Version=%d\n", version);

    // 当前日期位于证书起始日期 not_before 和 not_after 之间
    if (!check_validity(x)) {
      printf("not validity!\n");
      return -1;
    }
    // 证书签名算法
    int nid = X509_get_signature_nid(x);
    printf("Signature Algorithm=%d\n", nid);  // obj_mac.h

    // 获取公钥，密钥长度 >= 2048bit
    EVP_PKEY* pubkey = X509_get_pubkey(x);
    RSA* r = EVP_PKEY_get0_RSA(pubkey);
    RSA_print_fp(stdout, r, 11);
    int rlen = RSA_size(r);
    printf("rlen=%d\n", rlen * 8);

    // CommonName
    X509_NAME* xn = X509_get_issuer_name(x);
    if (xn == NULL) {
      printf("X509_get_issuer_name err!\n");
      return -1;
    }
    char buf[1024] = {0};
    int len = X509_NAME_get_text_by_NID(xn, NID_commonName, buf, sizeof(buf));
    if (len == -1) {
      printf("X509_NAME_get_text_by_NID err!\n");
      return -1;
    }
    printf("CommonName=%s\n", buf);

    // 序列号
    {
      BIO* bm = BIO_new(BIO_s_mem());
      ASN1_INTEGER *sn = X509_get_serialNumber(x);
      int ret = i2a_ASN1_INTEGER(bm, sn);
      if (ret == -1) {
        printf("i2a_ASN1_INTEGER err!\n");
        return -1;
      }
      char* out = (char*)OPENSSL_malloc(1024);
      int len = BIO_read(bm, out, 1024);
      std::string hex = std::string(out, out + len);
      std::string serial = hex2bin(hex);
      for (size_t i = 0; i < serial.size(); i++) {
        printf("%02X ", (int)static_cast<uint8_t>(serial[i]));
      }
      printf("\n");

      OPENSSL_free(out);
      BIO_free(bm);
    }

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