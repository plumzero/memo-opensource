cmake_minimum_required(VERSION 2.8.0)

project("ZMQ Test" C CXX)

option(BUILD_01_REQ_REP        "Build 01_req_rep program"   ON)
option(BUILD_02_PUB_SUB        "Build 02_pub_sub program"   ON)
option(BUILD_03_PUB_SUB        "Build 03_pub_sub program"   ON)
option(BUILD_04_PUB_SUB        "Build 04_pub_sub program"   ON)
option(BUILD_05_PUSH_PULL      "Build 05_push_pull program" ON)

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
find_package(zmq REQUIRED)
include_directories(SYSTEM ${zmq_INCLUDE})
get_filename_component(zmq_LIBRARY_PATH ${zmq_LIBRARY} PATH)
get_filename_component(zmq_LIBRARY_NAME ${zmq_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" zmq_LIBRARY_NAME ${zmq_LIBRARY_NAME})

link_directories(${zmq_LIBRARY_PATH})

#
# "zmq.hpp" Support
#
find_package(zmqhpp REQUIRED)
include_directories(SYSTEM ${zmqhpp_INCLUDE})
if(ZMQHPP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
endif()

#
# Test in Sub Directories
#
if(BUILD_01_REQ_REP)
    add_subdirectory(01_req_rep)
endif()

if(BUILD_02_PUB_SUB)
    add_subdirectory(02_pub_sub)
endif()

if(BUILD_03_PUB_SUB)
    add_subdirectory(03_pub_sub)
endif()

if(BUILD_04_PUB_SUB)
    add_subdirectory(04_pub_sub)
endif()

if(BUILD_05_PUSH_PULL)
    add_subdirectory(05_push_pull)
endif()

#
# End
#
