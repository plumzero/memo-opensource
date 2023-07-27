
cmake_minimum_required(VERSION 3.5)

project("thread pool test" CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -std=c++11 -g -O2 -Wall -W")

set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(file ${files})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file})
    target_link_libraries(${name} pthread)
endforeach()
