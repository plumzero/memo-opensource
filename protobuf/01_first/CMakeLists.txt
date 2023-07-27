cmake_minimum_required(VERSION 2.8.0)

## protoc --cpp_out=. addressbook.proto

include_directories(.)

add_executable(testFirst testFirst.cpp addressbook.pb.cc)
target_link_libraries(testFirst ${protobuf_LIBRARY_NAME})