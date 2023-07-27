cmake_minimum_required(VERSION 2.8.0)

## protoc --cpp_out=. eighth.proto

include_directories(.)

add_executable(testAccountOverviews testAccountOverviews.cpp eighth.pb.cc)
target_link_libraries(testAccountOverviews ${protobuf_LIBRARY_NAME})

add_executable(testHistoryResult testHistoryResult.cpp eighth.pb.cc)
target_link_libraries(testHistoryResult ${protobuf_LIBRARY_NAME})