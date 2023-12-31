
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

    // 当遇到 info 消息级别以上的立刻刷新到日志，也可设置为遇到 warning 级别及其以上的立刻刷新
    logger->flush_on(spdlog::level::info);
    SPDLOG_LOGGER_TRACE(logger, "Should not be flushed");

    SPDLOG_LOGGER_INFO(logger, "Test message {}", 1);
    SPDLOG_LOGGER_INFO(logger, "Test message {}", 2);

    SPDLOG_LOGGER_DEBUG(logger, "test3 {}", 3); // 会输出文件名和行号

    return 0;
}