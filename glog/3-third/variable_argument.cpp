
#include <glog/logging.h>
// #include <glog/log_severity.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <string>

#define BUFSIZE 512

#define GP(Level, module, section, format, ...)     \
        gprintf(__FILE__, __LINE__, Level, module, section, format, ##__VA_ARGS__)

// 0 INFO = GLOG_INFO
// 1 WARNING = GLOG_WARNING,
// 2 ERROR = GLOG_ERROR
// 3 FATAL = GLOG_FATAL;

void gprintf(const char * file, const int line, const int Level, const std::string& module, const std::string& section, const char * format, ...)
{
    char buf[BUFSIZE] = {0};
    va_list args;
    
    va_start(args, format);
    int len = vsnprintf(buf, BUFSIZE, format, args);
    va_end(args);
    
    google::LogMessage(file, line, Level).stream() << module << section << std::string(buf, len);
}

int main()
{
    google::InitGoogleLogging("cstyle");
    FLAGS_log_dir = "./";
    FLAGS_stderrthreshold = google::WARNING;
    FLAGS_colorlogtostderr = true;
    FLAGS_v = 3;
    
    GP(0, "[Hello]", "[World]", "this is %d %s", 123, "DragonBall");
    
    google::ShutdownGoogleLogging();

    return 0;
}
