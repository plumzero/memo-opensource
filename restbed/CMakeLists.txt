
cmake_minimum_required(VERSION 2.8.10)

project( restbed CXX )

option(BUILD_1_FIRST   	"Build 1-first program"    		ON)
option(BUILD_2_SECOND  	"Build 2-second program"   		OFF)
option(BUILD_3_THIRD   	"Build 3-third program"    		OFF)
option(BUILD_4_FOURTH   "Build 4-fourth program"    	OFF)
option(BUILD_5_FIFTH   	"Build 5-fifth program"    		OFF)
option(BUILD_6_SIXTH   	"Build 6-sixth program"   		OFF)
option(BUILD_7_SEVENTH 	"Build 7-seventh program"  		ON)
option(BUILD_8_EIGHTH 	"Build 8-eighth program"  		ON)

string(REGEX REPLACE "/mytest$" "" PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})

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
# Set Module Path
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules")

#
# Find Dependencies
#
find_package(asio REQUIRED)
include_directories(SYSTEM ${asio_INCLUDE})

find_package(kashmir REQUIRED)
include_directories(SYSTEM ${kashmir_INCLUDE})

#
# Build
#
include("${PROJECT_SOURCE_DIR}/cmake/manifest.cmake")

include_directories(${INCLUDE_DIR})
include_directories(.)

set(RestbedSRCS ${MANIFEST})

include_directories(.)

#
# Test in Sub Directories
#
if(BUILD_1_FIRST)
    add_subdirectory(01_first)
endif()

if(BUILD_2_SECOND)
    add_subdirectory(02_second)
endif()

if(BUILD_3_THIRD)
    add_subdirectory(03_third)
endif()

if(BUILD_4_FORTH)
	add_subdirectory(04_fourth)
endif()

if(BUILD_5_FIFTH)
	add_subdirectory(05_fifth)
endif()

if(BUILD_6_SIXTH)
	add_subdirectory(06_sixth)
endif()

if(BUILD_7_SEVENTH)
	add_subdirectory(07_seventh)
endif()

if(BUILD_8_EIGHTH)
	add_subdirectory(08_eighth)
endif()