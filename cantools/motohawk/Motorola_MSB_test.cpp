
#include "motohawk.h"
#include <stdio.h>
#include <string.h>
#include <bitset>

/*
  BO_ 496 ExampleMessage: 8 PCM1
   SG_ Temperature : 0|12@0- (0.01,250) [229.52|270.47] "degK"  PCM1,FOO
   SG_ AverageRadius : 6|6@0+ (0.1,0) [0|5] "m" Vector__XXX
   SG_ Enable : 7|1@0+ (1,0) [0|0] "-" Vector__XXX

  @ 后的 0 表示是 Motorola 格式(大端传输)，如果是 1 表示是 Intel 格式(小端传输)，+表示是无符号数据，-表示是有符号数据。符号不影响格式。

  两种格式区分如下:
   Intel 格式: 低字节低位起始
   Motorola_LSB: 高字节低位起始
   Motorola_MSB: 低字节高位起始

  上面的示例中，使用的是 Motorola_MSB 格式。接下来分析在这种格式下是如何对信号值进行 pack 的。

  首先要清楚字节和比特位的排布，以 CAN 报文(长度为 8 个字节)为例，如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  1 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  2 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  4 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  5 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  6 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  7 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  横轴从右到左，比特位从低变高。
  纵轴从上到下，字节地址从低变高。

  上面一共 8 个字节，共 64 个比特位，如果对每个比特位进行索引(从 0 开始，从右到左，从上到下)，则范围是 [0,64) 。
  这里为了描述方便，以 (横轴点坐标，纵轴点坐标) 代表一个比特点位(简称点位)。比如最左上角的点位为 (0,7)，最左下角的点位为 (7,7) 。

  用于 pack 示例的信号值(十进制)及对应二进制展示如下:
    .temperature = 55;      00110111
    .average_radius = 32;   00100000
    .enable = 1;            00000001

  Motorola_MSB 格式进行 pack:
  Temperature 信号从索引为 0 的位置开始，长度为 12 个比特位。
  (0,0) 是起始点位，当它作为这个信号值的最高位时，第一个字节已经没有比它更低的比特位，那就继续在下一个字节中的比特位寻找(从左到右)。
  所以 Temperature 最终所占空间点位如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 |   |   |   |   |   |   |   | x |
    +---+---+---+---+---+---+---+---+
  1 | x | x | x | x | x | x | x | x |
    +---+---+---+---+---+---+---+---+
  2 | x | x | x |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |
  Temperature 信号大于 8 但小于 16，有符号，可以使用 int16_t 类型表示。
  int16_t 一共有 16 个比特位，但是在这里只可能用到它的低 12 位，而高 4 位不用考虑。
   将 temperature 向右偏移 11 个比特位，与 0x01 进行掩码计算后，得到信号值中的高 1 位，填入 (0,0) 点位
   将 temperature 向右偏移 3 个比特位，与 0xff 进行掩码计算后，得到信号值中的中 8 位，依次填入 (1,7),(1,6),(1,5),(1,4),(1,3),(1,2),(1,1),(1,0)
   将 temperature 向左偏移 5 个比特位，与 0xe0 进行掩码计算后，得到信号值中的低 3 位，依次填入 (2,7),(2,6),(2,5)
  Temperature 计算填充后的空间点位如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 |   |   |   |   |   |   |   | 0 |
    +---+---+---+---+---+---+---+---+
  1 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 |
    +---+---+---+---+---+---+---+---+
  2 | 1 | 1 | 1 |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |

  AverageRadius 信号从索引为 6 的位置开始，长度为 6 个比特位。
  (0，6) 是起始点位，它作为这个信号值的最高位时，第一个字节仍有比它更低的空闲比特位。AverageRadius 最终所占空间点位如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 |   | x | x | x | x | x | x | 0 |
    +---+---+---+---+---+---+---+---+
  1 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 |
    +---+---+---+---+---+---+---+---+
  2 | 1 | 1 | 1 |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |
  AverageRadius 信号小于 8，无符号，可以使用 uint8_t 类型表示。同样 uint8_t 的高 2 位可以不用考虑。
   将 average_radius 向左偏移 1 个比特位(因为(0,0)点位已经被占用了)，与 0x7e 进行掩码计算后，得到的值依次填入 (0,6),(0,5),(0,4),(0,3),(0,2),(0,1)
  AverageRadius 计算填充后的空间点位如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 |   | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
    +---+---+---+---+---+---+---+---+
  1 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 |
    +---+---+---+---+---+---+---+---+
  2 | 1 | 1 | 1 |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |

  Enable 信号从索引为 7 的位置开始，长度为 1 个比特位。按照和上面同样的方式，最终 pack 的结果如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
    +---+---+---+---+---+---+---+---+
  1 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 |
    +---+---+---+---+---+---+---+---+
  2 | 1 | 1 | 1 |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |

  cantools 已经在预解析生成的 .c 文件中，为我们生成了执行上述操作的函数和代码，如下:
    dst_p[0] |= pack_left_shift_u8(src_p->enable, 7u, 0x80u);
    dst_p[0] |= pack_left_shift_u8(src_p->average_radius, 1u, 0x7eu);
    temperature = (uint16_t)src_p->temperature;
    dst_p[0] |= pack_right_shift_u16(temperature, 11u, 0x01u);
    dst_p[1] |= pack_right_shift_u16(temperature, 3u, 0xffu);
    dst_p[2] |= pack_left_shift_u16(temperature, 5u, 0xe0u);

  unpack 的方式与之相反。
*/

int main()
{
  {
    struct motohawk_example_message_t unpacked;
    uint8_t packed[8] = {0};

    unpacked.temperature = 55;
    unpacked.average_radius = 32;
    unpacked.enable = 1;

    printf("0x%02X: %s\n", (int)unpacked.temperature, std::bitset<8>(unpacked.temperature).to_string().c_str());
    printf("0x%02X: %s\n", (int)unpacked.average_radius, std::bitset<8>(unpacked.average_radius).to_string().c_str());
    printf("0x%02X: %s\n\n", (int)unpacked.enable, std::bitset<8>(unpacked.enable).to_string().c_str());

    int ret;
    // pack
    ret = motohawk_example_message_pack(packed, &unpacked, sizeof(packed));
    if (ret < 0) {
      printf("motohawk_example_message_pack: %d\n", ret);
      return -1;
    }
    for (size_t i = 0; i < sizeof(packed); i++) {
      printf("0x%02X: %s\n", (int)packed[i], std::bitset<8>(packed[i]).to_string().c_str());
    }
    printf("\n");

    // bitset
    uint64_t u64;
    memcpy(&u64, packed, 8);
    std::bitset<sizeof(packed) * 8> foo(u64);
    printf("%08lX: %s\n", u64, foo.to_string().c_str());
  }

  printf("===== test over =====\n");
  
  return 0;
}