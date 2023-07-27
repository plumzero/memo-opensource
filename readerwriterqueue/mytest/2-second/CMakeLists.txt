cmake_minimum_required(VERSION 2.8)

add_executable(second second.cpp)
target_link_libraries(second pthread)