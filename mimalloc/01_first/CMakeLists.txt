cmake_minimum_required(VERSION 2.8.0)

## protoc --cpp_out=. struct.proto

#
# Package mimalloc 1
# 
find_package(mimalloc1 REQUIRED)
include_directories(SYSTEM ${mimalloc1_INCLUDE})
get_filename_component(mimalloc1_LIBRARY_PATH ${mimalloc1_LIBRARY} PATH)
get_filename_component(mimalloc1_LIBRARY_NAME ${mimalloc1_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" mimalloc1_LIBRARY_NAME ${mimalloc1_LIBRARY_NAME})

link_directories(${mimalloc1_LIBRARY_PATH})

include_directories(.)

add_executable(version1 version1.cpp struct.pb.cc)
target_link_libraries(version1 ${mimalloc1_LIBRARY_NAME} ${protobuf_LIBRARY_NAME})
