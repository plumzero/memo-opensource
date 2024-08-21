
#include "hawkmoto.h"
#include <stdio.h>
#include <string.h>
#include <bitset>

/*
  BO_ 496 ExampleMessage: 8 PCM1
   SG_ Temperature : 0|12@1- (0.01,250) [229.52|270.47] "degK"  PCM1,FOO
   SG_ AverageRadius : 12|6@1+ (0.1,0) [0|5] "m" Vector__XXX
   SG_ Enable : 18|1@1+ (1,0) [0|0] "-" Vector__XXX

  上面的示例中，使用的是 Intel 格式。这种格式是从低字节低位起始。

  接下来分析是如何对信号值进行 pack 的。

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
  横轴从右向左，比特位从低到高。
  纵轴从上到下，字节地址从低到高。

  上面一共 8 个字节，共 64 个比特位，如果对每个比特位进行索引(从 0 开始)，则范围是 [0,64) 。
  这里为了描述方便，以 (横轴点坐标，纵轴点坐标) 代表一个比特点位(简称点位)。比如最左上角的点位为 (0,7)，最左下角的点位为 (7,7) 。

  用于 pack 示例的信号值(十进制)及对应二进制展示如下:
    .temperature = 55;      00110111
    .average_radius = 32;   0010 0000
    .enable = 1;            00000001

  Intel 格式进行 pack:
  Temperature 信号从索引为 0 的位置开始，长度为 12 个比特位。
  (0,0) 是起始点位，它作为这个信号值的最低位时，第一个字节中仍有比它更高的比特位。
  所以 Temperature 最终所占空间点位如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 | x | x | x | x | x | x | x | x |
    +---+---+---+---+---+---+---+---+
  1 |   |   |   |   | x | x | x | x |
    +---+---+---+---+---+---+---+---+
  2 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |
  Temperature 信号大于 8 但小于 16，所以可以使用 uint16_t 类型表示。
  uint16_t 一共有 16 个比特位，但是在这里只可能是低 12 位有值，而高 4 位(值为 0)可以不用考虑。
  因为 Intel 格式采用小端传输，所以要将它的低位放在低地址上，高位放在高地址上。这里也就是将低 8 位放在第一个字节中，高 4 位放在第 2 个字节中。
   将 temperature 不作任何偏移，与 0xff 进行掩码计算后，得到信号值中的低 8 位，依次填入 (0,7),(0,6),(0,5),(0,4),(0,3),(0,2),(0,1),(0,0)
   将 temperature 向右偏移 8 个比特位，与 0x0f 进行掩码计算后，得到信号值中的高 8 位，依次填入 (1,3),(1,2),(1,1),(1,0)
  Temperature 计算填充后的空间点位如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 1 |
    +---+---+---+---+---+---+---+---+
  1 |   |   |   |   | 0 | 0 | 0 | 0 |
    +---+---+---+---+---+---+---+---+
  2 |   |   |   |   |   |   |   |   |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |

  AverageRadius 信号从索引为 12 的位置开始，长度为 6 个比特位。
  (1, 4) 是起始点位，它作为这个信号值的最低位时，第二个字节仍有比它更高的空闲比特位。AverageRadius 最终所占空间点位如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 1 |
    +---+---+---+---+---+---+---+---+
  1 | x | x | x | x | 0 | 0 | 0 | 0 |
    +---+---+---+---+---+---+---+---+
  2 |   |   |   |   |   |   | x | x |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |

  AverageRadius 信号小于 8，所以可以使用 uint8_t 类型表示。同样 uint8_t 的高 2 位可以不用考虑。
   将 average_radius 向左偏移 4 个比特位，与 0xf0 进行掩码计算后，得到信号值中的低 4 位，得到的值依次填入 (1,7),(1,6),(1,5),(1,4)
   将 average_radius 向右偏移 4 个比特位，与 0x03 进行掩码计算后，得到信号值中的高 2 位，得到的值依次填入 (2,1),(2,0)
  AverageRadius 计算填充后的空间点位如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 1 |
    +---+---+---+---+---+---+---+---+
  1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
    +---+---+---+---+---+---+---+---+
  2 |   |   |   |   |   |   | 1 | 0 |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |

  Enable 信号从索引为 18 的位置开始，长度为 1 个比特位。按照和上面同样的方式，最终 pack 的结果如下:
      7   6   5   4   3   2   1   0
    +---+---+---+---+---+---+---+---+
  0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 1 |
    +---+---+---+---+---+---+---+---+
  1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
    +---+---+---+---+---+---+---+---+
  2 |   |   |   |   |   | 1 | 1 | 0 |
    +---+---+---+---+---+---+---+---+
  3 |   |   |   |   |   |   |   |   |

  cantools 已经在预解析生成的 .c 文件中，为我们生成了执行上述操作的函数和代码，如下:
    temperature = (uint16_t)src_p->temperature;
    dst_p[0] |= pack_left_shift_u16(temperature, 0u, 0xffu);
    dst_p[1] |= pack_right_shift_u16(temperature, 8u, 0x0fu);
    dst_p[1] |= pack_left_shift_u8(src_p->average_radius, 4u, 0xf0u);
    dst_p[2] |= pack_right_shift_u8(src_p->average_radius, 4u, 0x03u);
    dst_p[2] |= pack_left_shift_u8(src_p->enable, 2u, 0x04u);

  unpack 的方式与之相反。
*/

int main()
{
  {
    struct hawkmoto_example_message_t unpacked;
    uint8_t packed[8] = {0};

    unpacked.temperature = 55;
    unpacked.average_radius = 32;
    unpacked.enable = 1;

    printf("0x%02X: %s\n", (int)unpacked.temperature, std::bitset<8>(unpacked.temperature).to_string().c_str());
    printf("0x%02X: %s\n", (int)unpacked.average_radius, std::bitset<8>(unpacked.average_radius).to_string().c_str());
    printf("0x%02X: %s\n\n", (int)unpacked.enable, std::bitset<8>(unpacked.enable).to_string().c_str());

    int ret;
    // pack
    ret = hawkmoto_example_message_pack(packed, &unpacked, sizeof(packed));
    if (ret < 0) {
      printf("hawkmoto_example_message_pack: %d\n", ret);
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