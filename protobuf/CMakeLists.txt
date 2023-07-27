cmake_minimum_required(VERSION 2.8.0)

project("Protobuf Test" C CXX)

option(BUILD_01_FIRST_TEST   	"Build 01_first program"    		ON)
option(BUILD_02_SECOND_TEST  	"Build 02_second program"   		ON)
option(BUILD_03_THIRD_TEST   	"Build 03_third program"    		ON)
option(BUILD_04_FOURTH_TEST   	"Build 04_fourth program"    		ON)
option(BUILD_05_FIFTH_TEST   	"Build 05_fifth program"    		ON)
option(BUILD_06_SIXTH_TEST   	"Build 06_sixth program"   			ON)
option(BUILD_07_SEVENTH_TEST 	"Build 07_seventh program"  		ON)
option(BUILD_08_EIGHTH_TEST  	"Build 08_eighth program"   		ON)
option(BUILD_09_NINTH_TEST   	"Build 09_ninth program"    		ON)
option(BUILD_10_TENTH_TEST  	"Build 10_tenth program"   			OFF)
option(BUILD_11_ELEVENTH_TEST  	"Build 11_eleventh program" 		ON)
option(BUILD_12_TWELFTH_TEST  	"Build 12_twelfth program"  		ON)
option(BUILD_13_THIRTEENTH_TEST "Build 13_thirteenth program"  		ON)
option(BUILD_14_FOURTEENTH_TEST "Build 14_fourteenth program"  		ON)
option(BUILD_15_FIFTEENTH_TEST  "Build 15_fifteenth program"  		ON)
option(BUILD_16_SIXTEENTH_TEST  "Build 16_sixteenth program"  		ON)

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

if(BUILD_04_FOURTH_TEST)
	add_subdirectory(04_fourth)
endif()

if(BUILD_05_FIFTH_TEST)
	add_subdirectory(05_fifth)
endif()

if(BUILD_06_SIXTH_TEST)
	add_subdirectory(06_sixth)
endif()

if(BUILD_07_SEVENTH_TEST)
	add_subdirectory(07_seventh)
endif()

if(BUILD_08_EIGHTH_TEST)
	add_subdirectory(08_eighth)
endif()

if(BUILD_09_NINTH_TEST)
	add_subdirectory(09_ninth)
endif()

if(BUILD_10_TENTH_TEST)
	add_subdirectory(10_tenth)
endif()

if(BUILD_11_ELEVENTH_TEST)
	add_subdirectory(11_eleventh)
endif()

if(BUILD_12_TWELFTH_TEST)
	add_subdirectory(12_twelfth)
endif()

if(BUILD_13_THIRTEENTH_TEST)
	add_subdirectory(13_thirteenth)
endif()

if(BUILD_14_FOURTEENTH_TEST)
	add_subdirectory(14_fourteenth)
endif()

if(BUILD_15_FIFTEENTH_TEST)
	add_subdirectory(15_fifteenth)
endif()

if(BUILD_16_SIXTEENTH_TEST)
	add_subdirectory(16_sixteenth)
endif()

#
# End
#
