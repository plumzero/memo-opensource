
#include <openssl/pem.h>

// 功能: 解析证书请求文件

// 测试:
//  openssl req -text -in csr.pem -noout -verify

const char* csrpem = "csr.pem";
const char* csrder = "csr.der";

int main()
{
  // pem 格式
  {
    BIO* in = BIO_new_file(csrpem, "rb");
    X509_REQ* req = PEM_read_bio_X509_REQ(in, NULL, NULL, NULL);
    if (req == NULL) {
      printf("PEM_read_bio_X509_REQ err!\n");
      return -1;
    }

    // 打印证书请求信息
    BIO_set_fp(in, stdout, BIO_NOCLOSE);
    X509_REQ_print(in, req);

    BIO_free(in);
    X509_REQ_free(req);
  }

  // der 格式(有问题)
  if (0) {
    FILE* fp = fopen(csrder, "r");
    unsigned char buf[1024] = {0};
    size_t len = fread(buf, 1, sizeof(buf), fp);
    fclose(fp);

    const unsigned char* p = buf;
    X509_REQ** req = (X509_REQ**)malloc(sizeof(X509_REQ*));
    d2i_X509_REQ(req, &p, len);
    if (*req == NULL) {
      printf("d2i_X509_REQ err!\n");
      return -1;
    }

    X509_REQ_free(*req);
    free(req);
  }

  printf("===== test success =====\n");

  return 0;
}