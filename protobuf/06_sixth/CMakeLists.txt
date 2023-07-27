cmake_minimum_required(VERSION 2.8.0)

## protoc --cpp_out=. sixth.proto

include_directories(.)

add_executable(testSixth testSixth.cpp sixth.pb.cc)
target_link_libraries(testSixth ${protobuf_LIBRARY_NAME})
