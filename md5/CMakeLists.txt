
cmake_minimum_required(VERSION 3.5)

project("cmdline test" CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -DNDEBUG -Wall -W")

set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(.)

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(file ${files})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file} md5.cc)
endforeach()
