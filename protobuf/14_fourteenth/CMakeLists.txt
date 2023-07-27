cmake_minimum_required(VERSION 2.8.0)

#
# protoc --cpp_out=. fourteenth.proto
#

include_directories(.)

add_executable(testFourteenth testFourteenth.cpp fourteenth.pb.cc)
target_link_libraries(testFourteenth ${protobuf_LIBRARY_NAME})