
cmake_minimum_required(VERSION 2.8.0)

add_executable(FirstMatch FirstMatch.cpp)
target_link_libraries(FirstMatch ${re2_LIBRARY_NAME})
