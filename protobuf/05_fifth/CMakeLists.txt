cmake_minimum_required(VERSION 2.8.0)

## protoc --cpp_out=. mtable.proto

include_directories(.)

add_executable(testPriceList testPriceList.cpp mtable.pb.cc)
target_link_libraries(testPriceList ${protobuf_LIBRARY_NAME})
