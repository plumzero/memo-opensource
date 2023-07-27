cmake_minimum_required(VERSION 2.8.0)

#
# protoc --cpp_out=. sixteenth.proto
#

# 使用引用

include_directories(.)

add_executable(testSixteenth testSixteenth.cpp sixteenth.pb.cc)
target_link_libraries(testSixteenth ${protobuf_LIBRARY_NAME})
