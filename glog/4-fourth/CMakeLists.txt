cmake_minimum_required(VERSION 3.0)

add_executable(maxsizelogs maxsizelogs.cpp)
target_link_libraries(maxsizelogs ${glog_LIBRARY_NAME})
