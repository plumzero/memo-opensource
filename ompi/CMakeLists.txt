cmake_minimum_required(VERSION 3.5.0)

project("pmi test" C CXX)

option(BUILD_1_first            "build 1 first"                  ON)

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
# Package - pmi
#
find_package(pmi REQUIRED)
include_directories(SYSTEM ${pmi_INCLUDE})
get_filename_component(pmi_LIBRARY_PATH ${pmi_LIBRARY} PATH)
get_filename_component(pmi_LIBRARY_NAME ${pmi_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" pmi_LIBRARY_NAME ${pmi_LIBRARY_NAME})

link_directories(${pmi_LIBRARY_PATH})

#
# Test in Sub Directories
#
if(BUILD_1_first)
  add_subdirectory(01_first)
endif()

#
# End
#
