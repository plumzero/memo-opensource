cmake_minimum_required(VERSION 2.8.0)

project("mimalloc test" C CXX)

option(BUILD_01_FIRST_TEST   	"Build 01_first program"    		ON)
option(BUILD_02_SECOND_TEST  	"Build 02_second program"   		ON)


#
# if u wanna release, do like this `cmake -DCMAKE_BUILD_TYPE=Release`
#
message(STATUS "")    
if (NOT CMAKE_BUILD_TYPE)
  	if ("${CMAKE_BINARY_DIR}" MATCHES ".*(D|d)ebug$" OR  MI_DEBUG_FULL)
		message(STATUS "No build type selected, default to: Debug")
		set(CMAKE_BUILD_TYPE "Debug")
  	else()
		message(STATUS "No build type selected, default to: Release")
		set(CMAKE_BUILD_TYPE "Release")
  	endif()
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
# Package protobuf
#
find_package(protobuf REQUIRED)
include_directories(SYSTEM ${protobuf_INCLUDE})
get_filename_component(protobuf_LIBRARY_PATH ${protobuf_LIBRARY} PATH)
get_filename_component(protobuf_LIBRARY_NAME ${protobuf_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" protobuf_LIBRARY_NAME ${protobuf_LIBRARY_NAME})

link_directories(${protobuf_LIBRARY_PATH})

#
# Other
#

include_directories(.)

#
# Test in Sub Directories
#
if(BUILD_01_FIRST_TEST)
    add_subdirectory(01_first)
endif()

if(BUILD_02_SECOND_TEST)
    add_subdirectory(02_second)
endif()

if(BUILD_03_THIRD_TEST)
    add_subdirectory(03_third)
endif()

#
# End
#
