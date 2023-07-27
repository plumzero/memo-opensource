
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -g -O2 -std=c++11")

option(BUILD_EXE_RAJTEST        "build executable rajtest"          ON)
option(BUILD_EXE_EFFICIENCY     "build executable efficiency"       ON)
option(BUILD_EXE_ITERATOR       "build executable ITERATOR"         ON)

include_directories(../include)

if (BUILD_EXE_RAJTEST)
    add_executable(rajtest  rajtest.cpp bic.cpp)
    target_link_libraries(rajtest pthread)
endif(BUILD_EXE_RAJTEST)

if (BUILD_EXE_EFFICIENCY)
    add_executable(efficiency  efficiency.cpp)
    target_link_libraries(efficiency pthread)
endif(BUILD_EXE_EFFICIENCY)

if (BUILD_EXE_ITERATOR)
    add_executable(iterator  iterator.cpp)
    target_link_libraries(iterator pthread)
endif(BUILD_EXE_ITERATOR)