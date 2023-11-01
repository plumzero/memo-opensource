
#include <string.h>
#include <openssl/x509.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

// 功能: 生成证书请求文件

// 测试:
//  openssl req -text -in csr.pem -noout -verify
//  openssl req -inform PEM -in csr.pem -outform DER -out csr.der
//  openssl req -text -in csr.der -noout -verify -inform DER

const char* prikey = "pri.pem";
const char* pubkey = "pub.pem";
const char* csrpem = "csr.pem";
const char* csrder = "csr.der";

int main(int argc, char* argv[])
{
  (void) argc;

  bool verify = false;
  if (argv[0]) {
    verify = true;
  }

  X509_REQ* req = X509_REQ_new();
  long version = 1;
  int ret = X509_REQ_set_version(req, version);
  X509_NAME* name = X509_NAME_new();
  X509_NAME_ENTRY* entry = NULL;
  const char* common_name = "as-a-test";
  entry = X509_NAME_ENTRY_create_by_txt(&entry, "commonName", V_ASN1_UTF8STRING, (unsigned char *)common_name, strlen(common_name));
  X509_NAME_add_entry(name, entry, 0, -1);
  const char* country_name = "CHINA";
  entry = X509_NAME_ENTRY_create_by_txt(&entry, "countryName", V_ASN1_UTF8STRING, (unsigned char*)country_name, strlen(country_name));
  X509_NAME_add_entry(name, entry, 1, -1);
  /* subject name */
  ret = X509_REQ_set_subject_name(req, name);
  /* pub key */
  RSA* rsa;
  if (verify) {
    BIO* in = BIO_new_file(prikey, "r");
    rsa = PEM_read_bio_RSAPrivateKey(in, NULL, NULL, NULL);
    BIO_free(in);
  } else {
    BIO* in = BIO_new_file(pubkey, "r");
    rsa = PEM_read_bio_RSAPublicKey(in, NULL, NULL, NULL);
    BIO_free(in);
  }
  EVP_PKEY* pkey = EVP_PKEY_new();
  EVP_PKEY_assign_RSA(pkey, rsa);
  ret = X509_REQ_set_pubkey(req, pkey);
  /* attribute */
  const char* attr_1 = "Tech Platform";
  ret = X509_REQ_add1_attr_by_txt(req, "organizationName", V_ASN1_UTF8STRING, (unsigned char*)attr_1, strlen(attr_1));
  const char* attr_2 = "Group One";
  ret = X509_REQ_add1_attr_by_txt(req, "organizationalUnitName", V_ASN1_UTF8STRING, (unsigned char*)attr_2, strlen(attr_2));
  const EVP_MD *md = EVP_sha1();
  unsigned char mdout[20] = {0};
  unsigned int mdlen;
  ret = X509_REQ_digest(req, md, mdout, &mdlen);
  ret = X509_REQ_sign(req, pkey, md);
  if (!ret) {
    printf("X509_REQ_sign err!\n");
    X509_REQ_free(req);
    return -1;
  }
  if (verify) {
    ret = X509_REQ_verify(req, pkey);
    if (ret < 0) {
      printf("X509_REQ_verify err!\n");
    }
  }
  /* 写入文件 PEM 格式 */
  BIO *b = BIO_new_file(csrpem, "w");
  PEM_write_bio_X509_REQ(b, req);
  BIO_free(b);

  /* DER 编码: 写入结果有问题 */
  if (0) {
    int len = i2d_X509_REQ(req, NULL);
    unsigned char* der = (unsigned char*)malloc(len);
    unsigned char* p = der;
    len = i2d_X509_REQ(req, &p);
    FILE* fp = fopen(csrder, "wb");
    fwrite(der, 1, len, fp);
    fclose(fp);
    free(der);
  }

  X509_REQ_free(req);
  EVP_PKEY_free(pkey);

  printf("===== test success =====\n");
  
  return 0;
}
