
cmake_minimum_required(VERSION 2.8.0)

add_executable(FullMatch FullMatch.cpp)
target_link_libraries(FullMatch ${re2_LIBRARY_NAME})
