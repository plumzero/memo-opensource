cmake_minimum_required(VERSION 2.8.0)

#
# protoc --cpp_out=. twelfth.proto
#

include_directories(.)

add_executable(testTwelfth testTwelfth.cpp twelfth.pb.cc)
target_link_libraries(testTwelfth ${protobuf_LIBRARY_NAME})