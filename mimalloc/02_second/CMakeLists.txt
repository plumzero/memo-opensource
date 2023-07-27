cmake_minimum_required(VERSION 2.8.0)

## protoc --cpp_out=. struct.proto

#
# Package mimalloc 2
# 
find_package(mimalloc2 REQUIRED)
include_directories(SYSTEM ${mimalloc2_INCLUDE})
get_filename_component(mimalloc2_LIBRARY_PATH ${mimalloc2_LIBRARY} PATH)
get_filename_component(mimalloc2_LIBRARY_NAME ${mimalloc2_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" mimalloc2_LIBRARY_NAME ${mimalloc2_LIBRARY_NAME})

link_directories(${mimalloc2_LIBRARY_PATH})

include_directories(.)

add_executable(version2 version2.cpp struct.pb.cc)
target_link_libraries(version2 ${mimalloc2_LIBRARY_NAME} ${protobuf_LIBRARY_NAME})
