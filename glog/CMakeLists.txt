cmake_minimum_required(VERSION 3.0)

project("glog Test" C CXX)

option(BUILD_1_FIRST_TEST  "Build 1-first program"      OFF)
option(BUILD_2_SECOND_TEST "Build 2-second program"     ON)
option(BUILD_3_THIRD_TEST  "Build 3-third program"      ON)
option(BUILD_4_FOURTH_TEST "Build 4-fourth program"     ON)

#
# if u wanna debug, do like this `cmake -DCMAKE_BUILD_TYPE=Debug`
#
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
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
# Package - glog
#
find_package(glog REQUIRED)
if(GLOG_FOUND)
    include_directories(SYSTEM ${glog_INCLUDE})
    get_filename_component(glog_LIBRARY_PATH ${glog_LIBRARY} PATH)
    get_filename_component(glog_LIBRARY_NAME ${glog_LIBRARY} NAME_WE)
    string(REGEX REPLACE "^lib" "" glog_LIBRARY_NAME ${glog_LIBRARY_NAME})
endif()

link_directories(${glog_LIBRARY_PATH})

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

#
# End
#
