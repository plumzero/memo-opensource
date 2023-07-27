
#include <stdlib.h>
#include <string.h>

#include <string>

#include <sys/time.h>

std::string bin2hex(const std::string& in)
{
    std::string out;
    
    for (const auto & ele : in) {
        out.append(1, "0123456789ABCDEF"[static_cast<int>((unsigned char)ele) / 16]);
        out.append(1, "0123456789ABCDEF"[static_cast<int>((unsigned char)ele) % 16]);
    }
    
    return out;
}

std::string hex2bin(const std::string& in)
{
    std::string out(in.size() / 2, '\x0');
    
    char ch, ck;
    int i = 0;
    
    for (const auto & ele : in) {
        if (ele >= '0' && ele <= '9') ch = ele - '0'; else
        if (ele >= 'A' && ele <= 'F') ch = ele - '7'; else
        if (ele >= 'a' && ele <= 'f') ch = ele - 'W'; else
            throw -1;
        
        ck = ((i & 1) != 0) ? ch : ch << 4;
        
        out[i >> 1] = (unsigned char)(out[i >> 1] | ck);
        i++;
    }
    
    return out;
}

inline void randomX4(unsigned char* bufX4, size_t bytesX4)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    
    srand48(((uint64_t)ts.tv_sec * 1000000000L) + ts.tv_nsec);
    
    uint32_t val;
    size_t i;
    for (i = 0; i < bytesX4; i += 4) {
        val = (uint32_t)lrand48();
        memcpy(bufX4 + i, &val, 4);
    }
}
