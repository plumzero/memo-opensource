
#include <glog/logging.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>

#include <string>

#include <sys/time.h>


#define BUFSIZE 512

#define GP(Level, module, section, format, ...)     \
        gprintf(__FILE__, __LINE__, Level, module, section, format, ##__VA_ARGS__)

// 0 INFO = GLOG_INFO
// 1 WARNING = GLOG_WARNING,
// 2 ERROR = GLOG_ERROR
// 3 FATAL = GLOG_FATAL;

inline void gprintf(const char * file, const int line, const int Level, const std::string& module, const std::string& section, const char * format, ...)
{
    char buf[BUFSIZE] = {0};
    va_list args;
    
    va_start(args, format);
    int len = vsnprintf(buf, BUFSIZE, format, args);
    va_end(args);
    
    google::LogMessage(file, line, Level).stream() << module << section << std::string(buf, len);
}

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
        google::InitGoogleLogging("macro");
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
            GP(0, "[Hello]", "[World]", "test type(uint64_t, double, int, std::string):uint64_t=%lu,double=%f,int=%d,str=%s",
                                        random64(), pi, gold100, str.c_str());
        }
        e = now_time();
        
        LOG(INFO) << "total spend " << std::to_string(e - b) << " milliseconds";
        
        google::ShutdownGoogleLogging();
    }
    
    printf("sleep for 5 seconds...\n");
    sleep(5);
    
    {
        google::InitGoogleLogging("origin");
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
            LOG(INFO) << "[Hello]" << "[World]" << "test type(uint64_t, double, int, std::string):uint64_t=" +
                         std::to_string((uint64_t)random64()) +
                         ",double=" + std::to_string(pi) +
                         ",int=" + std::to_string(gold100) +
                         ",str=" + str;
        }
        e = now_time();
        
        LOG(INFO) << "total spend " << std::to_string(e - b) << " milliseconds";
        
        google::ShutdownGoogleLogging();
    }
        
    return 0;
}
