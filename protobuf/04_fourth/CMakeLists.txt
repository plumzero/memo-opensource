cmake_minimum_required(VERSION 2.8.0)

## protoc --cpp_out=. trade.proto

include_directories(.)

add_executable(testAccountInfos testAccountInfos.cpp trade.pb.cc)
target_link_libraries(testAccountInfos ${protobuf_LIBRARY_NAME})
