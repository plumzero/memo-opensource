
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include "md5-x86-asm.h"
#include "md5-arm-asm.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

template<typename HT>
void md5_init(MD5_STATE<HT>* state) {
	state->A = 0x67452301;
	state->B = 0xefcdab89;
	state->C = 0x98badcfe;
	state->D = 0x10325476;
}
#ifdef __AVX512VL__
#include <immintrin.h>
template<>
void md5_init<__m128i>(MD5_STATE<__m128i>* state) {
	state->A = _mm_cvtsi32_si128(0x67452301);
	state->B = _mm_cvtsi32_si128(0xefcdab89);
	state->C = _mm_cvtsi32_si128(0x98badcfe);
	state->D = _mm_cvtsi32_si128(0x10325476);
}
#endif

template<typename HT, void(&fn)(MD5_STATE<HT>*, const void*)>
void md5(MD5_STATE<HT>* state, const void* __restrict__ src, size_t len) {
	md5_init<HT>(state);
	char* __restrict__ _src = (char* __restrict__)src;
	uint64_t totalLen = len << 3; // length in bits
	
	for(; len >= 64; len -= 64) {
		fn(state, _src);
		_src += 64;
	}
	len &= 63;
	
	
	// finalize
	char block[64];
	memcpy(block, _src, len);
	block[len++] = 0x80;
	
	// write this in a loop to avoid duplicating the force-inlined process_block function twice
	for(int iter = (len <= 64-8); iter < 2; iter++) {
		if(iter == 0) {
			memset(block + len, 0, 64-len);
			len = 0;
		} else {
			memset(block + len, 0, 64-8 - len);
			memcpy(block + 64-8, &totalLen, 8);
		}
		
		fn(state, block);
	}
}

inline void md5FileSpeed(const char* filename, uint8_t* result){
	int fd = open(filename, O_RDONLY, 0644);
	if (fd < 0)
		return ;

  long size = lseek(fd, 0, SEEK_END);
  if (size == -1) {
    close(fd);
    return ;
  }

void* start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (start == NULL) {
    close(fd);
    return ;
  }
  close(fd);

  MD5_STATE<uint32_t> hash;
  md5<uint32_t, md5_block_std>(&hash, start, size);

  memcpy(result, &hash, 16);

  munmap(start, size);
}
