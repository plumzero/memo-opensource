
#include "motohawk.h"
#include <stdio.h>

int main()
{
  {
    // 简单使用
    struct motohawk_example_message_t unpacked;
    uint8_t packed[8] = {0};

    unpacked.temperature = 55;
    unpacked.average_radius = 32;
    unpacked.enable = 1;

    int ret;
    // pack
    ret = motohawk_example_message_pack(packed, &unpacked, sizeof(packed));
    if (ret < 0) {
      printf("motohawk_example_message_pack: %d\n", ret);
      return -1;
    }
    printf("pack 后内容: ");
    for (size_t i = 0; i < sizeof(packed); i++) {
      printf("%02X ", (int)packed[i]);
    }
    printf("\n");
    // unpack
    struct motohawk_example_message_t message;
    ret = motohawk_example_message_unpack(&message, packed, sizeof(packed));
    if (ret != 0) {
      printf("motohawk_example_message_unpack: %d\n", ret);
      return -1;
    }
    printf("unpack 后字段: ");
    printf("temperature=%d,average_radius=%d,enable=%d\n", message.temperature, message.average_radius, message.enable);
    // 检验
    printf("temperature(%d) checked: %s\n", (int)unpacked.temperature, motohawk_example_message_temperature_is_in_range(unpacked.temperature) ? "true" : "false");
    printf("average_radius(%d) checked: %s\n", (int)unpacked.average_radius, motohawk_example_message_temperature_is_in_range(unpacked.average_radius) ? "true" : "false");
    printf("enable(%d) checked: %s\n", (int)unpacked.enable, motohawk_example_message_temperature_is_in_range(unpacked.enable) ? "true" : "false");
  }

  {
    // 检验 %s\n", motohawk_example_message
    printf("\n检验\n");
    printf("temperature(-2049) checked: %s\n", motohawk_example_message_temperature_is_in_range(-2049) ? "true" : "false");
    printf("temperature(-2048) checked: %s\n", motohawk_example_message_temperature_is_in_range(-2048) ? "true" : "false");
    printf("temperature(2047) checked: %s\n", motohawk_example_message_temperature_is_in_range(2047) ? "true" : "false");
    printf("temperature(2048) checked: %s\n", motohawk_example_message_temperature_is_in_range(2048) ? "true" : "false");

    printf("average_radius(-1) checked: %s\n", motohawk_example_message_average_radius_is_in_range(-1) ? "true" : "false");
    printf("average_radius(0) checked: %s\n", motohawk_example_message_average_radius_is_in_range(0) ? "true" : "false");
    printf("average_radius(50) checked: %s\n", motohawk_example_message_average_radius_is_in_range(50) ? "true" : "false");
    printf("average_radius(51) checked: %s\n", motohawk_example_message_average_radius_is_in_range(51) ? "true" : "false");

    printf("enable(-1) checked: %s\n", motohawk_example_message_enable_is_in_range(-1) ? "true" : "false");
    printf("enable(0) checked: %s\n", motohawk_example_message_enable_is_in_range(0) ? "true" : "false");
    printf("enable(1) checked: %s\n", motohawk_example_message_enable_is_in_range(1) ? "true" : "false");
    printf("enable(2) checked: %s\n", motohawk_example_message_enable_is_in_range(2) ? "true" : "false");
  }

  {
    // 以信号 Temperature 为例
    //  SG_ Temperature : 0|12@0- (0.01,250) [229.52|270.47] "degK"  PCM1,FOO
    // 上面描述的范围是 [229.52,270.47]，但是在相应的 motohawk_example_message_temperature_is_in_range 校验的范围参数却是 [-2048,2047]
    // 这是怎么回事呢？
    // 以 pack can 数据为例，原来实际的过程是 物理值 -> 信号值 -> can数据。信号值就是 motohawk_example_message_t 中保存的数据，而物理值才是真值。
    // 物理值通过 motohawk_example_message_average_radius_encode 可以转为信号值，信号值再通过 motohawk_example_message_pack 转成 can 数据。
    double temperature = 234.56;    // 在赋值的时候，要注意精度是 0.01，所以这里不能赋值为形如 234.567 的值，否则就超过数值的精度了
    double average_radius = 1.2;
    double enable = 1;

    struct motohawk_example_message_t unpacked;
    uint8_t packed[8] = {0};

    // encode
    unpacked.temperature = motohawk_example_message_temperature_encode(temperature);
    unpacked.average_radius = motohawk_example_message_average_radius_encode(average_radius);
    unpacked.enable = motohawk_example_message_enable_encode(enable);
    printf("\nencoded: temperature=%d,average_radius=%d,enable=%d\n", unpacked.temperature, unpacked.average_radius, unpacked.enable);
    // 校验(事实上，即使校验失败，也可能不影响后续的 pack)
    if (!motohawk_example_message_temperature_is_in_range(unpacked.temperature)) { printf("temperature(%d) checked error\n", unpacked.temperature); }
    if (!motohawk_example_message_average_radius_is_in_range(unpacked.average_radius)) { printf("average_radius(%d) checked error\n", unpacked.average_radius); }
    if (!motohawk_example_message_enable_is_in_range(unpacked.enable)) { printf("enable(%d) checked error\n", unpacked.enable); }
    int ret;
    // pack
    ret = motohawk_example_message_pack(packed, &unpacked, sizeof(packed));
    if (ret < 0) {
      printf("motohawk_example_message_pack: %d\n", ret);
      return -1;
    }
    // unpack
    struct motohawk_example_message_t message;
    ret = motohawk_example_message_unpack(&message, packed, sizeof(packed));
    if (ret != 0) {
      printf("motohawk_example_message_unpack: %d\n", ret);
      return -1;
    }
    double t, a, e;
    t = motohawk_example_message_temperature_decode(message.temperature);
    a = motohawk_example_message_average_radius_decode(message.average_radius);
    e = motohawk_example_message_enable_decode(message.enable);
    printf("decoded: temperature=%.3f,average_radius=%.3f,enable=%.3f\n", t, a, e);
    // 上面一行的打印为 decoded: temperature=234.570,average_radius=1.100,enable=1.000
    // 可以看得出来有一些精度损失。这可以给使用者以启示: 如果你想要保持 0.01 精度，那么在 dbc 文件中最好定义为 0.001 精度。
  }

  printf("===== test over =====\n");

  return 0;
}