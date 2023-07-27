
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"

int main()
{
    spdlog::cfg::load_env_levels();

    // Runtime log levels
    spdlog::set_level(spdlog::level::info); // Set global log level to info
    spdlog::debug("This message should not be displayed!");
    spdlog::set_level(spdlog::level::trace); // Set specific logger's log level
    spdlog::debug("This message should be displayed..");

    return 0;
}