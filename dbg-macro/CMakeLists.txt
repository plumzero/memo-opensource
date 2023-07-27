
cmake_minimum_required(VERSION 2.8.10)

project("dbg-macro test" CXX)

set(CMAKE_CXX_STANDARD 11)
include(CheckCXXCompilerFlag)

include_directories(.)

add_executable(example example.cpp)
add_executable(demo demo.cpp)
