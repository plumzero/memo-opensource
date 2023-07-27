
#include "DoIPGenericHeaderHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
  // 明确载荷长度与消息长度之间的区别

  PayloadType type = PayloadType::DIAGNOSTICMESSAGE;
  uint32_t payload_len = 12;
  uint32_t msg_len = payload_len + 8;
  unsigned char* header = createGenericHeader(type, payload_len);
  if (header == NULL) {
    printf("createGenericHeader failed\n");
    exit(-1);
  }

  // 1.1 DoIP 通用报头检查
  //  - 检查 DoIP 数据报版本信息
  //  - 检查载荷类型
  //  - 检查载荷长度是否超出 DoIP 协议最大值
  GenericHeaderAction action = parseGenericHeader(header, msg_len);
  printf("action: type=%d,value=%d,payloadLength=%ld\n", action.type, action.value, action.payloadLength);
  if (action.type == type) {
    printf("====== test success ======\n");
  } else {
    printf("xxxxxx test failed xxxxxx\n");
  }
  // 1.2 检查载荷长度是否超出嵌套字可用 memory
  if (action.payloadLength > 0) {
    unsigned char* buf = new unsigned char[action.payloadLength];
    if (buf == NULL) {
      printf("new failed: %s\n", strerror(errno));
      exit(-1);
    }
  }
  // 1.3 


  return 0;
}