cmake_minimum_required(VERSION 3.5.0)

project("grpc Test" C CXX)

option(BUILD_1_FIRST_TEST       "Build 1-first program"          ON)
option(BUILD_2_SECOND_TEST      "Build 2-second program"         ON)
option(BUILD_3_STREAM_TEST      "Build 3-stream program"         ON)
option(BUILD_4_COMPRESS_TEST    "Build 4-compress program"       ON)

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
# Package - protobuf
#
find_package(protobuf REQUIRED)
include_directories(SYSTEM ${protobuf_INCLUDE})
get_filename_component(protobuf_LIBRARY_PATH ${protobuf_LIBRARY} PATH)
get_filename_component(protobuf_LIBRARY_NAME ${protobuf_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" protobuf_LIBRARY_NAME ${protobuf_LIBRARY_NAME})

link_directories(${protobuf_LIBRARY_PATH})

#
# Package - grpc
#
find_package(grpc REQUIRED)
include_directories(SYSTEM ${grpc_INCLUDE})
get_filename_component(grpc_LIBRARY_PATH ${grpc_LIBRARY} PATH)
get_filename_component(grpc_LIBRARY_NAME ${grpc_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" grpc_LIBRARY_NAME ${grpc_LIBRARY_NAME})

link_directories(${grpc_LIBRARY_PATH})

#
# Support grpcpp
#
find_package(grpcpp REQUIRED)
include_directories(SYSTEM ${grpcpp_INCLUDE})
get_filename_component(grpc++_LIBRARY_PATH ${grpc++_LIBRARY} PATH)
get_filename_component(grpc++_LIBRARY_NAME ${grpc++_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" grpc++_LIBRARY_NAME ${grpc++_LIBRARY_NAME})

get_filename_component(grpc++_reflection_LIBRARY_PATH ${grpc++_reflection_LIBRARY} PATH)
get_filename_component(grpc++_reflection_LIBRARY_NAME ${grpc++_reflection_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" grpc++_reflection_LIBRARY_NAME ${grpc++_reflection_LIBRARY_NAME})

#
# Test in Sub Directories
#
if(BUILD_1_FIRST_TEST)
    add_subdirectory(01_first)
endif()

if(BUILD_2_SECOND_TEST)
    add_subdirectory(02_second)
endif()

if(BUILD_3_STREAM_TEST)
    add_subdirectory(03_stream)
endif()

if (BUILD_4_COMPRESS_TEST)
add_subdirectory(04_compress)
endif()

#
# End
#
