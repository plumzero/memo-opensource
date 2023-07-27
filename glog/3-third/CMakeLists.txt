cmake_minimum_required(VERSION 3.0)

add_executable(variable_argument variable_argument.cpp)
target_link_libraries(variable_argument ${glog_LIBRARY_NAME})

add_executable(va_efficiency va_efficiency.cpp)
target_link_libraries(va_efficiency ${glog_LIBRARY_NAME})
