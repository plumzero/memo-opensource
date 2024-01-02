
#include "spdlog/spdlog.h"
#include "spdlog/common.h"  // for log_clock
#include "spdlog/sinks/monthly_file_sink.h"
#include <stdio.h>

// 清除 7 个月之前的日志，或者说是仅保留最近 7 个月的日志

std::chrono::time_point<std::chrono::system_clock> before_7_month(std::chrono::time_point<std::chrono::system_clock> tp)
{
  int count = 0;
  while (count != 7) {
    time_t t = std::chrono::system_clock::to_time_t(tp);
    struct tm * date = localtime(&t);
    int last_month = (date->tm_mon - 1 + 12) % 12 + 1;
    int days = 0;
    switch (last_month) {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
        days = 31;
        break;
      case 4:
      case 6:
      case 9:
      case 11:
        days = 30;
        break;
      case 2:
        int year = 1900 + date->tm_year;
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0 && year % 3200 != 0) || year % 172800 == 0) {
          days = 29;
        } else {
          days = 28;
        }
      break;
    }
    tp -= std::chrono::hours(days * 24);
    count++;
  }

  return tp;
}

int main()
{
  // 获取当前日志名称
  auto now = spdlog::log_clock::now();
  spdlog::filename_t base_filename = "/tmp/monthly.log";
  auto cur_log_name = spdlog::sinks::monthly_filename_calculator::calc_filename(base_filename, spdlog::details::os::localtime(spdlog::log_clock::to_time_t(now)));
  printf("%s\n", cur_log_name.c_str());

  // 获取 7 个月之前的日志
  auto before_7 = before_7_month(now);
  auto before_7_log_name = spdlog::sinks::monthly_filename_calculator::calc_filename(base_filename, spdlog::details::os::localtime(spdlog::log_clock::to_time_t(before_7)));
  printf("%s\n", before_7_log_name.c_str());  

  auto before_14 = before_7_month(before_7);
  auto before_14_log_name = spdlog::sinks::monthly_filename_calculator::calc_filename(base_filename, spdlog::details::os::localtime(spdlog::log_clock::to_time_t(before_14)));
  printf("%s\n", before_14_log_name.c_str());  

  auto before_21 = before_7_month(before_14);
  auto before_21_log_name = spdlog::sinks::monthly_filename_calculator::calc_filename(base_filename, spdlog::details::os::localtime(spdlog::log_clock::to_time_t(before_21)));
  printf("%s\n", before_21_log_name.c_str()); 

  auto before_28 = before_7_month(before_21);
  auto before_28_log_name = spdlog::sinks::monthly_filename_calculator::calc_filename(base_filename, spdlog::details::os::localtime(spdlog::log_clock::to_time_t(before_28)));
  printf("%s\n", before_28_log_name.c_str()); 

  return 0;
}
