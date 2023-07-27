
cmake_minimum_required(VERSION 2.8.0)

project("Re2 Test" C CXX)

option(BUILD_1_FIRST_TEST   "Build 1-first program"    ON)
option(BUILD_2_SECOND_TEST  "Build 2-second program"   ON)
option(BUILD_3_THIRD_TEST   "Build 3-third program"    ON)
option(BUILD_4_FORTH_TEST   "Build 4-forth program"    OFF)

#
# if u wanna release, do like this `cmake -DCMAKE_BUILD_TYPE=Release`
#
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(CheckCXXCompilerFlag)

#
# Configuration
#
include("${PROJECT_SOURCE_DIR}/cmake/configuration.cmake")

#
# Dependencies
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")

#
# Package
#
find_package(re2 REQUIRED)
include_directories(SYSTEM ${re2_INCLUDE})
get_filename_component(re2_LIBRARY_PATH ${re2_LIBRARY} PATH)
get_filename_component(re2_LIBRARY_NAME ${re2_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" re2_LIBRARY_NAME ${re2_LIBRARY_NAME})

link_directories(${re2_LIBRARY_PATH})

#
# Test in Sub Directories
#
if(BUILD_1_FIRST_TEST)
    add_subdirectory(1-first)
endif()

if(BUILD_2_SECOND_TEST)
    add_subdirectory(2-second)
endif()

if(BUILD_3_THIRD_TEST)
    add_subdirectory(3-third)
endif()

if(BUILD_4_FORTH_TEST)
	add_subdirectory(4-forth)
endif()

#
# End
#
