cmake_minimum_required(VERSION 2.8.0)

#
# protoc --cpp_out=. fifteenth.proto
#

# 本次测试的目标不是特定结构编码，而是结构的长度，及为结构赋值之后的长度

include_directories(.)

add_executable(testFifteenth testFifteenth.cpp fifteenth.pb.cc)
target_link_libraries(testFifteenth ${protobuf_LIBRARY_NAME})
