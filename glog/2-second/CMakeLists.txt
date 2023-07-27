cmake_minimum_required(VERSION 3.0)

add_executable(efficiency efficiency.cpp)
target_link_libraries(efficiency ${glog_LIBRARY_NAME})
