
#include <stdint.h>
#include <string.h>
#include <vector>
#include <openssl/hmac.h>

std::vector<uint8_t> sha256_hmac(const std::vector<uint8_t>& key, const std::vector<uint8_t>& in)
{
  HMAC_CTX* ctx = HMAC_CTX_new();
  HMAC_Init_ex(ctx, key.data(), key.size(), EVP_sha256(), NULL);
  HMAC_Update(ctx, in.data(), in.size());
  unsigned int len = 0;
  unsigned char digest[1024];
  HMAC_Final(ctx, digest, &len);
  HMAC_CTX_free(ctx);
  std::vector<uint8_t> vec(len);
  memcpy(vec.data(), digest, len);
  return vec;
}

int main()
{
  return 0;
}