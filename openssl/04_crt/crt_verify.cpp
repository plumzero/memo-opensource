
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/x509.h>

// 功能: 证书链验证

// 命令行验证:
// openssl verify -CAfile cert.ca.pem -verbose cert.sub.pem

X509* pem_to_x509(const char* pem)
{
  BIO *b = BIO_new_file(pem, "r");
  X509 *x = PEM_read_bio_X509(b, NULL, NULL, NULL);
  if (x == NULL) {
    printf("PEM_read_bio_X509 err!\n");
    return NULL;
  }

  BIO_free(b);
  
  return x;
}

X509* der_to_x509(const char* der)
{
  FILE* fp = fopen(der, "rb");
  if (!fp)
    return NULL;
  
  unsigned char buf[4096] = {0};
  int len = fread(buf, 1, sizeof(buf), fp);
  fclose(fp);

  unsigned char* p = buf;
  X509* x = d2i_X509(NULL, (const unsigned char **)&p, len);
  if (x == NULL) {
    printf("d2i_X509 err!\n");
    return NULL;
  }

  return x;
}

int main()
{
  X509_STORE* cert_store = X509_STORE_new();
  X509_STORE_CTX* ctx = X509_STORE_CTX_new();

  /* 将上级证书加入存储区 */
  const char* ca_path = "cert.ca.pem";
  X509* ca = pem_to_x509(ca_path);
  int ret = X509_STORE_add_cert(cert_store, ca);
  if (ret != 1) {
    printf("X509_STORE_add_cert err!\n");
    return -1;
  }

  /* 将待检验证书加入存储区 */
  const char* user_path = "cert.sub.pem";
  X509* user = pem_to_x509(user_path);
  STACK_OF(X509) *ca_stack = NULL;
  ret = X509_STORE_CTX_init(ctx, cert_store, user, ca_stack);
  if (ret != 1) {
    printf("X509_STORE_CTX_init err!\n");
    return -1;
  }

  /* 校验 */
  ret = X509_verify_cert(ctx);
  if (ret != 1) {
    printf("X509_verify_cert err!\n");
    return -1;
  }

  X509_free(user);
  X509_free(ca);

  X509_STORE_CTX_cleanup(ctx);
  X509_STORE_CTX_free(ctx);

  X509_STORE_free(cert_store);

  printf("===== test success =====\n");

  return 0;
}
