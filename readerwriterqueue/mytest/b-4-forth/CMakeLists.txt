cmake_minimum_required(VERSION 2.8)

add_executable(b_forth b_forth.cpp)
target_link_libraries(b_forth pthread)