cmake_minimum_required(VERSION 2.8)

add_executable(b_second b_second.cpp)
target_link_libraries(b_second pthread)