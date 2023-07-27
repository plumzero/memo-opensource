
cmake_minimum_required(VERSION 3.5)

project("nlohmann test" CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -std=c++11 -g -O0 -Wall -W")

set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(.)

# add_subdirectory(01_first)
add_subdirectory(02_second)
