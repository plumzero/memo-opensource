
cmake_minimum_required(VERSION 2.8.0)

add_executable(01_first  01_first.cpp)
target_link_libraries(01_first ${gtest_LIBRARY_NAME} ${gtest_main_LIBRARY_NAME})

