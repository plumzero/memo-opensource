cmake_minimum_required(VERSION 2.8.0)

#
# protoc --cpp_out=. eleventh.proto
#

include_directories(.)

add_executable(testEleventh testEleventh.cpp eleventh.pb.cc)
target_link_libraries(testEleventh ${protobuf_LIBRARY_NAME})