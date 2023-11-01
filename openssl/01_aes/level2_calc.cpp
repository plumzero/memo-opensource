
#include <openssl/aes.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Level2 安全解锁
// 加密算法: AES/CBC/NoPadding
// 安全认证key = AES + ECUKEY + (ECU序列号后 16 个字节左移 3 位)
//  ECU序列号: 通过以太网或者 CAN 的 $22 服务读取
//  ECUKEY: 软件测试开发部分配(16字节) 
// 秘钥 = AES 算法 + 随机数(seed) + 安全认证key

int main(int argc, char* argv[])
{
  (void) argc;
  (void) argv;

  unsigned char authkey[16] = {0};

  // 步骤一: 安全认证 key 的计算
  // ecukey 作密钥, iv 作向量, ecusn 作输入明文
  {
    unsigned char iv[16] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };
    unsigned char ecukey[16] = { 0x64, 0x1d, 0xa0, 0x0d, 0x7f, 0xb1, 0x4b, 0x51, 0x8d, 0xa4, 0x2b, 0xf4, 0xef, 0x0c, 0x24, 0xf4 };
    unsigned char ecusn[16] = { 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88 };
    unsigned char cmp[16] = { 0xAA, 0x7C, 0xE1, 0xB5, 0xD9, 0xD2, 0x7C, 0x4B, 0x89, 0xD6, 0x32, 0x10, 0xA8, 0x05, 0xCC, 0x07 };

    assert(sizeof(ecusn) == 16);

    AES_KEY key_ctx;
    int ret = AES_set_encrypt_key(ecukey, 128, &key_ctx);
    if (ret < 0) {
      printf("AES_set_encrypt_key failed\n");
      return -1;
    }

    AES_cbc_encrypt(ecusn, authkey, sizeof(ecusn), &key_ctx, iv, AES_ENCRYPT);

    for (int i = 0; i < 16; i++) {
      if (cmp[i] ^ authkey[i]) {
        printf("error occured!!!\n");
        return -1;
      }
    }
  }

  // 步骤二: 秘钥的计算
  // 安全认证 key 作密钥，iv 作向量，随机数作输入明文
  {
    unsigned char iv[16] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x00, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };
    unsigned char ra[16] = { 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 };
    unsigned char cmp[16] = { 0xAC, 0x7D, 0x36, 0xF1, 0x88, 0x1C, 0x79, 0xE5, 0x4C, 0x80, 0xE3, 0xBB, 0x87, 0x3D, 0x1D, 0x50 };
    unsigned char secretkey[16] = {0};

    assert(sizeof(ra) == 16);

    AES_KEY key_ctx;
    int ret = AES_set_encrypt_key(authkey, 128, &key_ctx);
    if (ret < 0) {
      printf("AES_set_encrypt_key failed\n");
      return -1;
    }

    AES_cbc_encrypt(ra, secretkey, sizeof(ra), &key_ctx, iv, AES_ENCRYPT);

    for (int i = 0; i < 16; i++) {
      if (cmp[i] ^ secretkey[i]) {
        printf("error occured!!!\n");
        return -1;
      }
    }
  }

  printf("=== test success ===\n");

  return 0;
}
