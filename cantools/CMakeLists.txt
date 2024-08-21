
cmake_minimum_required(VERSION 2.6)

project("cantools my test" C CXX)

set(CMAKE_CXX_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -W ")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -w")
endif()

add_subdirectory(motohawk)
add_subdirectory(hawkmoto)
