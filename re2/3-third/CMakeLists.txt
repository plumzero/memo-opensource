
cmake_minimum_required(VERSION 2.8.0)

add_executable(PartialMatch PartialMatch.cpp)
target_link_libraries(PartialMatch ${re2_LIBRARY_NAME})
