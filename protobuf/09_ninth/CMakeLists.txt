cmake_minimum_required(VERSION 2.8.0)

## protoc --cpp_out=. ninth.proto

include_directories(.)

add_executable(testNinth testNinth.cpp ninth.pb.cc)
target_link_libraries(testNinth ${protobuf_LIBRARY_NAME})
