cmake_minimum_required(VERSION 2.8.0)

project("readerwriterqueue Test" CXX)

option(BUILD_1_FIRST_TEST   "Build 1-first program"        ON)
option(BUILD_2_SECOND_TEST  "Build 2-second program"       ON)
option(BUILD_3_THIRD_TEST   "Build 3-third program"        ON)

option(BUILD_B_1_FIRST_TEST   "Build b-1-first  program"   ON)
option(BUILD_B_2_SECOND_TEST  "Build b-2-second program"   ON)
option(BUILD_B_3_THIRD_TEST   "Build b-3-third  program"   ON)
option(BUILD_B_4_FORTH_TEST   "Build b-4-forth  program"   ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -Wall -Wextra -Wno-unused-but-set-variable -pedantic -Wno-unknown-pragmas")
else()
	string(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall -Wextra -Wno-unused-but-set-variable -pedantic -Wno-unknown-pragmas")
endif()

include_directories(SYSTEM ..)

if (BUILD_1_FIRST_TEST)
    add_subdirectory(1-first)
endif()

if (BUILD_2_SECOND_TEST)
    add_subdirectory(2-second)
endif()

if (BUILD_3_THIRD_TEST)
    add_subdirectory(3-third)
endif()

if (BUILD_B_1_FIRST_TEST)
    add_subdirectory(b-1-first)
endif()

if (BUILD_B_2_SECOND_TEST)
    add_subdirectory(b-2-second)
endif()

if (BUILD_B_3_THIRD_TEST)
    add_subdirectory(b-3-third)
endif()

if (BUILD_B_4_FORTH_TEST)
    add_subdirectory(b-4-forth)
endif()
