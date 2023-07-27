cmake_minimum_required(VERSION 2.8)

add_executable(b_first b_first.cpp)
target_link_libraries(b_first pthread)