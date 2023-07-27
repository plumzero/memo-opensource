cmake_minimum_required(VERSION 2.8)

add_executable(first first.cpp)
target_link_libraries(first pthread)
