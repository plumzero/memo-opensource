cmake_minimum_required(VERSION 2.8)

add_executable(third third.cpp)
target_link_libraries(third pthread)