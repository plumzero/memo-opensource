
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"

int main()
{
    spdlog::cfg::load_env_levels();

    // Customize msg format for all loggers
    spdlog::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
    spdlog::info("This an info message with custom format");
    spdlog::set_pattern("%+"); // back to default format
    spdlog::set_level(spdlog::level::info);

    return 0;
}