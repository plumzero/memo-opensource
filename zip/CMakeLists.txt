cmake_minimum_required(VERSION 3.4)

project("ziptest" C CXX)

set(CMAKE_BUILD_TYPE DEBUG)

include_directories(.)

add_executable(example example.c zip.c)
add_executable(decompressing decompressing.cpp zip.c)
