cmake_minimum_required(VERSION 2.8.0)

## protoc --cpp_out=. trade.proto

include_directories(.)

add_executable(testTradeAccountRsp2 testTradeAccountRsp2.cpp trade.pb.cc)
target_link_libraries(testTradeAccountRsp2 ${protobuf_LIBRARY_NAME})
