
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"
#include "spdlog/sinks/basic_file_sink.h"

int main()
{
    spdlog::cfg::load_env_levels();

    auto logger = spdlog::basic_logger_mt("file_logger", "my.log");

    // %t 线程号
    // %z 时区      [%H:%M:%S %z]
    // %e 毫秒
    // %f 微秒
    // %F 纳秒
    // %T 等同于 %H:%M:%S
    // %n 这里是 file_logger
    // %L 日志等级的字符串大写首字母，如 Info 中的 'I'
    // %l 日志等级的字符串全称，如 'info'
    logger->set_pattern("[%Y-%m-%d %T.%f][%L][%s:%#] %v");
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::info);
    logger->trace("Should not be flushed");

    logger->info("Test message {}", 1);
    logger->info("Test message {}", 2);

    return 0;
}