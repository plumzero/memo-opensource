cmake_minimum_required(VERSION 3.12)

project("hdf5 Test" C CXX)

option(BUILD_1_FIRST_TEST  "Build 1-first program"      ON)
option(BUILD_2_SECOND_TEST "Build 2-second program"     ON)
option(BUILD_3_THIRD_TEST  "Build 3-third program"      ON)
option(BUILD_4_FOURTH_TEST "Build 4-fourth program"     ON)
option(BUILD_5_FIFTH_TEST  "Build 5-fifth program"      ON)
option(BUILD_6_SIXTH_TEST  "Build 6-sixth program"      ON)

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
# Package - hdf5
#
find_package(hdf5 REQUIRED)
include_directories(SYSTEM ${hdf5_INCLUDE})
get_filename_component(hdf5_LIBRARY_PATH ${hdf5_LIBRARY} PATH)
get_filename_component(hdf5_LIBRARY_NAME ${hdf5_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" hdf5_LIBRARY_NAME ${hdf5_LIBRARY_NAME})

link_directories(${hdf5_LIBRARY_PATH})

#
# Support hdf5cpp
#
find_package(hdf5cpp REQUIRED)
include_directories(SYSTEM ${hdf5cpp_INCLUDE})
get_filename_component(hdf5cpp_LIBRARY_PATH ${hdf5cpp_LIBRARY} PATH)
get_filename_component(hdf5cpp_LIBRARY_NAME ${hdf5cpp_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" hdf5cpp_LIBRARY_NAME ${hdf5cpp_LIBRARY_NAME})

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

if(BUILD_4_FOURTH_TEST)
    add_subdirectory(4-fourth)
endif()

if(BUILD_5_FIFTH_TEST)
    add_subdirectory(5-fifth)
endif()

if(BUILD_6_SIXTH_TEST)
    add_subdirectory(6-sixth)
endif()

#
# End
#
