
#include <glog/logging.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>

#include <sys/time.h>

#define COUNT_FOR_TEST 1000000

uint64_t now_time()
{
    struct timeval tv;

    gettimeofday(&tv, nullptr);
    
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static inline uint64_t
random64(void)
{
    uint64_t val;

    val = (uint64_t)lrand48();
    val <<= 32;
    val += (uint64_t)lrand48();

    return val;
}

int main()
{
    {
        google::InitGoogleLogging("cppstyle");
        FLAGS_log_dir = "./";
        FLAGS_stderrthreshold = google::WARNING;
        FLAGS_colorlogtostderr = true;
        FLAGS_v = 3;
        
        double pi = 3.1415926;
        
        int gold100 = 618;
        
        const std::string str = "Hello World";
        
        uint64_t b, e;
        
        size_t i;
        
        b = now_time();
        for (i = 0; i < COUNT_FOR_TEST; i++) {
            LOG(INFO) << "test type(uint64_t, double, int, std::string):uint64_t=" +
                         std::to_string((uint64_t)random64()) +
                         ",double=" + std::to_string(pi) +
                         ",int=" + std::to_string(gold100) +
                         ",str=" + str;
        }
        e = now_time();
        
        LOG(INFO) << "total spend " << std::to_string(e - b) << " milliseconds";
        
        google::ShutdownGoogleLogging();
    }
    
    printf("sleep for 10 seconds...\n");
    sleep(10);
    
    {
        google::InitGoogleLogging("cstyle");
        FLAGS_log_dir = "./";
        FLAGS_stderrthreshold = google::WARNING;
        FLAGS_colorlogtostderr = true;
        FLAGS_v = 3;
        
        double pi = 3.1415926;
        
        int gold100 = 618;
        
        const std::string str = "Hello World";
        
        uint64_t b, e;
        
        size_t i;
        
        
        b = now_time();
        for (i = 0; i < COUNT_FOR_TEST; i++) {
            char buf[1024];
            int bytes;
            bytes = snprintf(buf, 1024, "test type(uint64_t, double, int, std::string):uint64_t=%lu,double=%f,int=%d,str=%s",
                                        random64(), pi, gold100, str.c_str());
            LOG(INFO) << std::string(buf, buf + bytes);
        }
        e = now_time();
        
        LOG(INFO) << "total spend " << std::to_string(e - b) << " milliseconds";
        
        google::ShutdownGoogleLogging();
    }
    
    return 0;
}
