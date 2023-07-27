cmake_minimum_required(VERSION 3.0)

add_executable(helloworld helloworld.cpp)
target_link_libraries(helloworld ${glog_LIBRARY_NAME})
