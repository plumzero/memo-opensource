cmake_minimum_required(VERSION 2.8.0)

#
# Package re2
#
find_package(re2 REQUIRED)
include_directories(SYSTEM ${re2_INCLUDE})
get_filename_component(re2_LIBRARY_PATH ${re2_LIBRARY} PATH)
get_filename_component(re2_LIBRARY_NAME ${re2_LIBRARY} NAME_WE)
string(REGEX REPLACE "^lib" "" re2_LIBRARY_NAME ${re2_LIBRARY_NAME})

link_directories(${re2_LIBRARY_PATH})

#
# protoc --cpp_out=. tenth.proto
#

include_directories(.)

add_executable(Serialize Serialize.cpp tenth.pb.cc)
target_link_libraries(Serialize ${re2_LIBRARY_NAME} ${protobuf_LIBRARY_NAME})

add_executable(Structuralize Structuralize.cpp tenth.pb.cc)
target_link_libraries(Structuralize ${re2_LIBRARY_NAME} ${protobuf_LIBRARY_NAME})
