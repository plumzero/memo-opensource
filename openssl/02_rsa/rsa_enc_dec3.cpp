
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <stdio.h>
#include <string.h>
#include <string>

// 功能: 加解密字符串

const char* prikey = "pri.pem";
const char* pubkey = "pub.pem";

int main()
{
  int padding = RSA_PKCS1_PADDING;

  // const std::string kStr = "MIICdTCCAV0CAQAwLzEOMAwGA1UEBgwFQ0hJTkExDDAKBgNVBAoMA0JZRDEPMA0GA1UEAwwGU04tVklOMIIBIzANBgkqhkiG9w0BAQEFAAOCARAAMIIBCwKCAQEAqFDFm5BUnAKzZa3jg3TcazhZiyLsw2rLoDRkDIStCkRYj4Ecq3N35desornIj8tW/UY6en1hvxDufsg7tyAykO0aiXpuxtRaMcolsf6xZCpcuaUY67+9icMSkzgrYYOUw/ZtIc71Vd4ZqZBIZAZWEJzbo4RX6SuHLcItTtmkocwQXkaIY/X2O6GFBxRmPlcA9KfubHgbleD9HjS4MlDKXyjGP9n1ncIYzXR16znX1w5aijFIPTziaONfYTWcewD6OnriBKDNdS72uZVm0H0G/8yDlcJoYAXoTTBnURiythLIszh8fsOki/ptwRBwJn+z6HsLfTWjmV5oM4s+Zy95rQIEAAEAAaAAMA0GCSqGSIb3DQEBCwUAA4IBAQAOn1q5G1EPLzmCUr1zoFD19tKRT6UVxd4y82UDYoVPntj3TIEZzSNAP8++psqkK9yBeHZ2rejugvVO7w2PlXzGwoGDXu7oGYPe6nU0Nz6lnAXE2B4b3wCMKMYiDNyYPdAI6ZTQ+Qfz6OYvd2wGgoWb8G0RhpHPYbwa+N/JdwqJybOMSeMOKWWwEDy+NK9M4OP6powRy8Zwch84zVLiJ53sTl04mmrh5dsbj7Ecxa4EiMwdzgvJwQ9Bj0IStcNERolFvj9te67qGTjSNNruVXS/DpgcpWbyVCIRyDWfD0yHz/cmzEGtmimNLLvaWbmlXRBEuuZLVBbT6qiSTzyCDB1n";
  const std::string kStr = "MIIC5TCCAc0CFEkQ9OxsWlD2UXxJHvmSNkwAfVkeMA0GCSqGSIb3DQEBCwUAMC8xDzANBgNVBAMMBlNOLVZJTjEMMAoGA1UECgwDQllEMQ4wDAYDVQQGDAVDSElOQTAeFw0yMzExMTcwOTAwMTNaFw0y";
  std::string cipher;

  // 公钥加密
  {
    RSA* r;
    BIO* in = BIO_new_file(pubkey, "r");
    r = PEM_read_bio_RSAPublicKey(in, NULL, NULL, NULL);
    int rlen = RSA_size(r);
    rlen -= 11;

    long ssize = kStr.size();

    unsigned char outbuf[1024] = {0};
    for (long offset = 0; offset < ssize; offset += rlen) {
      long n = (ssize - offset >= rlen ? rlen : ssize - offset);
      int len = RSA_public_encrypt(n, (const unsigned char*)kStr.data() + offset, outbuf, r, padding);
      if (len <= 0) {
        printf("RSA_public_encrypt(RSA_NO_PADDING) err!\n");
        return -1;
      }
      cipher.append(std::string((const char*)outbuf, len));
      memset(outbuf, 0, sizeof(outbuf));
    }
    
    BIO_free(in);
    RSA_free(r);
  }

  std::string plain;

  // 私钥解密
  {
    RSA* r;
    BIO* in = BIO_new_file(prikey, "r");
    r = PEM_read_bio_RSAPrivateKey(in, NULL, NULL, NULL);
    int rlen = RSA_size(r);
    
    long ssize = cipher.size();

    unsigned char outbuf[1024] = {0};
    for (long offset = 0; offset < ssize; offset += rlen) {
      long n = (ssize - offset >= rlen ? rlen : ssize - offset);
      int len = RSA_private_decrypt(n, (const unsigned char*)cipher.data() + offset, outbuf, r, padding);
      if (len <= 0) {
        printf("RSA_private_decrypt(RSA_NO_PADDING) err!\n");
        return -1;
      }
      plain.append(std::string((const char*)outbuf, len));
      memset(outbuf, 0, sizeof(outbuf));
    }

    BIO_free(in);
    RSA_free(r);
  }

  if (kStr.size() != plain.size()) {
    return -1;
  }

  for (size_t i = 0; i < plain.size(); i++) {
    if (plain[i] ^ kStr[i]) {
      return -1;
    }
  }

  printf("===== test success =====\n");

  return 0;
}