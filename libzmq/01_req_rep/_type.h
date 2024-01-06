
#pragma once

#include <stdint.h>

typedef struct {
  uint32_t id;
  std::vector<uint8_t> payload;
} Payload_t;
