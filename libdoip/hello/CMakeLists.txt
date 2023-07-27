
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_BUILD_TYPE Debug)

# set(CMAKE_VERBOSE_MAKEFILE on)

#
# libdoip
#
include_directories(
  ${CMAKE_SOURCE_DIR}/libdoipclient/include
  ${CMAKE_SOURCE_DIR}/libdoipserver/include
  ${CMAKE_SOURCE_DIR}/libdoipcommon/include
)

file(GLOB LIBDOIP_SRCS
  ${CMAKE_SOURCE_DIR}/libdoipclient/src/*.cpp
  ${CMAKE_SOURCE_DIR}/libdoipserver/src/*.cpp
  ${CMAKE_SOURCE_DIR}/libdoipcommon/src/*.cpp
)

#
# gtest
#
include_directories(
  /usr/local/gtest/include
)

link_directories(
  /usr/local/gtest/lib
)

#
# realize
#
include_directories(
  ${CMAKE_SOURCE_DIR}/realize
)

file(GLOB REALIZE_SRCS
  ${CMAKE_SOURCE_DIR}/realize/*.cpp
)

add_subdirectory(unittest)
