cmake_minimum_required(VERSION 2.8.0)

project("Gtest Test" C CXX)

#
# if u wanna debug, do like this `cmake -DCMAKE_BUILD_TYPE=Debug`
#
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#
# Configuration
#
include("${PROJECT_SOURCE_DIR}/cmake/configuration.cmake")

#
# Modules
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")

find_package(gtest REQUIRED)
include_directories(SYSTEM ${gtest_INCLUDE})
link_directories(${gtest_LIBRARY_PATH})

add_subdirectory(01_first)
