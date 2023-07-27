
#include <glog/logging.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>

#include <time.h>
#include <sys/time.h>

void init_log(const char * name)
{
    google::InitGoogleLogging(name);
    FLAGS_log_dir = "./";
    FLAGS_colorlogtostderr = true;
    FLAGS_logbufsecs = 10;
    FLAGS_max_log_size = 3600;
    FLAGS_stop_logging_if_full_disk = true;
    google::SetStderrLogging(google::GLOG_ERROR);
}

int main(int argc, char **argv)
{
    init_log(std::string("TEST").c_str());

    int count = 100000;

    while (1) {
        LOG(INFO) << "Returns the character attribute for the given color.";
    }

    google::ShutdownGoogleLogging();

    return 0;
}
