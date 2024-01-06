cmake_minimum_required(VERSION 2.8.0)

project("sqlite test" C CXX)

option(BUILD_01_FIRST   	"Build 01_first program"    		OFF)
option(BUILD_02_SECOND  	"Build 02_second program"   		ON)
option(BUILD_03_THIRD   	"Build 03_third program"     		ON)

set(CMAKE_CXX_STANDARD 14)

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

include("${PROJECT_SOURCE_DIR}/cmake/modules/Findsqlite.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/modules/Findsqliteorm.cmake")

#
# Package
#
include_directories(SYSTEM ${sqlite_INCLUDE})
include_directories(SYSTEM ${sqliteorm_INCLUDE})

#
# Test in Sub Directories
#
if(BUILD_01_FIRST)
  add_subdirectory(01_first)
endif()

if(BUILD_02_SECOND)
  add_subdirectory(02_second)
endif()

if(BUILD_03_THIRD)
  add_subdirectory(03_third)
endif()
