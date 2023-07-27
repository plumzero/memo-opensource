cmake_minimum_required(VERSION 2.8)

add_executable(b_third b_third.cpp)
target_link_libraries(b_third pthread)